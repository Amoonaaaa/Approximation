#include"ExcelFile.h"

void ExcelFile::createFile(const std::string& file)
{
	
	OpenXLSX::XLDocument doc;
	doc.create(file);
	auto wks = doc.workbook().worksheet("Sheet1");
	for (int i = 0; i < 96; i++)
		for (int j = 0; j < 13; j++)
			wks.cell(OpenXLSX::XLCellReference(i + 1, j + 1)).value() = m_vecDataIn[i][j];
	doc.save();
	doc.close();
}

void ExcelFile::read(const std::string& file)
{
	
	OpenXLSX::XLDocument doc;
	doc.open(file);
	std::vector<std::string> nameSheets = doc.workbook().sheetNames();
	auto wks = doc.workbook().worksheet(nameSheets[0]);
	OpenXLSX::XLValueType type =  wks.cell(OpenXLSX::XLCellReference(1, 1)).value().type();
	m_vecDataOut = new std::string * [96];
	for (int i = 0; i < 96; i++)
		m_vecDataOut[i] = new std::string[13];
	for (int i = 0; i < 96; i++)
		for (int j = 0; j < 13; j++)
			m_vecDataOut[i][j] = wks.cell(OpenXLSX::XLCellReference(i + 1, j + 1)).value().get<std::string>();
	doc.save();
	doc.close();
}

void ExcelFile::setVecDataIn( std::string** vecDataIn)
{
	
	m_vecDataIn = vecDataIn;
}

std::string** ExcelFile::getVecDataIn()
{
	if (m_vecDataIn != nullptr)
		return m_vecDataIn;
	else
		return nullptr;
}

std::string** ExcelFile::getVecDataOut()
{
	if (m_vecDataOut == nullptr)
		return nullptr;
	else
		return m_vecDataOut;
}
