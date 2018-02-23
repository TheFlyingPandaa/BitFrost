#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include "Mesh.h"
#include <string>
#include <cmath>

struct MatrixBuffert {
	DirectX::XMMATRIX WVP;
	DirectX::XMMATRIX worldSpace;
};
struct textureInformationBuffer {
	BOOL TEXTURE;
	BOOL NORMAL;
	DirectX::XMFLOAT3A tangent;
};
struct TessellationFactor
{
	float tessellationFactor;
	float pad;
	float pad2;
	float pad3;
};


class RenderObject
{
private:	
	Mesh * mesh;
	
	LPCWSTR textureFile;
	Texture ** tex;
	Texture ** normal;


	Texture * lowPolly;

	ID3D11Buffer ** vertexBuffer = nullptr;
	ID3D11Buffer * constantBuffer = nullptr;
	ID3D11Buffer * psConstantBuffer = nullptr;
	ID3D11Buffer * hullConstantBuffer = nullptr;

	XMFLOAT3 * position = nullptr;

	MatrixBuffert matrixBuffer;
	textureInformationBuffer texInfo;
	TessellationFactor tessInfo;

	float posX, posY, posZ;
	float scaleX, scaleY, scaleZ;

	bool distanceCalc(XMFLOAT3 * view);
public:
	RenderObject();

	RenderObject(const char * meshDirr, LPCWSTR textureFile = NULL, const bool normalIn = false);
	RenderObject(const wchar_t * meshDirr, LPCWSTR textureFile = NULL, const bool normalIn = false);
	~RenderObject();

	void loadBuffer(ID3D11Device *& device);
	void draw(ID3D11DeviceContext *& deviceContext, XMFLOAT3 * view);

	void setMatrix(const XMMATRIX& view, const XMMATRIX& proj, float radsRot = NULL);

	void setPosition(float x, float y, float z);
	XMFLOAT3 getPosition() const;
	void setScale(float x, float y, float z);

	

	XMFLOAT3 * getPos();
};

#endif // !RENDER_OBJECT_H
