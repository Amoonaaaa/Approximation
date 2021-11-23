#pragma once
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

class ReadFile
{
public:
	void read(const std::string& file);
	std::string** getVecData();
private:
	std::string** vecData;
};