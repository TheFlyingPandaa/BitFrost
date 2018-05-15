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

cbuffer Light_Buffer : register(b9)
{
    float4x4 view;
    float4x4 projection;
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
	
	
    float4 lightView = mul(float4(position, 1.0f), mul(view, projection)); // Translate the world position into the view space of the light
	//lightView.w = 1;
    lightView.xy /= lightView.w; // Get the texture coords of the "object" in the shadow map


    float2 smTex = float2(0.5f * lightView.x + 0.5f, -0.5f * lightView.y + 0.5f); // Texcoords are not [-1, 1], change the coords to [0, 1]
	
	float depth = lightView.z / lightView.w;
	float SMAP_SIZE = 400;
	float dx = 1.0f / SMAP_SIZE;
	//depth /= lightView.w;

    
																		  //posLightH = mul(posLightH, lightProj);					// Translate the view position into the projection space of the light
    
  
     
    //float depth = posLightH.z / posLightH.w; // Get the actual depth (seen from the camera)
    //float SHADOW_EPSILON = 0.001f; // Some small value

    //float angle = max(dot(sunLightToObject, float3(0, -1, 0)), 0.0f);

    
    if (abs(lightView.x) > 1.0f)							// Check if we are outside the shadow map (we are not in the light)
        return float4(diffuse.rgb, 1);
    if (abs(lightView.y) > 1.0f)							// Check if we are outside the shadow map (we are not in the light)
        return float4(diffuse.rgb, 1);

    float shadowCoeff = (gTexShadow.Sample(sampAni, smTex).r + 0.001 < depth) ? 0.5f : 1.0f; // If the depth from camera is larger than depth from light,
 
	float3 litColor = diffuse.rgb * shadowCoeff;

    return float4((diffuse) * shadowCoeff, 1.0);

}