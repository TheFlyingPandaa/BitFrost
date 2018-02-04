cbuffer EXAMPLE_BUFFER : register(b0)
{
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
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD;
    //float3 Normal : NORMAL;
};

[maxvertexcount(6)]
void GS_main(triangle GS_IN input[3]  ,inout TriangleStream< GSOutput > output)
{


	GSOutput element = (GSOutput)0;
	for (uint i = 0; i < 3; i++)
	{
		
		element.pos = input[i].Pos;
		element.Normal = normalize(cross(input[0].Pos - input[1].Pos, input[0].Pos - input[2].Pos));
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