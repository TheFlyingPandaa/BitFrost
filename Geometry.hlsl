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

	float4x4 WVP;
    float4x4 worldSpace;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

struct GS_IN
{
	float4 Pos : SV_POSITION;
	float2 Tex : TEXCOORD;
    float3 Normal : NORMAL;
};

[maxvertexcount(6)]
void GS_main(triangle GS_IN input[3]  ,inout TriangleStream< GSOutput > output)
{

	GSOutput element = (GSOutput)0;
	for (uint i = 0; i < 3; i++)
	{
		
		element.pos = input[i].Pos;
        element.Normal = input[i].Normal;
		element.Tex = input[i].Tex;

		element.pos = mul(element.pos, WVP);
        element.Normal = mul(element.Normal, worldSpace);

		output.Append(element);
	}
	output.RestartStrip();

	/*float3 a1 = normal.pos.y - normal.pos.x;
	float3 a2 = normal.pos.z - normal.pos.x;
	float3 normm = cross(a1, a2);
	float4 norm = (norm, 1);*/

	/*for (uint i = 0; i < 3; i++)
	{

		element.pos = input[i].Pos;
		//element.pos.y -= 1;
		//element.pos.x -= 1;
		element.pos.z -= 1;
		element.Tex = input[i].Tex;
        element.Normal = input[i].Normal;
       // element.Normal.z -= 1;
		element.pos = mul(element.pos, WVP);
        element.Normal = mul(element.Normal, worldSpace);
		// element.pos = mul(element.pos, tes);
		output.Append(element);
	}*/
	
}