cbuffer EXAMPLE_BUFFER : register(b1)
{
    float4x4 WVP;
    float4x4 worldSpace;
};

struct VertexShaderInput
{
    float3 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

 
struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float3 TextureCoordinate : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;

    output.Position = mul(float4(input.Pos, 1), WVP);
    output.TextureCoordinate = input.Pos;

    return output;
}