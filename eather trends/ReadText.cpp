#include "ReadText.h"

void ReadFile::read(const std::string& file)
{
	vecData = new std::string * [96];
	for (int i = 0; i < 96; i++)
		vecData[i] = new std::string[13];
	std::ifstream ifstr(file, std::ios::in);
	if (!ifstr.is_open())
		std::cout << "Error open file" << std::endl;
	for (int i = 0; i < 96; i++)
		for (int j = 0; j < 13; j++)
			ifstr >> vecData[i][j];
	ifstr.close();
}

std::string** ReadFile::getVecData()
{
	if (vecData != nullptr)
		return vecData;
	else
		return nullptr;
}
