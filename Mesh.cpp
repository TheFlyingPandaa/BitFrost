#include "Mesh.h"

using namespace DirectX;


void Mesh::loadMesh(const wchar_t * fileName, const bool normalIn)
{
	wchar_t inputBuffer[64];

	std::vector<Coords*> vertex;
	std::vector<Coords*> normals;
	std::vector<TexCoord*> texCoord;

	vertex = std::vector<Coords*>();
	normals = std::vector<Coords*>();
	texCoord = std::vector<TexCoord*>();

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
			vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
			swscanf(coord[i]->c_str(), L"%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't') {
			swscanf(coord[i]->c_str(), L"%*s %f %f", &tmpX, &tmpY);
			texCoord.push_back(new TexCoord(tmpX, tmpY));
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
		this->objects[b]->mat = new Material(this->mtlLib, this->objects[b]->materialname);

		this->objects[b]->mesh = new Vertex[this->objects[b]->faces.size() * 6];
		this->objects[b]->meshData = new meshdata[this->objects[b]->faces.size() * 6];
		this->objects[b]->nrOfVertexes = 0;
		this->objects[b]->nrOfMeshData = 0;
		if (normalIn == false)
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[j] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[(j + 2) % 4] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
			}
		}
		else
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[j] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[(j + 2) % 4] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
			}
		}
	}
	for (int i = 0; i < this->getNrOfObjects(); i++)
	{
		if (this->objects[i]->mat->getMtl()->normal != L"") {
			this->computeTangent(this->objects[i], vertex);
		}
	}
	for (int b = 0; b < this->objects.size(); b++)
	{
		for (int i = 0; i < this->objects[b]->faces.size(); i++)
			delete this->objects[b]->faces[i];
	}
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];
	for (int i = 0; i < texCoord.size(); i++)
		delete texCoord[i];
	
}

void Mesh::loadMesh(const char * fileName, const bool normalIn)
{
	wchar_t inputBuffer[64];

	std::vector<Coords*> vertex;
	std::vector<Coords*> normals;
	std::vector<TexCoord*> texCoord;

	vertex = std::vector<Coords*>();
	normals = std::vector<Coords*>();
	texCoord = std::vector<TexCoord*>();

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
			vertex.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 'n') {
			swscanf(coord[i]->c_str(), L"%*s %f %f %f", &tmpX, &tmpY, &tmpZ);
			normals.push_back(new Coords(tmpX, tmpY, tmpZ));
		}
		else if ((*coord[i])[0] == 'v' && (*coord[i])[1] == 't') {
			swscanf(coord[i]->c_str(), L"%*s %f %f", &tmpX, &tmpY);
			texCoord.push_back(new TexCoord(tmpX, tmpY));
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
		this->objects[b]->mat = new Material(this->mtlLib, this->objects[b]->materialname);

		this->objects[b]->mesh = new Vertex[this->objects[b]->faces.size() * 6];
		this->objects[b]->meshData = new meshdata[this->objects[b]->faces.size() * 6];
		this->objects[b]->nrOfVertexes = 0;
		this->objects[b]->nrOfMeshData = 0;
		if (normalIn == false)
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[j] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[(j + 2) % 4] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
			}
		}
		else
		{
			for (int i = 0; i < this->objects[b]->faces.size(); i++)
			{
				for (int j = 0; j < 3; j++) {
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[j] - 1]->z,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[j] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[j] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[j] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
				for (int j = 0; j < 3; j++)
				{
					this->objects[b]->mesh[this->objects[b]->nrOfVertexes++] = Vertex(vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->z,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->y,
						vertex[this->objects[b]->faces[i]->face[(j + 2) % 4] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->x,
						texCoord[this->objects[b]->faces[i]->texCord[(j + 2) % 4] - 1]->y);
					this->objects[b]->meshData[this->objects[b]->nrOfMeshData++] = meshdata(this->objects[b]->faces[i]->face[(j + 2) % 4] - 1, this->objects[b]->mesh[this->objects[b]->nrOfVertexes]);
				}
			}
		}
	}
	for (int i = 0; i < this->getNrOfObjects(); i++)
	{
		if (this->objects[i]->mat->getMtl()->normal != L"") {
			this->computeTangent(this->objects[i], vertex);
		}
	}
	for (int b = 0; b < this->objects.size(); b++)
	{
		for (int i = 0; i < this->objects[b]->faces.size(); i++)
			delete this->objects[b]->faces[i];
	}
	for (int i = 0; i < normals.size(); i++)
		delete normals[i];
	for (int i = 0; i < vertex.size(); i++)
		delete vertex[i];
	for (int i = 0; i < texCoord.size(); i++)
		delete texCoord[i];
}

