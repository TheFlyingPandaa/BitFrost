#include "ObjectRenderHandler.h"

#include <cstringt.h>

ObjectRenderHandler::ObjectRenderHandler()
{
}

ObjectRenderHandler::ObjectRenderHandler(const char * objectFile)
{
	this->objectFile = objectFile;
}


ObjectRenderHandler::~ObjectRenderHandler()
{
	for (size_t i = 0; i < this->renderObjects.size(); i++)
	{
		delete this->renderObjects[i];
	}
	delete[] this->objNames;
	delete[] this->textureNames;
}

void ObjectRenderHandler::loadObjects()
{
	int i = 0;

	float** pos;
	float** scale;
	

	std::string buf;

	std::wifstream inFile(this->objectFile);
	inFile >> i;
	pos = new float*[i];
	scale = new float*[i];
	this->objNames = new std::wstring[i];
	this->textureNames = new std::wstring[i];
	for (size_t j = 0; j < i; j++)
	{
		inFile >> objNames[j];
		inFile >> textureNames[j];
		pos[j] = new float[3];
		scale[j] = new float[3];
		for (int k = 0; k < 3; k++)
		{
			inFile >> pos[j][k];
		}
		for (int k = 0; k < 3; k++)
		{
			inFile >> scale[j][k];
		}
	}
	inFile.close();
	RenderObject * obj;
	for (size_t j = 0; j < i; j++)
	{
		if (this->textureNames[j] == L"NAN") {
			obj = new RenderObject(this->objNames[j].c_str());
			obj->setPosition(pos[j][0], pos[j][1], pos[j][2]);
			obj->setScale(scale[j][0], scale[j][1], scale[j][2]);
			this->renderObjects.push_back(obj);
		}
		else {			
			obj = new RenderObject(this->objNames[j].c_str(), this->textureNames[j].c_str());
			obj->setPosition(pos[j][0], pos[j][1], pos[j][2]);
			obj->setScale(scale[j][0], scale[j][1], scale[j][2]);
			this->renderObjects.push_back(obj);			
		}
	}
	for (int j = 0; j < i; j++)
	{
		delete[] pos[j];
		delete[] scale[j];
	}
	delete[] pos;
	delete[] scale;
}


void ObjectRenderHandler::loadBuffert(ID3D11Device *& device)
{
	for (int i = 0; i < this->renderObjects.size(); i++)
	{
		this->renderObjects[i]->loadBuffer(device);
	}
}

void ObjectRenderHandler::setMatrix(const DirectX::XMMATRIX & view, const DirectX::XMMATRIX & proj)
{
	for (int i = 0; i < this->renderObjects.size(); i++)
	{
		this->renderObjects[i]->setMatrix(view, proj);
	}
}

void ObjectRenderHandler::render(ID3D11DeviceContext *& gDevice)
{
	for (int i = 0; i < this->renderObjects.size(); i++)
	{
		this->renderObjects[i]->draw(gDevice);
	}
}
