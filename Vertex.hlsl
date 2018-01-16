// NEW
// each constant buffer uses a register number (0)
// which has to match the number used in the CPU when
// setting the buffer for the shader to be able to use it.


struct VS_IN
{
	float3 Pos : POSITION;
	float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
	//float4x4 mulMatrix;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	// NEW
	// we can use value1 without the EXAMPLE_BUFFER prefix.
	//input.Pos.x += sin(value1);
	//input.Pos.y += sin(value2);
	//input.Pos.z += sin(value3);
	// ===================================

	

	output.Pos = float4(input.Pos, 1);
	//output.Pos = mul(output.Pos, tes);
	output.Tex = input.Tex;

    output.Normal = input.Normal;
	//output.mulMatrix = tes;
	return output;
}