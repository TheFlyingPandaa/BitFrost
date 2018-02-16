#include "RenderObject.h"

#include <iostream>

void RenderObject::loadBuffer(ID3D11Device *& device)
{
	for (size_t i = 0; i < this->mesh->getNrOfObjects(); i++)
	{
		D3D11_BUFFER_DESC bufferDesc;
		memset(&bufferDesc, 0, sizeof(bufferDesc));
		bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;
		bufferDesc.ByteWidth = sizeof(Vertex) * this->mesh->getObjects()[i]->nrOfVertexes;

		D3D11_SUBRESOURCE_DATA vertexData;
		vertexData.pSysMem = this->mesh->getObjects()[i]->mesh;
		HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexData, &vertexBuffer[i]);

		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cBufferDesc.ByteWidth = sizeof(MatrixBuffert);
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;

		hr = device->CreateBuffer(&cBufferDesc, nullptr, &constantBuffer);

		cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		cBufferDesc.ByteWidth = sizeof(textureInformationBuffer);
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		hr = 0;
		hr = device->CreateBuffer(&cBufferDesc, nullptr, &psConstantBuffer);
	}

	for (size_t i = 0; i < this->mesh->getNrOfObjects(); i++)
	{
		if (this->mesh->getObjects()[i]->mat->getMtl()->textureName != L"")
			this->tex[i]->createTexture(device, this->mesh->getObjects()[i]->mat->getMtl()->textureName.c_str());
		if (this->mesh->getObjects()[i]->mat->getMtl()->normal != L"") {
			this->normal[i]->createTexture(device, this->mesh->getObjects()[i]->mat->getMtl()->normal.c_str());
			std::wcout << this->mesh->getObjects()[i]->mat->getMtl()->normal << std::endl;
		}
	}
}

void RenderObject::draw(ID3D11DeviceContext *& deviceContext)
{
	UINT32 vertexSize = sizeof(float) * 5;
	UINT offset = 0;
	for (int i = 0; i < this->mesh->getNrOfObjects(); i++)
	{

		deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer[i], &vertexSize, &offset);

		D3D11_MAPPED_SUBRESOURCE dataPtr;
		deviceContext->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);
		//	Copy memory from CPU to GPU
		memcpy(dataPtr.pData, &matrixBuffer, sizeof(MatrixBuffert));
		// Unmap constant buffer so that we can use it again in the GPU
		deviceContext->Unmap(constantBuffer, 0);
		// set resources to shaders


		texInfo.NORMAL = this->normal[i] != nullptr;
		texInfo.TEXTURE = this->tex[i] != nullptr;
		if (texInfo.NORMAL)
			texInfo.tangent = XMFLOAT3A(this->mesh->getObjects()[i]->tangents[i].tx, this->mesh->getObjects()[i]->tangents[i].ty, this->mesh->getObjects()[i]->tangents[i].tz);

		deviceContext->Map(psConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &dataPtr);	
		memcpy(dataPtr.pData, &texInfo, sizeof(textureInformationBuffer));		
		deviceContext->Unmap(psConstantBuffer, 0);

		deviceContext->GSSetConstantBuffers(0, 1, &constantBuffer);
		deviceContext->PSSetConstantBuffers(2, 1, &psConstantBuffer);
		deviceContext->PSSetShaderResources(0, 1, &this->tex[i]->getTexture());
		deviceContext->PSSetSamplers(0, 1, &this->tex[i]->getSampleState()); 
		if (this->normal[i] != nullptr) {
			deviceContext->PSSetShaderResources(1, 1, &this->normal[i]->getTexture());
			deviceContext->PSSetSamplers(1, 1, &this->normal[i]->getSampleState());
		}		
		deviceContext->VSSetConstantBuffers(3, 1, &constantBuffer);

		deviceContext->Draw(this->mesh->getNrOfVertexes(), 0);
	}
}

