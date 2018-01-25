#include "Mesh.h"

using namespace DirectX;

void Mesh::loadMesh(const char * fileName)
{
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
				sscanf(coord[i]->c_str(), "f %d %d %d %d %d %d %d %d", &f[0], &f[1], &f[2], &f[3], &f[4], &f[5], &f[6], &f[7]);
				faces.push_back(new Faces(f[1], f[0], f[2], f[3], f[4]));
			}
			else if (std::count(coord[i]->begin(), coord[i]->end(), ' ') == 4 || true) {
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
	for (int i = 0; i < this->faces.size(); i++)
	{
		for (int j = 0; j < 3; j++) {
			m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[j] - 1]->x,
													vertex[faces[i]->face[j] - 1]->y,
													vertex[faces[i]->face[j] - 1]->z,
													texCoord[faces[i]->texCord[j] - 1]->x,
													texCoord[faces[i]->texCord[j] - 1]->y);
		}
		m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[2] - 1]->x,
												vertex[faces[i]->face[2] - 1]->y,
												vertex[faces[i]->face[2] - 1]->z,
												texCoord[faces[i]->texCord[2] - 1]->x,
												texCoord[faces[i]->texCord[2] - 1]->y);
		m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[3] - 1]->x,
												vertex[faces[i]->face[3] - 1]->y,
												vertex[faces[i]->face[3] - 1]->z,
												texCoord[faces[i]->texCord[3] - 1]->x,
												texCoord[faces[i]->texCord[3] - 1]->y);
		m_vertex[this->nrOfVertexes++] = Vertex(vertex[faces[i]->face[0] - 1]->x,
												vertex[faces[i]->face[0] - 1]->y,
												vertex[faces[i]->face[0] - 1]->z,
												texCoord[faces[i]->texCord[0] - 1]->x,
												texCoord[faces[i]->texCord[0] - 1]->y);
	}
	//231 123 321 132 312

	std::ofstream out("Debug.txt");
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
	out.close();
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

void Mesh::loadBuffer(ID3D11Device *& device)
{
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * this->nrOfVertexes;

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = m_vertex;
	HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = sizeof(MatrixBuffert);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&cBufferDesc, nullptr, &constantBuffer);
}

void Mesh::draw(ID3D11DeviceContext *& deviceContext) const
{
	UINT32 vertexSize = sizeof(float) * 5;
	UINT offset = 0;
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	
	D3D11_MAPPED_SUBRESOURCE dataPtr;
	deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);

	//	Copy memory from CPU to GPU
	memcpy(dataPtr.pData, &matrixBuffer, sizeof(MatrixBuffert));

	// Unmap constant buffer so that we can use it again in the GPU
	deviceContext->Unmap(constantBuffer, 0);
	// set resources to shaders
	
	deviceContext->GSSetConstantBuffers(0, 1, &constantBuffer);
	
	deviceContext->Draw(nrOfVertexes, 0);
}

void Mesh::setMatrix(DirectX::XMMATRIX worldSpace, DirectX::XMMATRIX wvp, XMMATRIX view, XMMATRIX proj)
{
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(1, 1, 1);
	
	XMVECTOR quat = XMVECTOR();
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(quat);


	XMMATRIX scale = XMMatrixScaling(.5f, .5f, .5f);
	XMMATRIX m_worldMatrix = rotation * scale * translate;

	matrixBuffer.worldSpace = DirectX::XMMatrixTranspose(m_worldMatrix);
	matrixBuffer.WVP = DirectX::XMMatrixTranspose(m_worldMatrix * view * proj);
}





Mesh::Mesh()
{
}

Mesh::Mesh(const char * fileName)
{
	this->fileName = fileName;
	vertex = std::vector<Coords*>();
	faces = std::vector<Faces*>();
	normals = std::vector<Coords*>();

	texCoord = std::vector<TexCoord*>();
	loadMesh(fileName);

	matrixBuffer = { 0.0f,0.0f,0.0f,0.0f };
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
