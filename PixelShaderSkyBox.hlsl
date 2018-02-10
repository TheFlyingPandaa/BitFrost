Texture2D txDiffuse : register(t0);
//SamplerState sampAni;

SamplerState samTriLinearSam
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
    return txDiffuse.Sample(samTriLinearSam, input.Tex);
}