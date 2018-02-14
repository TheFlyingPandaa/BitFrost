cbuffer EXAMPLE_BUFFER : register(b0)
{
    float2 output;
    float2 camPos;
    float2 objectPos;
    float2 padding;     //NEED PADDING CUZ 16 * n
};

struct computeShader
{
    float2 output;
    float2 camPos;
    float2 objectPos;
    float2 padding;
};

RWStructuredBuffer<computeShader> BufferOut : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    float angle = atan2(objectPos.x - camPos.x, objectPos.y - camPos.y) * (180.0 / 3.14159265359f);
	// Convert rotation into radians.
    float rotInRad = (float) angle * 0.0174532925f;
	
    BufferOut[DTid.x].output.x = rotInRad;
}