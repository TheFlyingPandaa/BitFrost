#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include "Structs.h"

class HeightMap
{
public:
	HeightMap();
	~HeightMap();

	bool LoadHeightMap(char* filename, HeightMapInfo &hminfo);

private:
};

#endif

