Texture2D txDiffuse : register(t0);
SamplerState sampAni;

Texture2D gTexDiffuse : register(t1);
Texture2D gTexNormal : register(t2);
Texture2D gTexPosition : register(t3);
Texture2D gTexShadow : register(t4);

struct Light
{
    float3 dir;
	float pad;
    float4 ambient;
    float4 diffuse;
	float4 position;
};

cbuffer PER_FRAME_BUFF : register(b1)
{
    Light light;
    //float4x4 view;
};

cbuffer CAMERA_BUFFERT : register(b2)
{
	float4 cameraPosition;
};

cbuffer EXAMPLE_BUFFER : register(b4)
{
	float4x4 WVP;
	float4x4 worldSpace;
	float4x4 wwo;
	float4x4 lightVP;
};


struct VS_OUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
	//float4x4 mulMatrix;
};

float4 ambientLight = (1.0f, 1.0f, 1.0f, 1.0f);



float4 main(VS_OUT input) : SV_Target
{


    float4 pOut;
    float3 normal = gTexNormal.Sample(sampAni, input.Tex).rgb;
    float3 diffuse = gTexDiffuse.Sample(sampAni, input.Tex).rgb;
	float3 shadow = gTexShadow.Sample(sampAni, input.Tex).rgb;
	float3 position = gTexPosition.Sample(sampAni, input.Tex).rgb;
	normal = normalize(normal);
	
	
	float4 lightView = mul(float4(position,1.0f), lightVP ); //Swap if it dose't work
	//lightView.w = 1;
	lightView.xy /= lightView.w;


	float2 smTex = float2(0.5f*lightView.x + 0.5f, -0.5f*lightView.y + 0.5f);
	
	float depth = lightView.z / lightView.w;
	float SMAP_SIZE = 400;
	float dx = 1.0f / SMAP_SIZE;
	//depth /= lightView.w;
	float s0;
	float s1;
	float s2;
	float s3;
	if ((saturate(smTex.x) == smTex.x) && (saturate(smTex.y) == smTex.y))
	{
		

		s0 = (gTexShadow.Sample(sampAni, smTex).r + 0.0019 < depth) ? 0.0f : 1.0f;
		s1 = (gTexShadow.Sample(sampAni, smTex + float2(dx, 0.0f)).r + 0.0019 < depth) ? 0.0f : 1.0f;
		s2 = (gTexShadow.Sample(sampAni, smTex + float2(0.0f, dx)).r + 0.0019 < depth) ? 0.0f : 1.0f;
		s3 = (gTexShadow.Sample(sampAni, smTex + float2(dx, dx)).r + 0.0019 < depth) ? 0.0f : 1.0f;
	}
    else
    {
        return float4(diffuse,1.0f);
    }

	float2 texelPos = smTex * SMAP_SIZE;

	float2 lerps = frac(texelPos);

	float shadowCoeff = lerp(lerp(s0, s1, lerps.x), lerp(s2, s3, lerps.x), lerps.y);
	float3 litColor = diffuse.rgb * shadowCoeff;


                                //Lightpos
    float3 lightObj = normalize(float3(0,1,3).xyz - position);
    float3 viewFromCam = normalize(cameraPosition.xyz - position);

    float3 diffusee = diffuse * max(dot(normal, lightObj), 0.0);
	
    float3 halfDir = normalize(lightObj + viewFromCam);
		
    float spec = pow(max(dot(normal, halfDir), 0.0), 62.0);
		
    float3 specular = spec * 1.0f;
    return float4(light.ambient.xyz + (specular + diffuse) * shadowCoeff, 1.0);

}