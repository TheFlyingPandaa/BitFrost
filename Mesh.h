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
		quad = true;
	};
};

struct Vertex {
	float x, y, z;
	float u, v;
	Vertex() {}
	Vertex(float a, float b, float c, float d, float e) : x(a), y(b), z(c), u(d), v(e) {};
};

struct MatrixBuffert {

	float realy1, realy2, realy3, realy4;

	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;
};


class Mesh
{
private:


	std::string fileName;	

	//std::vector<std::string*> coord;

	ID3D11Buffer * vertexBuffer = nullptr;
	ID3D11Buffer * constantBuffer = nullptr;

	MatrixBuffert matrixBuffer;

	float posX, posY, posZ;

	DirectX::XMMATRIX view;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX world;


public:
	Mesh();
	Mesh(const char* fileName);
	~Mesh();
	void loadMesh(const char* fileName);

	std::vector<Coords*> vertex;
	std::vector<Faces*> faces;
	std::vector<Coords*> normals;

	std::vector<TexCoord*> texCoord;

	int nrOfVertexes;
	Vertex* m_vertex;
	
	std::vector<Coords*> getVertex() const;
	std::vector<Faces*> getFaces() const;
	std::vector<Coords*> getNormals() const;

	

	void loadBuffer(ID3D11Device *& device);
	void draw(ID3D11DeviceContext *& deviceContext) const;

	void setMatrix(DirectX::XMMATRIX worldSpace, DirectX::XMMATRIX wvp, XMMATRIX view, XMMATRIX proj);

	

	//void setMatrix(DirectX::XMMATRIX worldSpace, DirectX::XMMATRIX wvp);
};





#endif // !Mesh.h
