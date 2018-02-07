#ifndef ORH_H
#define ORH_H

#include "RenderObject.h"

#include <functional>
#include <algorithm>

using namespace std::placeholders;

class ObjectRenderHandler
{
	std::string objectFile;

	std::vector<RenderObject*> renderObjects;
	std::vector<RenderObject*> objectsToRender;

	std::wstring * objNames;
	std::wstring * textureNames;

	DirectX::XMFLOAT3 * camPos;

	void sortByDistance(const DirectX::XMVECTOR & camPos);
	void removeBehind(const DirectX::XMVECTOR & camPos, const DirectX::XMVECTOR & lookAt);

	//bool closes(RenderObject * a, RenderObject * b);


public:
	ObjectRenderHandler();
	ObjectRenderHandler(const char * objectFile);
	~ObjectRenderHandler();

	void loadObjects();
	void loadBuffert(ID3D11Device *& device);

	void setCamPosition(const DirectX::XMVECTOR & camPos, const DirectX::XMVECTOR & lookAt);
	void setMatrix(const DirectX::XMMATRIX & view, const DirectX::XMMATRIX & proj);
	void render(ID3D11DeviceContext *& gDevice);

	
};

#endif // !ORH_H
