Texture2D txDiffuse : register(t0);
SamplerState sampAni;
struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

struct Light
{
    float3 dir;
    float4 ambient;
    float4 diffuse;
};

cbuffer PER_FRAME_BUFF : register(b1)
{
    Light light;
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
    pOut.diffuse = float4(input.Normal, 1.0f);
    pOut.normal = float4(input.Normal, 1.0f);
    pOut.position = input.Pos;
    return pOut;

};