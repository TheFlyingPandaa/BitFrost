cbuffer EXAMPLE_BUFFER : register(b3)
{
	float4x4 WVP;
	float4x4 worldSpace;
	float4x4 wwo;
};

cbuffer Light_Buffer : register(b9)
{
    float4x4 view;
    float4x4 projection;
};

struct VS_IN
{
	float3 Pos : POSITION;
	//float2 Tex : TEXCOORD;
	//float3 Normal : NORMAL;
};

struct VS_OUT
{
	float4 Pos : POSITION;
	//float2 Tex : TEXCOORD;
	//float3 Normal : NORMAL;
	//float4x4 mulMatrix;
};

VS_OUT main( VS_IN input) : SV_POSITION
{
	VS_OUT output = (VS_OUT)0;
	
    output.Pos = mul(mul(float4(input.Pos, 1.0f), worldSpace), mul(view, projection));

	return output;
}