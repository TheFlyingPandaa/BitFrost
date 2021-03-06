Texture2D txDiffuse : register(t0);
Texture2D txNormal : register(t1);
SamplerState sampAni;
struct GS_OUT
{
    float4 pos : SV_POSITION;
    float4 worldPos : PUSS;
    float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

struct Light
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
	float3 position;
};

cbuffer PER_FRAME_BUFF : register(b1)
{
    Light light;
};

cbuffer TEXTURE_INFORMATION : register(b2)
{
    bool useTex;
    bool useNormal;
    float4 tangent;
};

struct PixelOutDeferred
{
    float4 diffuse : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
};

PixelOutDeferred PS_main(GS_OUT input) : SV_Target
{
    //float3 normal = normalize(input.Normal);

    //float4 diffuse = txDiffuse.Sample(sampAni, input.Tex);
	
   

    

    //float3 finalColor;

    //finalColor = diffuse * light.ambient;
    //finalColor += saturate(dot(light.dir, input.Normal) * light.diffuse * diffuse);
    
    //return float4(finalColor, diffuse.a);

    PixelOutDeferred pOut;

    pOut.diffuse = txDiffuse.Sample(sampAni, input.Tex);    
    //pOut.diffuse = float4((input.Normal.x + 1) / 2, (input.Normal.y + 1) / 2, (input.Normal.z + 1) / 2, 1.0f);
    //pOut.normal = float4((input.Normal.x + 1) / 2, (input.Normal.y + 1) / 2, (input.Normal.z + 1) / 2, 1.0f);
    pOut.normal = float4(input.Normal, 1.0f);
    if (useNormal)
    {
        float3 n = txNormal.Sample(sampAni, input.Tex);

        n = (2.0f * n) - 1.0f;        

        float3 biTangent = cross(input.Normal, tangent.xyz);

        float3x3 texSpace = float3x3(tangent.xyz, biTangent, input.Normal);

        pOut.normal = float4(normalize(mul(n, texSpace)), 1.0f);

    }
    
    pOut.position = input.worldPos;
    return pOut;

};