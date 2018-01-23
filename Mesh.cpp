#include "Mesh.h"



void Mesh::loadMesh(const char * fileName)
{
	std::vector<std::string*> coord;
	std::ifstream in(fileName);

	if (!in.is_open())
		return;	//Insert real error lul

	char buff[256];
	while (!in.eof())
	{
		in.getline(buff, 256);
		coord.push_back(new std::string(buff));
	}

	float tmpX, tmpY, tmpZ;
	for (int i = 0; i < coord.size(); i++)
	{
		if ((*coord[i])[0] == '#')
		{ }
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {			
			sscanf(coord[i]->c_str(), "v %f %f %f", &tmpX, &tmpY, &tmpZ);
			vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {			
			sscanf(coord[i]->c_str(), "v %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'f')
		{
			int a, b, c, d, e;
			if (std::count(coord[i]->begin(), coord[i]->end(), ' ') == 4) {
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b, &e, &b);
				faces.push_back(new Faces(b, a, c, d, e));
			}
			else
			{
				sscanf(coord[i]->c_str(), "f %d//%d %d//%d %d//%d", &a, &b, &c, &b, &d, &b);
				faces.push_back(new Faces(b, a, c, d));
			}
		}
	}
	for (int i = 0; i < coord.size(); i++)
		delete coord[i];

	
}

std::vector<Coords*> Mesh::getVertex() const
{
	return this->vertex;
}

std::vector<Faces*> Mesh::getFaces() const
{
	return this->faces;
}

std::vector<Coords*> Mesh::getNormals() const
{
	return this->normals;
}



Mesh::Mesh()
{
}

Mesh::Mesh(const char * fileName)
{
	this->fileName = fileName;
	loadMesh(fileName);
}


Mesh::~Mesh()
{	
	for (int i = 0; i < faces.size(); i++)
		delete faces[i];
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];
}
