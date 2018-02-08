
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

VS_OUT main(VS_IN input, uint id : SV_VertexID)
{
    VS_OUT output = (VS_OUT) 0;    
 
    if (id == 0)
    {
        output.Pos = float4(-1, -1, 0, 1);
        output.Tex = float2(0, 1);
    }
    if (id == 1)
    {
        output.Pos = float4(-1, 1, 0, 1);
        output.Tex = float2(0, 0);
    }
    if (id == 2)
    {
        output.Pos = float4(1, -1, 0, 1);
        output.Tex = float2(1, 1);
    }
    if (id == 3)
    {
        output.Pos = float4(1, 1, 0, 1);
        output.Tex = float2(1, 0);
    }

	return output;
}