#include "RenderObject.h"



void RenderObject::loadBuffer(ID3D11Device *& device)
{
	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(Vertex) * this->mesh->getNrOfVertexes();

	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = this->mesh->GetMesh();
	HRESULT hr = device->CreateBuffer(&bufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	cBufferDesc.ByteWidth = sizeof(MatrixBuffert);
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;

	hr = device->CreateBuffer(&cBufferDesc, nullptr, &constantBuffer);

	this->tex->createTexture(device, this->textureFile);
}

void RenderObject::draw(ID3D11DeviceContext *& deviceContext) const
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
	deviceContext->PSSetShaderResources(0, 1, &this->tex->getTexture());
	deviceContext->PSSetSamplers(0, 1, &this->tex->getSampleState());

	deviceContext->Draw(this->mesh->getNrOfVertexes(), 0);
}

void RenderObject::setMatrix(const XMMATRIX & view, const XMMATRIX & proj)
{
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(posX, posY, posZ);

	XMVECTOR quat = XMVECTOR();
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(quat);


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

void RenderObject::setScale(float x, float y, float z)
{
	scaleX = x;
	scaleY = y;
	scaleZ = z;
}

RenderObject::RenderObject()
{
	this->mesh = new Mesh(L"r8.obj", false);
}

RenderObject::RenderObject(const wchar_t * meshDirr, LPCWSTR textureFile,const bool normalIn)
{
	this->mesh = new Mesh(meshDirr, normalIn);
	this->tex = new Texture();
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
	delete mesh;
	if (this->tex != nullptr)
		delete this->tex;

	this->vertexBuffer->Release();
	this->constantBuffer->Release();
}
