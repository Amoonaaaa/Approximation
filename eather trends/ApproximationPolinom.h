#pragma once
#include<iostream>
#include<string>;
#include<fstream>
template<typename Type>
class ApproximationPolinom
{
public:
	ApproximationPolinom();
	~ApproximationPolinom();
	void setSizePolinom(int n);
	void Matrix_Transform(Type* input_x, Type* input_y, int sizeData);
	void Matrix_LU();
	void Error_Estimate();
	void SaveTextPolinom(const std::string& nameFile);
	Type Polinom(Type Value);
private:
	Type* Coefficient;
	Type** Matrix_Main;
	Type** Matrix_L;
	Type** Matrix_U;
	Type* Matrix_Mapping;
	Type** Matrix_Error_Estimate;
	int n;
};

template<typename Type>
inline ApproximationPolinom<Type>::ApproximationPolinom()
{
}

template<typename Type>
inline ApproximationPolinom<Type>::~ApproximationPolinom()
{
	
	for (int i = 0; i < n; i++)
	{
		delete[]Matrix_L[i];
		delete[]Matrix_U[i];
		delete[]Matrix_Main[i];
		
	}
	delete[]Matrix_L;
	delete[]Matrix_Main;
	delete[]Matrix_Mapping;
	delete[]Matrix_U;
	delete[]Coefficient;
}

template<typename Type>
inline void ApproximationPolinom<Type>::setSizePolinom(int n)
{
	this->n = n;
}

template<typename Type>
inline void ApproximationPolinom<Type>::Matrix_Transform(Type* input_x, Type* input_y, int sizeData)
{
	Type supportSumm;
	Type supportMult;
	Type support = 0;
	Matrix_Main = new Type * [n];
	for (int i = 0; i < n; i++)
		Matrix_Main[i] = new Type[n];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			supportSumm = 0;
			for (int t = 0; t < sizeData; t++)
			{
				supportMult = 1;
				for (int k = 0; k < i + j; k++)
				{
					supportMult = supportMult * input_x[t];
				}
				supportSumm += supportMult;
			}
			Matrix_Main[i][j] = supportSumm;
		}
	}
	
	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			std::cout << Matrix_Main[i][j] << "\t";
		}
		*/
	Matrix_Mapping = new Type[n];
	for (int k = 0; k < n; k++)
	{
		supportSumm = 0;
		for (int i = 0; i < sizeData; i++)
		{
			supportMult = 1;
			for (int j = 0; j < k; j++)
			{
				supportMult *= input_x[i];
			}
			supportSumm += supportMult * input_y[i];
		}
		Matrix_Mapping[k] = supportSumm;
	//	std::cout << Matrix_Mapping[k] << std::endl;
	}
	
}

template<typename Type>
inline void ApproximationPolinom<Type>::Matrix_LU()
{
	Matrix_L = new Type * [n];
	for (int i = 0; i < n; i++)
		Matrix_L[i] = new Type[n];

	Matrix_U = new Type * [n];
	for (int i = 0; i < n; i++)
		Matrix_U[i] = new Type[n];

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			Matrix_L[i][j] = 0;
			Matrix_U[i][j] = 0;
			if (i == j)
				Matrix_U[i][j] = 1;
		}
	}
	Type support = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			support = 0;
			if (i >= j)
			{
				for (int k = 0; k < i; k++)
					support += Matrix_L[i][k] * Matrix_U[k][j];
				Matrix_L[i][j] = Matrix_Main[i][j] - support;
			}
			else
			{
				for (int k = 0; k < i; k++)
					support += Matrix_L[i][k] * Matrix_U[k][j];
				Matrix_U[i][j] = (Matrix_Main[i][j] - support) / Matrix_L[i][i];
			}
		}
	}
	//std::cout << "Matirx_L \n";
	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << Matrix_L[i][j] << "\t";
		std::cout << "\n";
	}
	//std::cout << "Matirx_U \n";
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << Matrix_U[i][j] << "\t";
		std::cout << "\n";
	}*/
	//std::cout << "suppVec \n";
	Type* suppVec = new Type[n];
	for (int i = 0; i < n; i++)
	{
		support = 0;
		for (int j = 0; j < i; j++)
			support += Matrix_L[i][j] * suppVec[j];
		suppVec[i] = (Matrix_Mapping[i] - support) / Matrix_L[i][i];
	//	std::cout << suppVec[i] << "\n";
	}
	//std::cout << "Coef\n";
	Coefficient = new Type[n];
	support = 0;
	for (int i = 0; i < n; i++)
	{
		
		for (int j = n - i; j < n; j++)
			support += Matrix_U[n - i - 1][j] * Coefficient[j];
		Coefficient[n - i - 1] = suppVec[n - i - 1] - support;
	//	std::cout << Coefficient[n - i - 1] << std::endl;
		support = 0;
	}
	delete[]suppVec;
}

template<typename Type>
inline void ApproximationPolinom<Type>::Error_Estimate()
{
	
	Matrix_Error_Estimate = new Type * [n];
	for (int i = 0; i < n; i++)
	{
		Matrix_Error_Estimate[i] = new Type[n];
	}
	Type support = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			support = 0;
			for (int k = 0; k < n; k++)
			{
				support += Matrix_L[i][k] * Matrix_U[k][j];
			}

			Matrix_Error_Estimate[i][j] = Matrix_Main[i][j] - support;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			std::cout << Matrix_Error_Estimate[i][j] << "\t";
		std::cout << "\n";
	}

}

template<typename Type>
inline void ApproximationPolinom<Type>::SaveTextPolinom(const std::string& nameFile)
{
	std::ofstream ofstr(nameFile, std::ios_base::ate | std::ios_base::app);
	
	ofstr << "f(x) =\t" ;
	ofstr << Coefficient[0] <<"\t" << '+' <<"\t";
	if(n > 2)
	ofstr << Coefficient[1] << "x" << "\t" << "+" << "\t";
	else
	{
		ofstr << Coefficient[1] << "x" <<";" << "\n" << "\n";
	}
	for (int i = 2; i < n; i++)
	{
		//ofstr << Coefficient[i];
		if (i != (n - 1))
		{
			ofstr << Coefficient[i] << "x^" << i << "\t" << '+' <<"\t";
			

		}
		else
		{
			ofstr << Coefficient[i] << "x^" << i << ';' <<"\n"<<"\n";
		}
			
	}
	ofstr.close();
}

template<typename Type>
inline Type ApproximationPolinom<Type>::Polinom(Type Value)
{
	Type resultSum = 0, resultMult = 1;
	for (int i = 0; i < n; i++)
	{
		resultSum += Coefficient[i] * resultMult;
		resultMult = resultMult * Value;
	}
	return resultSum;
}
