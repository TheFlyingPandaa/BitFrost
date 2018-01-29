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

	deviceContext->Draw(this->mesh->getNrOfVertexes(), 0);
}

void RenderObject::setMatrix(const XMMATRIX & view, const XMMATRIX & proj)
{
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(1, 1, 1);

	XMVECTOR quat = XMVECTOR();
	XMMATRIX rotation = XMMatrixRotationRollPitchYawFromVector(quat);


	XMMATRIX scale = XMMatrixScaling(.5f, .5f, .5f);
	XMMATRIX m_worldMatrix = rotation * scale * translate;

	matrixBuffer.worldSpace = DirectX::XMMatrixTranspose(m_worldMatrix);
	matrixBuffer.WVP = DirectX::XMMatrixTranspose(m_worldMatrix * view * proj);
}

RenderObject::RenderObject()
{
	this->mesh = new Mesh("r8.obj");
}

RenderObject::RenderObject(const char * meshDirr)
{
	this->mesh = new Mesh(meshDirr);
}

RenderObject::~RenderObject()
{
	delete mesh;

	this->vertexBuffer->Release();
	this->constantBuffer->Release();
}
