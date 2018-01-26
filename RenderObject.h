#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "Mesh.h"

struct MatrixBuffert {
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;
};

class RenderObject
{
private:	
	Mesh * mesh;

	ID3D11Buffer * vertexBuffer = nullptr;
	ID3D11Buffer * constantBuffer = nullptr;

	MatrixBuffert matrixBuffer;

	float posX, posY, posZ;

public:
	RenderObject();
	RenderObject(const char * meshDirr);
	~RenderObject();

	void loadBuffer(ID3D11Device *& device);
	void draw(ID3D11DeviceContext *& deviceContext) const;

	void setMatrix(const XMMATRIX& view, const XMMATRIX& proj);
};

#endif // !RENDER_OBJECT_H
