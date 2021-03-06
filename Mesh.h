#ifndef MESH_H
#define MESH_H
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

#include <chrono>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <iostream>
#include <DirectXMath.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "Texture.h"

#include "Matrial.h"

using namespace DirectX;

struct Coords
{
	float x, y, z;
	Coords(float a, float b, float c) : x(a), y(b), z(c) {};
};

struct TexCoord {
	float x, y;
	TexCoord(float a, float b) : x(a), y(b) {};
};

struct Faces
{
	int facenum;
	bool quad;
	int face[4];
	int texCord[4];
	Faces(int face, int f1, int f2, int f3) : facenum(face) {
		this->face[0] = f1;
		this->face[1] = f2;
		this->face[2] = f3;
		quad = false;
	};
	Faces(int face, int f1, int f2, int f3, int f4) : facenum(face) {
		this->face[0] = f1;
		this->face[1] = f2;
		this->face[2] = f3;
		this->face[3] = f4;

		this->texCord[0] = 0;
		this->texCord[1] = 0;
		this->texCord[2] = 0;
		this->texCord[3] = 0;
		quad = true;
	};
};

struct Vertex {
	float x, y, z;
	float u, v;
	
	Vertex() {}
	Vertex(float a, float b, float c, float d, float e) : x(a), y(b), z(c), u(d), v(e) {};
};

struct Tangents {
	float tx, ty, tz;
};

struct meshdata {
	int index;
	Vertex vert;	
	meshdata() {};
	meshdata(int i, Vertex v) : index(i), vert(v) {};
};

struct object {
	std::wstring objectName;
	std::wstring materialname;

	std::vector<Faces*> faces;
	
	Tangents * tangents;

	int nrOfVertexes;
	Vertex * mesh;
	Material* mat;

	int nrOfMeshData;
	meshdata * meshData;

	object(std::wstring objectName) : objectName(objectName) {
		tangents = nullptr;
		faces = std::vector<Faces*>();
		
	};
};

class Mesh
{
private:
	std::wstring fileName;
	std::string fileNameS;	

	std::vector<object*> objects;

	std::wstring mtlLib;


	void loadMesh(const wchar_t* fileName, const bool normalIn);
	void loadMesh(const char * fileName, const bool normalIn);

	void computeTangent(object *& object, const std::vector<Coords*>& vertexes);

public:
	Mesh();
	Mesh(const wchar_t* fileName, const bool normalIn);
	Mesh(const char * fileName, const bool normalIn);
	Mesh(const wchar_t* fileName, const char * textureName, const bool normalIn);
	~Mesh();	

	int getNrOfVertexes() const;
	Vertex * GetMesh();

	std::vector<object*> getObjects();
	int getNrOfObjects() const;
};
#endif // !Mesh.h
