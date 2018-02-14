#include "Mesh.h"

using namespace DirectX;


void Mesh::loadMesh(const wchar_t * fileName, const bool normalIn)
{
	wchar_t inputBuffer[64];

	std::vector<std::wstring*> coord;
	std::wifstream in(fileName);

	if (!in.is_open())
		return;

	wchar_t buff[256];
	while (!in.eof())
	{
		in.getline(buff, 256);
		coord.push_back(new std::wstring(buff));
	}

	object * obj = nullptr;

	float tmpX, tmpY, tmpZ;
	for (int i = 0; i < coord.size(); i++)
	{
		if ((*coord[i])[0] == '#') {}
		else if (coord[i]->substr(0, coord[i]->find(' ')) == L"o") {
			swscanf(coord[i]->c_str(), L"%*s %s", &inputBuffer);
			if (obj != nullptr)
				this->objects.push_back(obj);
			obj = new object(inputBuffer);
		}
		else if (coord[i]->substr(0, coord[i]->find(' ')) == L"mtllib") {
			swscanf(coord[i]->c_str(), L"%*s %s", &inputBuffer);
			this->mtlLib = std::wstring(inputBuffer);
		}
		else if (coord[i]->substr(0, coord[i]->find(' ')) == L"usemtl") {
			swscanf(coord[i]->c_str(), L"%*s %s", &inputBuffer);
			obj->materialname = std::wstring(inputBuffer);
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {
			swscanf(coord[i]->c_str(), L"%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			obj->vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
			swscanf(coord[i]->c_str(), L"%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			obj->normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't') {
			swscanf(coord[i]->c_str(), L"%*s %f %f", &tmpX, &tmpY);
			obj->texCoord.push_back(new TexCoord(tmpX, tmpY));
		}
		else if ((*coord[i])[0] == 'f')
		{
			int f[12];
			if (std::count(coord[i]->begin(), coord[i]->end(), ' ') == 4 && false) {
				swscanf(coord[i]->c_str(), L"%*s %d//%d %d//%d %d//%d %d//%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7]);
				obj->faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
			else if (std::count(coord[i]->begin(), coord[i]->end(), '%d') == 12 || true) {
				swscanf(coord[i]->c_str(), L"%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7], &f[8], &f[9], &f[10], &f[11]);
				Faces * vf = new Faces(f[2], f[0], f[3], f[6], f[9]);
				vf->texCord[0] = f[1];
				vf->texCord[1] = f[4];
				vf->texCord[2] = f[7];
				vf->texCord[3] = f[10];
				obj->faces.push_back(vf);
			}
			else
			{
				swscanf(coord[i]->c_str(), L"f %d %d %d %d %d %d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
				obj->faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
		}
	}
	this->objects.push_back(obj);

	for (int i = 0; i < coord.size(); i++)
		delete coord[i];

	for (int b = 0; b < this->objects.size(); b++)
	{
		this->objects[b]->mesh = new Vertex[this->objects[b]->faces.size() * 6];
		this->objects[b]->nrOfVertexes = 0;
		if (normalIn == false)
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
				}
			}
		}
		else
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
				}
			}
		}
	}
	for (int b = 0; b < this->objects.size(); b++)
	{
		for (int i = 0; i < this->objects[b]->faces.size(); i++)
			delete this->objects[b]->faces[i];
		for (int i = 0; i < this->objects[b]->normals.size(); i++)
			delete this->objects[b]->normals[i];
		for (int i = 0; i < this->objects[b]->vertex.size(); i++)
			delete this->objects[b]->vertex[i];
		for (int i = 0; i < this->objects[b]->texCoord.size(); i++)
			delete this->objects[b]->texCoord[i];
	}
}

void Mesh::loadMesh(const char * fileName, const bool normalIn)
{
	wchar_t inputBuffer[64];

	std::vector<std::wstring*> coord;
	std::wifstream in(fileName);


	if (!in.is_open())
		return;

	wchar_t buff[256];
	while (!in.eof())
	{
		in.getline(buff, 256);
		coord.push_back(new std::wstring(buff));
	}

	object * obj = nullptr;

	float tmpX, tmpY, tmpZ;
	for (int i = 0; i < coord.size(); i++)
	{
		if ((*coord[i])[0] == '#') {}
		else if (coord[i]->substr(0, coord[i]->find(' ')) == L"o") {
			swscanf(coord[i]->c_str(), L"%*s %s", &inputBuffer);
			if (obj != nullptr)
				this->objects.push_back(obj);
			obj = new object(inputBuffer);
		}	
		else if (coord[i]->substr(0, coord[i]->find(' ')) == L"mtllib") {
			swscanf(coord[i]->c_str(), L"%*s %s", &inputBuffer);
			this->mtlLib = std::wstring(inputBuffer);			
		}
		else if (coord[i]->substr(0, coord[i]->find(' ')) == L"usemtl") {
			swscanf(coord[i]->c_str(), L"%*s %s", &inputBuffer);
			obj->materialname = std::wstring(inputBuffer);
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {
			swscanf(coord[i]->c_str(), L"%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			obj->vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
			swscanf(coord[i]->c_str(), L"%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			obj->normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't') {
			swscanf(coord[i]->c_str(), L"%*s %f %f", &tmpX, &tmpY);
			obj->texCoord.push_back(new TexCoord(tmpX, tmpY));
		}
		else if ((*coord[i])[0] == 'f')
		{
			int f[12];
			if (std::count(coord[i]->begin(), coord[i]->end(), ' ') == 4 && false) {
				swscanf(coord[i]->c_str(), L"%*s %d//%d %d//%d %d//%d %d//%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7]);
				obj->faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
			else if (std::count(coord[i]->begin(), coord[i]->end(), '%d') == 12 || true) {
				swscanf(coord[i]->c_str(), L"%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7], &f[8], &f[9], &f[10], &f[11]);
				Faces * vf = new Faces(f[2], f[0], f[3], f[6], f[9]);
				vf->texCord[0] = f[1];
				vf->texCord[1] = f[4];
				vf->texCord[2] = f[7];
				vf->texCord[3] = f[10];
				obj->faces.push_back(vf);
			}
			else
			{
				swscanf(coord[i]->c_str(), L"f %d %d %d %d %d %d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
				obj->faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
		}
	}	
	this->objects.push_back(obj);

	for (int i = 0; i < coord.size(); i++)
		delete coord[i];

	for (int b = 0; b < this->objects.size(); b++)
	{
		this->objects[b]->mesh = new Vertex[this->objects[b]->faces.size() * 6];
		this->objects[b]->nrOfVertexes = 0;
		if (normalIn == false)
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
				}
			}
		}
		else
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						this->objects[b]->vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						this->objects[b]->texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
				}
			}
		}
	}
	for (int b = 0; b < this->objects.size(); b++)
	{
		for (int i = 0; i < this->objects[b]->faces.size(); i++)
			delete this->objects[b]->faces[i];
		for (int i = 0; i < this->objects[b]->normals.size(); i++)
			delete this->objects[b]->normals[i];
		for (int i = 0; i < this->objects[b]->vertex.size(); i++)
			delete this->objects[b]->vertex[i];
		for (int i = 0; i < this->objects[b]->texCoord.size(); i++)
			delete this->objects[b]->texCoord[i];
	}
}

int Mesh::getNrOfVertexes() const
{
	return this->objects[0]->nrOfVertexes;
}

Vertex * Mesh::GetMesh()
{
	return this->objects[0]->mesh;
}

Mesh::Mesh()
{
}


Mesh::Mesh(const wchar_t * fileName, const bool normalIn)
{
	this->fileName = fileName;
	this->objects = std::vector<object*>();
	loadMesh(fileName,normalIn);	
}

Mesh::Mesh(const char * fileName, const bool normalIn)
{
	this->fileNameS = fileName;
	this->objects = std::vector<object*>();
	loadMesh(fileName, normalIn);
}

Mesh::Mesh(const wchar_t * fileName, const char * textureName, const bool normalIn)
{
	this->fileName = fileName;
	this->objects = std::vector<object*>();
	loadMesh(fileName, normalIn);	
}

Mesh::~Mesh()
{	
	for (size_t i = 0; i < this->objects.size(); i++)
	{
		delete[] this->objects[i]->mesh;
	}
}
