#include "Mesh.h"

using namespace DirectX;


void Mesh::loadMesh(const wchar_t * fileName, const bool normalIn)
{
	std::vector<Coords*> vertex;
	std::vector<Faces*> faces;
	std::vector<Coords*> normals;
	std::vector<TexCoord*> texCoord;

	vertex = std::vector<Coords*>();
	faces = std::vector<Faces*>();
	normals = std::vector<Coords*>();
	texCoord = std::vector<TexCoord*>();

	std::vector<std::string*> coord;
	std::ifstream in(fileName);

	if (!in.is_open())
		return;

	char buff[256];
	while (!in.eof())
	{
		in.getline(buff, 256);
		coord.push_back(new std::string(buff));
	}

	float tmpX, tmpY, tmpZ;
	for (int i = 0; i < coord.size(); i++)
	{
		if ((*coord[i])[0] == '#'){ }
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {			
			sscanf(coord[i]->c_str(), "%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
			
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {			
			sscanf(coord[i]->c_str(), "%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't') {
			sscanf(coord[i]->c_str(), "%*s %f %f", &tmpX, &tmpY);
			texCoord.push_back(new TexCoord(tmpX, tmpY));
		}
		else if ((*coord[i])[0] == 'f')
		{
			int f[12];
			if (std::count(coord[i]->begin(), coord[i]->end(), ' ') == 4 && false) {
				sscanf(coord[i]->c_str(), "%*s %d//%d %d//%d %d//%d %d//%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7]);
				faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
			else if (std::count(coord[i]->begin(), coord[i]->end(), '%d') == 12 || true) {
				sscanf(coord[i]->c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7], &f[8], &f[9], &f[10], &f[11]);
				Faces * vf = new Faces(f[2], f[0], f[3], f[6], f[9]);
				vf->texCord[0] = f[1];
				vf->texCord[1] = f[4];
				vf->texCord[2] = f[7];
				vf->texCord[3] = f[10];
				faces.push_back(vf);
			}
			else
			{
				sscanf(coord[i]->c_str(), "f %d %d %d %d %d %d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
				faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
		}
	}
	for (int i = 0; i < coord.size(); i++)
		delete coord[i];
		
	this->m_vertex = new Vertex[faces.size() * 6];
	this->nrOfVertexes = 0;
	if (normalIn == false)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			for (int j = 0; j < 3; j++) {
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[j] - 1]->x,
					vertex[faces[i]->face[j] - 1]->y,
					vertex[faces[i]->face[j] - 1]->z,
					texCoord[faces[i]->texCord[j] - 1]->x,
					texCoord[faces[i]->texCord[j] - 1]->y);
			}
			for (int j = 0; j < 3; j++)
			{
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[(j + 2) % 4] - 1]->x,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->y,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->z,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->x,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->y);
			}
		}
	}
	else
	{
		for (int i = 0; i < faces.size(); i++)
		{
			for (int j = 0; j < 3; j++) {
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[j] - 1]->z,
					vertex[faces[i]->face[j] - 1]->y,
					vertex[faces[i]->face[j] - 1]->x,
					texCoord[faces[i]->texCord[j] - 1]->x,
					texCoord[faces[i]->texCord[j] - 1]->y);
			}
			for (int j = 0; j < 3; j++)
			{
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[(j + 2) % 4] - 1]->z,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->y,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->x,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->x,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->y);
			}
		}
	}

	for (int i = 0; i < faces.size(); i++)
		delete faces[i];
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];
	for (int i = 0; i < texCoord.size(); i++)
		delete texCoord[i];
}

