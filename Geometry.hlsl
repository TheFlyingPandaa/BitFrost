cbuffer EXAMPLE_BUFFER : register(b0)
{
	float4x4 WVP;
    float4x4 worldSpace;
};

cbuffer CAMERA_BUFFERT : register(b2)
{
    float4 cameraPosition;
};

struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 worldPos : PUSS;
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
    float4 p0 = input[0].Pos;
    float4 p1 = input[1].Pos;
    float4 p2 = input[2].Pos;
    
    float3 c1 = mul(p0, worldSpace).xyz;
    float3 c2 = mul(p1, worldSpace).xyz;
    float3 c3 = mul(p2, worldSpace).xyz;

    float3 centre = (c1 * c1 * c2) / 3;
    
    //normal = float3(1, 1, 1);
    float3 a1 = mul(p1 - p0, worldSpace).xyz;
    float3 a2 = mul(p2 - p0, worldSpace).xyz;
    float3 normal = normalize(cross(a1, a2));

    //float3 cameraPos = mul(cameraPosition, worldSpace);

    float dott = dot(normalize(cameraPosition.xyz), normal);
    

    if (dott - 0.7 < 0){
    
        GSOutput element = (GSOutput) 0;
        for (uint i = 0; i < 3; i++)
        {
		
            element.pos = input[i].Pos;
            element.Tex = input[i].Tex;
			element.worldPos = mul(element.pos, worldSpace);
            element.pos = mul(element.pos, WVP);
            element.Normal = normal;
            //element.Normal = mul(element.Normal, worldSpace);

            output.Append(element);
		
        }
        output.RestartStrip();
    }
}