void RenderObject::setMatrix(const XMMATRIX & view, const XMMATRIX & proj, float radsRot)
{
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX, posY, posZ);
	XMMATRIX rotation;

	if (radsRot == NULL)
	{
		XMVECTOR quat = XMVECTOR();
		rotation = XMMatrixRotationRollPitchYawFromVector(quat);
	}
	else
	{
		XMVECTOR rotaxis = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		rotation = XMMatrixRotationAxis(rotaxis, radsRot);
	}

	XMMATRIX scale = XMMatrixScaling(scaleX, scaleY, scaleZ);
	XMMATRIX m_worldMatrix = rotation * scale * translate;

	matrixBuffer.worldSpace = DirectX::XMMatrixTranspose(m_worldMatrix);
	matrixBuffer.WVP = DirectX::XMMatrixTranspose(m_worldMatrix * view * proj);
}

void RenderObject::setPosition(float x, float y, float z)
{
	posX = x;
	posY = y;
	posZ = z;
}

XMFLOAT3 RenderObject::getPosition() const
{
	return XMFLOAT3(posX,posY,posZ);
}

void RenderObject::setScale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
}

XMFLOAT3 * RenderObject::getPos()
{
	if (this->position == nullptr)
		this->position = new XMFLOAT3(posX, posY, posZ);
	else {
		this->position->x = posX;
		this->position->y = posY;
		this->position->z = posZ;
	}
	return this->position;
}

RenderObject::RenderObject()
{
	this->mesh = new Mesh(L"r8.obj", false);
}

RenderObject::RenderObject(const wchar_t * meshDirr, LPCWSTR textureFile,const bool normalIn)
{
	this->mesh = new Mesh(meshDirr, normalIn);
	this->tex = new Texture*[this->mesh->getNrOfObjects()];
	this->normal = new Texture*[this->mesh->getNrOfObjects()];
	this->vertexBuffer = new ID3D11Buffer*[this->mesh->getNrOfObjects()];
	for (size_t i = 0; i < this->mesh->getObjects().size(); i++)
	{
		if (this->mesh->getObjects()[i]->mat->getMtl()->textureName != L"")
			this->tex[i] = new Texture();
		else
			this->tex[i] = nullptr;

		if (this->mesh->getObjects()[i]->mat->getMtl()->normal != L"")
			this->normal[i] = new Texture();
		else
			this->normal[i] = nullptr;
	}
	if (textureFile != NULL) {
		this->textureFile = textureFile;
	}
	else
		this->textureFile = L"Default.png";
	posX = 0;
	posY = 0;
	posZ = 0;
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
}
RenderObject::RenderObject(const char * meshDirr, LPCWSTR textureFile, const bool normalIn)
{
	this->mesh = new Mesh(meshDirr, normalIn);
	this->tex = new Texture*[this->mesh->getNrOfObjects()];
	this->normal = new Texture*[this->mesh->getNrOfObjects()];
	this->vertexBuffer = new ID3D11Buffer*[this->mesh->getNrOfObjects()];
	for (size_t i = 0; i < this->mesh->getObjects().size(); i++)
	{
		if (this->mesh->getObjects()[i]->mat->getMtl()->textureName != L"")
			this->tex[i] = new Texture();
		if (this->mesh->getObjects()[i]->mat->getMtl()->normal != L"")
			this->normal[i] = new Texture();
	}
	if (textureFile != NULL) {
		this->textureFile = textureFile;
	}
	else
		this->textureFile = L"Default.png";
	posX = 0;
	posY = 0;
	posZ = 0;
	scaleX = 1;
	scaleY = 1;
	scaleZ = 1;
}

RenderObject::~RenderObject()
{
	for (size_t i = 0; i < this->mesh->getNrOfObjects(); i++)
	{
		if (this->tex[i] != nullptr)
			delete this->tex[i];	
	}
	if (this->tex != nullptr)
		delete this->tex;

	for (size_t i = 0; i < this->mesh->getNrOfObjects(); i++)
	{
		if (this->normal[i] != nullptr)
			delete this->normal[i];
	}
	if (this->normal != nullptr)
		delete this->normal;

	if (this->position != nullptr)
		delete this->position;
	for (int i = 0; i < this->mesh->getNrOfObjects(); i++)
	{
		this->vertexBuffer[i]->Release();
	}
	delete[] this->vertexBuffer;
	this->constantBuffer->Release();
	this->psConstantBuffer->Release();

	delete mesh;
}
