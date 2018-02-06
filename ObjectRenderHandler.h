#ifndef ORH_H
#define ORH_H

#include "RenderObject.h"

class ObjectRenderHandler
{
	std::string objectFile;

	std::vector<RenderObject*> renderObjects;

	std::wstring * objNames;
	std::wstring * textureNames;

public:
	ObjectRenderHandler();
	ObjectRenderHandler(const char * objectFile);
	~ObjectRenderHandler();

	void loadObjects();
	void loadBuffert(ID3D11Device *& device);

	void setMatrix(const DirectX::XMMATRIX & view, const DirectX::XMMATRIX & proj);
	void render(ID3D11DeviceContext *& gDevice);

	
};

#endif // !ORH_H
