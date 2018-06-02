cbuffer EXAMPLE_BUFFER : register(b0)
{
    float4      cameraPos;
    float4      objectPosition[100]; 
};

struct computeShader
{
    float3x3 rotationMatrix;
};

RWStructuredBuffer<computeShader> BufferOut : register(u0);

[numthreads(100, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    float3 dir = normalize(objectPosition[DTid.x].xyz - cameraPos.xyz);
    float3 right = normalize(cross(dir.xyz, float3(0, 1, 0)));
    float3 up =     normalize(cross(dir.xyz, right.xyz));
    
    BufferOut[DTid.x].rotationMatrix = float3x3(right, up, dir);
}