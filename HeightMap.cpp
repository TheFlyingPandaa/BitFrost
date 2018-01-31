#include "HeightMap.h"
#include <DirectXMath.h>
#include <cstdio>
#include <Windows.h>
HeightMap::HeightMap()
{
}

HeightMap::~HeightMap()
{
}

bool HeightMap::LoadHeightMap(char* filename, HeightMapInfo &hminfo)
{
	FILE *filePtr;                            // Point to the current position in the file
	BITMAPFILEHEADER bitmapFileHeader;        // Structure which stores information about file
	BITMAPINFOHEADER bitmapInfoHeader;        // Structure which stores information about image
	int imageSize, index;
	unsigned char height;

	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return 0;

	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	hminfo.terrainWidth = bitmapInfoHeader.biWidth;
	hminfo.terrainHeight = bitmapInfoHeader.biHeight;

	imageSize = hminfo.terrainWidth * hminfo.terrainHeight * 3;

	unsigned char* bitmapImage = new unsigned char[imageSize];
	// Set the file pointer to the beginning of the image data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);
	// Store image data in bitmapImage
	fread(bitmapImage, 1, imageSize, filePtr);
	fclose(filePtr);

	hminfo.heightMap = new DirectX::XMFLOAT3[hminfo.terrainWidth * hminfo.terrainHeight];

	int k = 0;
	float heightFactor = 10.0f; //Smothing

	for (int j = 0; j< hminfo.terrainHeight; j++)
	{
		for (int i = 0; i< hminfo.terrainWidth; i++)
		{
			height = bitmapImage[k];

			index = (hminfo.terrainHeight * j) + i;

			hminfo.heightMap[index].x = (float)i;
			hminfo.heightMap[index].y = (float)height / heightFactor;
			hminfo.heightMap[index].z = (float)j;

			k += 3;
		}
	}

	delete[] bitmapImage;
	bitmapImage = 0;

	return true;
}
