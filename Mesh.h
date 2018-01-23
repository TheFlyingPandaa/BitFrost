#ifndef MESH_H
#define MESH_H

struct Coords
{
	float x, y, z;
	Coords(float a, float b, float c) : x(a), y(b), z(c) {};
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


#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <cstdio>

class Mesh
{
private:


	std::string fileName;	

	//std::vector<std::string*> coord;
	std::vector<Coords*> vertex;
	std::vector<Faces*> faces;
	std::vector<Coords*> normals;

public:
	Mesh();
	Mesh(const char* fileName);
	~Mesh();
	void loadMesh(const char* fileName);

	std::vector<Coords*> getVertex() const;
	std::vector<Faces*> getFaces() const;
	std::vector<Coords*> getNormals() const;

	

};





#endif // !Mesh.h
