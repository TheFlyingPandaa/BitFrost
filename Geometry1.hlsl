/*struct GS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};

[maxvertexcount(3)]
GS_OUT GS_main(triangle float4 input[3] : SV_POSITION, float3 color[3] : COLOR ,inout TriangleStream< GS_OUT > output)
{
	//GS_OUT output = (GS_OUT)0;

	//output.Pos = input.Pos;
	//output.Color = input.Color;

	for (uint i = 0; i < 3; i++)
	{
		GS_OUT element;
		element.Pos = input[i];
		output.Append(element);
	}

	return output;
}*/

cbuffer EXAMPLE_BUFFER : register(b0)
{
	float value1;
	float value2;
	float value3;
	float value4;

	float4x4 tes;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
};

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3]  ,inout TriangleStream< GSOutput > output)
{

	GSOutput element = (GSOutput)0;
	for (uint i = 0; i < 3; i++)
	{
		
		element.pos = input[i].Pos;
		element.color = input[i].Color;

		element.pos = mul(element.pos, tes);

		output.Append(element);
	}
	
	
}