cbuffer EXAMPLE_BUFFER : register(b1)
{
    float4x4 WVP;
    float4x4 worldSpace;
};

struct VS_IN
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
    //float3 Normal : NORMAL;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Tex : TEXCOORD;
};


VS_OUT main(VS_IN input )
{
    VS_OUT output;

    output.Pos = mul(float4(input.Pos, 1),WVP); //mul(float4(input.Pos, 1.0f), WVP);
    output.Tex = input.Pos;

    return output;
}