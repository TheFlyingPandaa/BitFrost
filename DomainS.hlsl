struct DS_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

// Output control point
struct HS_CONTROL_POINT_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
};

// Output patch constant data.
struct HS_CONSTANT_DATA_OUTPUT
{
    float EdgeTessFactor[3] : SV_TessFactor;
    float InsideTessFactor : SV_InsideTessFactor;
};


#define NUM_CONTROL_POINTS 3

[domain("tri")]
DS_OUTPUT main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
    float3 vertexPosition; //Why like this. This is just to avoid implicit conversions.
    DS_OUTPUT Output;

    vertexPosition = patch[0].Pos * domain.x + patch[1].Pos * domain.y + patch[2].Pos * domain.z;
    Output.Pos = float4(vertexPosition, 1.0f);
    Output.Tex = patch[0].Tex * domain.x + patch[1].Tex * domain.y; // TODO: FIX TEXTURES

    return Output;
}