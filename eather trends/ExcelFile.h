#pragma once
#include<openxlsx/OpenXLSX.hpp>

class ExcelFile
{
public:
	// создание документа
	void createFile(const std::string& file);
	void read(const std::string& file);
	void setVecDataIn( std::string** vecDataIn);
	
	std::string** getVecDataIn();
	std::string** getVecDataOut();
private:
	std::string** m_vecDataIn;
	std::string** m_vecDataOut;
};