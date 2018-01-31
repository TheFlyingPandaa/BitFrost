#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>

struct HeightMapInfo {
	int terrainWidth;
	int terrainHeight;
	DirectX::XMFLOAT3 *heightMap;
};


class HeightMap
{
public:

	HeightMap();
	~HeightMap();

	bool LoadHeightMap(char* filename, HeightMapInfo &hminfo);
private:


};

#endif
