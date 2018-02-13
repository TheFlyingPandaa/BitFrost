#include "Matrial.h"



Matrial::Matrial()
{
	this->material = new mtl();
}

Matrial::Matrial(std::wstring mtlPath, std::wstring materialName)
{
	this->material = new mtl();
	this->path = mtlPath;
	this->loadMtl(materialName);
}



Matrial::~Matrial()
{

	delete material;
	
}

void Matrial::loadMtl(std::wstring materialName)
{
	std::wstring buff;
	wchar_t inputBuff[32];
	float fInputBuff[4]{0,0,0,1.0f};
	int iInputBuff = 0;

	bool skip = false;

	std::wifstream inFile(this->path);
	while (!inFile.eof() && std::getline(inFile, buff))
	{
		if (buff[0] == '#'){}
		else if (buff.substr(0, buff.find(' ')) == L"newmtl") {
			swscanf(buff.c_str(), L"%*ls %ls", &inputBuff);	
			if (materialName == inputBuff) {
				material->name = inputBuff;
				skip = false;
			}
			else
				skip = true;
		}
		else if (buff.substr(0, buff.find(' ')) == L"Ka" && !skip) {
			swscanf(buff.c_str(), L"%*s %f %f %f", &fInputBuff[0], &fInputBuff[1], &fInputBuff[2]);		
			material->ka = DirectX::XMFLOAT4A(fInputBuff);
		}
		else if (buff.substr(0, buff.find(' ')) == L"Kd" && !skip) {
			swscanf(buff.c_str(), L"%*ls %d %d %d", &fInputBuff[0], &fInputBuff[1], &fInputBuff[2]);
			material->kd = DirectX::XMFLOAT4A(fInputBuff);
		}
		else if (buff.substr(0, buff.find(' ')) == L"Ks" && !skip) {
			swscanf(buff.c_str(), L"%*ls %d %d %d", &fInputBuff[0], &fInputBuff[1], &fInputBuff[2]);
			material->ks = DirectX::XMFLOAT4A(fInputBuff);
		}
		else if (buff.substr(0, buff.find(' ')) == L"Ke" && !skip) {
			swscanf(buff.c_str(), L"%*s %f %f %f", &fInputBuff[0], &fInputBuff[1], &fInputBuff[2]);
			material->ke = DirectX::XMFLOAT4A(fInputBuff);
		}
		else if (buff.substr(0, buff.find(' ')) == L"Ns" && !skip) {
			swscanf(buff.c_str(), L"%*s %f", &fInputBuff[0]);
			material->ns = fInputBuff[0];
		}
		else if (buff.substr(0, buff.find(' ')) == L"Ni" && !skip) {
			swscanf(buff.c_str(), L"%*s %f", &fInputBuff[0]);
			material->ni = fInputBuff[0];
		}
		else if (buff.substr(0, buff.find(' ')) == L"d" && !skip) {
			swscanf(buff.c_str(), L"%*s %f", &fInputBuff[0]);
			material->d = fInputBuff[0];
		}
		else if (buff.substr(0, buff.find(' ')) == L"illum" && !skip) {
			swscanf(buff.c_str(), L"%*s %d", &iInputBuff);
			material->ilum = iInputBuff;
		}
		else if (buff.substr(0, buff.find(' ')) == L"map_Kd" && !skip) {
			swscanf(buff.c_str(), L"%*s %ls", &inputBuff);
			material->textureName = inputBuff;
		}
	}
	
	
	std::wofstream out("debug.txt");

	out << material->name << std::endl;
	out << material->ns << std::endl;
	out << material->ka.x << material->ka.y << material->ka.z << material->ka.w << std::endl;
	out << material->kd.x << material->kd.y << material->kd.z << material->kd.w << std::endl;
	out << material->ks.x << material->ks.y << material->ks.z << material->ks.w << std::endl;
	out << material->ke.x << material->ke.y << material->ke.z << material->ke.w << std::endl;
	out << material->ni << std::endl;
	out << material->d << std::endl;
	out << material->ilum << std::endl;
	out << material->textureName << std::endl;

	out.close();
	


	inFile.close();
}
