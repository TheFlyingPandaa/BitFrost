// Input control point
struct VS_CONTROL_POINT_OUTPUT
{
    float4 Pos : POSITION;
    float2 Tex : TEXCOORD;
	// TODO: change/add other stuff
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
    float EdgeTessFactor[3] : SV_TessFactor; // e.g. would be [4] for a quad domain
    float InsideTessFactor : SV_InsideTessFactor; // e.g. would be Inside[2] for a quad domain
};

#define NUM_CONTROL_POINTS 3

// Patch Constant Function
HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONSTANT_DATA_OUTPUT Output;

    float tessellationFactor = 1;
	// Insert code to compute Output here
    Output.EdgeTessFactor[0] = tessellationFactor;
    Output.EdgeTessFactor[1] = tessellationFactor;
    Output.EdgeTessFactor[2] = tessellationFactor;
    Output.InsideTessFactor = tessellationFactor; // e.g. could calculate dynamic tessellation factors instead

    return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main(
	InputPatch<VS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patchInput,
	uint i : SV_OutputControlPointID,
	uint PatchID : SV_PrimitiveID)
{
    HS_CONTROL_POINT_OUTPUT Output;

	// Insert code to compute Output here
    Output.Pos = patchInput[i].Pos;
    Output.Tex = patchInput[i].Tex;


    return Output;
}
