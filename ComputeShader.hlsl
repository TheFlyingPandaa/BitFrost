cbuffer EXAMPLE_BUFFER : register(b0)
{
	float val;
	float val1;
	float val2;
	float val3;
};

struct Pixel
{
	int colour;
};

RWStructuredBuffer<Pixel> BufferOut : register(u0);

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
	for (int i = 0; i < 5; i++)
	{
		BufferOut[i].colour = val + 1;
	}
	
}