void Mesh::loadMesh(const char * fileName, const bool normalIn)
{
	std::vector<Coords*> vertex;
	std::vector<Faces*> faces;
	std::vector<Coords*> normals;
	std::vector<TexCoord*> texCoord;

	vertex = std::vector<Coords*>();
	faces = std::vector<Faces*>();
	normals = std::vector<Coords*>();
	texCoord = std::vector<TexCoord*>();

	std::vector<std::string*> coord;
	std::ifstream in(fileName);

	if (!in.is_open())
		return;

	char buff[256];
	while (!in.eof())
	{
		in.getline(buff, 256);
		coord.push_back(new std::string(buff));
	}

	float tmpX, tmpY, tmpZ;
	for (int i = 0; i < coord.size(); i++)
	{
		if ((*coord[i])[0] == '#') {}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == ' ') {
			sscanf(coord[i]->c_str(), "%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
			sscanf(coord[i]->c_str(), "%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't') {
			sscanf(coord[i]->c_str(), "%*s %f %f", &tmpX, &tmpY);
			texCoord.push_back(new TexCoord(tmpX, tmpY));
		}
		else if ((*coord[i])[0] == 'f')
		{
			int f[12];
			if (std::count(coord[i]->begin(), coord[i]->end(), ' ') == 4 && false) {
				sscanf(coord[i]->c_str(), "%*s %d//%d %d//%d %d//%d %d//%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7]);
				faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
			else if (std::count(coord[i]->begin(), coord[i]->end(), '%d') == 12 || true) {
				sscanf(coord[i]->c_str(), "%*s %d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7], &f[8], &f[9], &f[10], &f[11]);
				Faces * vf = new Faces(f[2], f[0], f[3], f[6], f[9]);
				vf->texCord[0] = f[1];
				vf->texCord[1] = f[4];
				vf->texCord[2] = f[7];
				vf->texCord[3] = f[10];
				faces.push_back(vf);
			}
			else
			{
				sscanf(coord[i]->c_str(), "f %d %d %d %d %d %d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5]);
				faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
		}
	}
	for (int i = 0; i < coord.size(); i++)
		delete coord[i];

	this->m_vertex = new Vertex[faces.size() * 6];
	this->nrOfVertexes = 0;
	if (normalIn == false)
	{
		for (int i = 0; i < faces.size(); i++)
		{
			for (int j = 0; j < 3; j++) {
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[j] - 1]->x,
					vertex[faces[i]->face[j] - 1]->y,
					vertex[faces[i]->face[j] - 1]->z,
					texCoord[faces[i]->texCord[j] - 1]->x,
					texCoord[faces[i]->texCord[j] - 1]->y);
			}
			for (int j = 0; j < 3; j++)
			{
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[(j + 2) % 4] - 1]->x,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->y,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->z,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->x,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->y);
			}
		}
	}
	else
	{
		for (int i = 0; i < faces.size(); i++)
		{
			for (int j = 0; j < 3; j++) {
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[j] - 1]->z,
					vertex[faces[i]->face[j] - 1]->y,
					vertex[faces[i]->face[j] - 1]->x,
					texCoord[faces[i]->texCord[j] - 1]->x,
					texCoord[faces[i]->texCord[j] - 1]->y);
			}
			for (int j = 0; j < 3; j++)
			{
				m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[(j + 2) % 4] - 1]->z,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->y,
					vertex[faces[i]->face[(j + 2) % 4] - 1]->x,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->x,
					texCoord[faces[i]->texCord[(j + 2) % 4] - 1]->y);
			}
		}
	}
	/*std::ofstream out("Debug.txt");
	out << "vertex " << vertex.size() << std::endl;
	for (int i = 0; i < vertex.size(); i++)
	{
	out << vertex[i]->x << " " << vertex[i]->y << " " << vertex[i]->z << std::endl;
	}
	out << "Tex " << texCoord.size() << std::endl;
	for (int i = 0; i < texCoord.size(); i++)
	{
	out << texCoord[i]->x << " " << texCoord[i]->y << std::endl;
	}
	out << "normals " << normals.size() << std::endl;
	for (int i = 0; i < normals.size(); i++)
	{
	out << normals[i]->x << " " << normals[i]->y << " " << normals[i]->z << std::endl;
	}
	out << "Face " << faces.size() << std::endl;
	for (int i = 0; i < faces.size(); i++)
	{

	if (faces[i]->quad)
	{
	for (int j = 0; j < 4; j++)
	{
	out << faces[i]->face[j] << " " << faces[i]->texCord[j] << " " << faces[i]->facenum << "/";
	}
	}
	else {
	for (int j = 0; j < 3; j++)
	{
	out << faces[i]->face[j] << " ";
	}
	}
	out << std::endl;
	}
	out << this->nrOfVertexes;
	out.close();*/

	for (int i = 0; i < faces.size(); i++)
		delete faces[i];
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];
	for (int i = 0; i < texCoord.size(); i++)
		delete texCoord[i];
}

int Mesh::getNrOfVertexes() const
{
	return this->nrOfVertexes;
}

Vertex * Mesh::GetMesh()
{
	return this->m_vertex;
}

Mesh::Mesh()
{
}


Mesh::Mesh(const wchar_t * fileName, const bool normalIn)
{
	this->fileName = fileName;
	loadMesh(fileName,normalIn);
	
}

Mesh::Mesh(const char * fileName, const bool normalIn)
{
	this->fileNameS = fileName;
	loadMesh(fileName, normalIn);

}

Mesh::Mesh(const wchar_t * fileName, const char * textureName, const bool normalIn)
{
	this->fileName = fileName;
	loadMesh(fileName, normalIn);
	
}

Mesh::~Mesh()
{	
	delete[] this->m_vertex;
}
