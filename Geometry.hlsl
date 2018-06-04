cbuffer EXAMPLE_BUFFER : register(b0)
{
	float4x4 WVP;
    float4x4 worldSpace;
};

cbuffer CAMERA_BUFFERT : register(b4)
{
    float4 cameraPosition;
    float4 cameraDirection;
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

    float4 p0 = mul(input[0].Pos, worldSpace);
    float4 p1 = mul(input[1].Pos, worldSpace);
    float4 p2 = mul(input[2].Pos, worldSpace);

    float3 v0 = (p1 - p0).xyz;
    float3 v1 = (p2 - p0).xyz;
    float3 n = normalize(cross(v0, v1));

    float4 pointToCamera = cameraPosition - p0;


    if (dot(pointToCamera.xyz, n) >= 0)    //<--- Backface culling 
    {
        GSOutput element = (GSOutput) 0;
        for (uint i = 0; i < 3; i++)
        {
            element.Tex = input[i].Tex;
            element.worldPos = mul(input[i].Pos, worldSpace);
            element.pos = mul(input[i].Pos, WVP);
            element.Normal = n;

            output.Append(element);
        }
        output.RestartStrip();
    }
}