Texture2D txDiffuse : register(t0);
SamplerState sampAni;
struct PixelOutDeferred
{
    float4 diffuse : SV_Target0;
    float4 normal : SV_Target1;
    float4 position : SV_Target2;
};

struct VertexShaderOutput
{
    float4 Position : SV_POSITION;
    float3 TextureCoordinate : TEXCOORD0;
};

PixelOutDeferred main(VertexShaderOutput input) : SV_TARGET
{
    PixelOutDeferred pOut;

    pOut.diffuse = txDiffuse.Sample(sampAni, input.TextureCoordinate);
    pOut.normal = (1,1,1, 1.0f);
    pOut.position = input.Position;

    return pOut;

}