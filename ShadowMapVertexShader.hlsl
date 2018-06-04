cbuffer EXAMPLE_BUFFER : register(b3)
{
	float4x4 LWVP;
	float4x4 worldSpace;
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
	
    output.Pos = mul(float4(input.Pos, 1.0f), LWVP);

	return output;
}