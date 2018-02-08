cbuffer EXAMPLE_BUFFER : register(b0)
{
	float4x4 WVP;
    float4x4 worldSpace;
    float4 cameraPosition;
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

[maxvertexcount(3)]
void GS_main(triangle GS_IN input[3]  ,inout TriangleStream< GSOutput > output)
{
    //float3 normal = normalize(cross(input[0].Pos.xyz - input[1].Pos.xyz, input[0].Pos.xyz - input[2].Pos.xyz));
    //float3 centre = (input[0].Pos.xyz * input[1].Pos.xyz * input[2].Pos.xyz) / 3;
    float4 p0 = input[0].Pos;
    float4 p1 = input[1].Pos;
    float4 p2 = input[2].Pos;
    /*
    float3 c1 = mul(p0, worldSpace).xyz;
    float3 c2 = mul(p1, worldSpace).xyz;
    float3 c3 = mul(p2, worldSpace).xyz;

    float3 centre = (c1 * c1 * c2) / 3;
    */
    //normal = float3(1, 1, 1);
    float3 a1 = mul(p1 - p0, worldSpace).xyz;
    float3 a2 = mul(p2 - p0, worldSpace).xyz;
    float3 normal = normalize(cross(a1, a2));

    //if (dot(cameraPosition.xyz, centre - normal) == 0)
    //{
	GSOutput element = (GSOutput)0;
	for (uint i = 0; i < 3; i++)
	{
		
        element.pos = input[i].Pos;
		element.Tex = input[i].Tex;

		element.pos = mul(element.pos, WVP);
		element.Normal = normal;
        //element.Normal = mul(element.Normal, worldSpace);

		output.Append(element);
		
	}
	output.RestartStrip();
    //}

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