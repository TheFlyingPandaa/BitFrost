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
    //float4 p0 = input[0].Pos;
    //float4 p1 = input[1].Pos;
    //float4 p2 = input[2].Pos;    
    
    //float3 a1 = mul(p0 - p1, worldSpace).xyz;
    //float3 a2 = mul(p2 - p1, worldSpace).xyz;
    //float3 normal = normalize(cross(a2, a1));

    float4 p0 = mul(input[0].Pos, worldSpace);
    float4 p1 = mul(input[1].Pos, worldSpace);
    float4 p2 = mul(input[2].Pos, worldSpace);

    float3 v0 = (p1 - p0).xyz;
    float3 v1 = (p2 - p0).xyz;
    float3 n = normalize(cross(v0, v1));

    //float4 vP0 = mul(p0, viewMatrix);
    //float4 vP1 = mul(p1, viewMatrix);
    //float4 vP2 = mul(p2, viewMatrix);

    //float3 vV0 = (vP1 - vP0).xyz;
    //float3 vV1 = (vP2 - vP0).xyz;

    //float3 vN = normalize(cross(vV1, vV0));

    //float3 cameraPos = mul(cameraPosition, worldSpace);

    float dott = dot(normalize(cameraDirection.xyz), n);
    

    if (dott - 0.7 < 0){
    
        GSOutput element = (GSOutput) 0;
        for (uint i = 0; i < 3; i++)
        {
		
            //element.pos = input[i].Pos;
            element.Tex = input[i].Tex;
            element.worldPos = mul(input[i].Pos, worldSpace);
            element.pos = mul(input[i].Pos, WVP);
            element.Normal = n;
            //element.Normal = mul(element.Normal, worldSpace);

            output.Append(element);
		
        }
        
        output.RestartStrip();
    }
}