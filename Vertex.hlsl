
struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
    //float3 Normal : NORMAL;
};

struct VS_OUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
    //float3 Normal : NORMAL;
	//float4x4 mulMatrix;
};


VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Pos = float4(input.Pos, 1);
	output.Tex = input.Tex;

	return output;
}