void Mesh::computeTangent(object *& object, const std::vector<Coords*> & vertexes)
{
	object->tangents = new Tangents[object->nrOfVertexes];

	std::vector<XMFLOAT3> tmpNormal;
	XMFLOAT3 unnormalized = XMFLOAT3(0, 0, 0);

	std::vector<XMFLOAT3> tmpTangent;
	XMFLOAT3 tangent = XMFLOAT3(0, 0, 0);
	float tcu1, tcv1, tcu2, tcv2;

	float vecX, vecY, vecZ;

	XMVECTOR edge1 = XMVectorSet(0, 0, 0, 0);
	XMVECTOR edge2 = XMVectorSet(0, 0, 0, 0);

	std::cout << object->nrOfVertexes;

	for (int i = 0; i < object->nrOfVertexes; i += 3)
	{
		vecX = object->mesh[i].x - object->mesh[i + 2].x;
		vecY = object->mesh[i].y - object->mesh[i + 2].y;
		vecZ = object->mesh[i].z - object->mesh[i + 2].z;
		edge1 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

		vecX = object->mesh[i].x - object->mesh[i + 1].x;
		vecY = object->mesh[i].y - object->mesh[i + 1].y;
		vecZ = object->mesh[i].z - object->mesh[i + 1].z;
		edge2 = XMVectorSet(vecX, vecY, vecZ, 0.0f);

		XMStoreFloat3(&unnormalized, XMVector3Cross(edge1, edge2));

		tmpNormal.push_back(unnormalized);

		tcu1 = object->mesh[i].u - object->mesh[i + 2].u;
		tcv1 = object->mesh[i].v - object->mesh[i + 2].v;

		tcu2 = object->mesh[i + 2].u - object->mesh[i + 1].u;
		tcv2 = object->mesh[i + 2].v - object->mesh[i + 1].v;

		tangent.x = (tcv1 * XMVectorGetX(edge1) - tcv2 * XMVectorGetX(edge2)) * (1.0f / (tcu1 * tcv2 - tcu2 * tcv1));
		tangent.y = (tcv1 * XMVectorGetY(edge1) - tcv2 * XMVectorGetY(edge2)) * (1.0f / (tcu1 * tcv2 - tcu2 * tcv1));
		tangent.z = (tcv1 * XMVectorGetZ(edge1) - tcv2 * XMVectorGetZ(edge2)) * (1.0f / (tcu1 * tcv2 - tcu2 * tcv1));
		
		tmpTangent.push_back(tangent);

	}
	XMVECTOR normalSum = XMVectorSet(0, 0, 0, 0);
	XMVECTOR tangentSum = XMVectorSet(0, 0, 0, 0);
	int faceUsing = 0;
	float tx, ty, tz;

	for (int i = 0; i < object->nrOfMeshData; i++)
	{
		for (int j = 0; j < object->faces.size(); j++)
		{
			if (object->faces[j]->face[0] == object->meshData[i].index ||
				object->faces[j]->face[1] == object->meshData[i].index ||
				object->faces[j]->face[2] == object->meshData[i].index ||
				object->faces[j]->face[3] == object->meshData[i].index)
			{
				tx = XMVectorGetX(normalSum) + tmpNormal[j].x;
				ty = XMVectorGetY(normalSum) + tmpNormal[j].y;
				tz = XMVectorGetZ(normalSum) + tmpNormal[j].z;

				normalSum = XMVectorSet(tx, ty, tz, 0.0f);

				tx = XMVectorGetX(tangentSum) + tmpTangent[j].x;
				ty = XMVectorGetY(tangentSum) + tmpTangent[j].y;
				tz = XMVectorGetZ(tangentSum) + tmpTangent[j].z;
				tangentSum = XMVectorSet(tx, ty, tz, 0.0f);

				faceUsing++;
			}
		}

		normalSum = normalSum / faceUsing;		
		tangentSum = tangentSum / faceUsing;

		normalSum = XMVector3Normalize(normalSum);
		tangentSum = XMVector3Normalize(tangentSum);

		object->tangents[i].tx = XMVectorGetX(tangentSum);
		object->tangents[i].ty = XMVectorGetY(tangentSum);
		object->tangents[i].tz = XMVectorGetZ(tangentSum);

		normalSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);		
		tangentSum = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		
		faceUsing = 0;

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

std::vector<object*> Mesh::getObjects()
{
	return this->objects;
}

int Mesh::getNrOfObjects() const
{
	return this->objects.size();
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
		delete this->objects[i]->mat;
		if (this->objects[i]->tangents != nullptr)
			delete this->objects[i]->tangents;
		delete[] this->objects[i]->meshData;
		delete[] this->objects[i]->mesh;
		delete this->objects[i];
	}
}
