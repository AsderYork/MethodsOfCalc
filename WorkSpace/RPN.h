#pragma once
#include "LambdaMathClass.h"
#include <map>
#include <math.h>
#include <stack>
#include <vector>
#include <queue>
#include <memory>

/*
��������� �������������.
���������� - {x}; ���� ��� ��� ���.
������� - ��� ���������
*/

bool IsNumber(std::string Str);

TYPE ToNumber(std::string Str);

class RPN_Converter
{
private:
	

	struct SimpleBlocks {//��� ����� ������. �� �� ����� ������ �������� ������ � ������� �� ���� �����. ������
						 //�� ������� ��������� ������ � ��������� ��������� ����, � ����� �������� ����� ��������� � �������
						 //� ��� ��������� �������� ��� � ���������� ���������� � �����, ���������� ������ ������ �������
		std::string Value;
		int Type;//0-Integer/Variable; 1-Sum/sub; 2-mul/div; 3-power; -2-Open; 5-Close 6-func; -1 - NOT CLASSIFIED
		SimpleBlocks(std::string _Value) :Value(_Value), Type(-1){}
		SimpleBlocks(char _Value) {
			Value = ""; 
			Value +=_Value;
			Type = -1;
		}
	};

	std::map<std::string, std::unique_ptr<MathFunc>> FuncMap;

public:

	void AddFunctionToLibrary(MathFunc* Func, std::string Name);

	MathFunc GetFuncFromLibrary(std::string Name);

	void ListFuncsInLibrary();

	static std::vector<SimpleBlocks> StringToSimpleBlock(std::string Str);

	/*static std::string StringToRPNString(std::string Str, std::string Variable = "x")
	{//C����� ������� ��� ������� � ������ ����. � �������� �����������

		std::vector<std::string> Output;
		std::vector<SimpleBlocks> SimpleBlockRepresentation;

	}*/
	
	static std::vector<SimpleBlocks> SimpleBlocksToPRN(std::vector<SimpleBlocks> Input);

	MathFunc RPN_To_MathFunc(std::vector<SimpleBlocks> Input);

	MathFunc StringToMathFunc(std::string Str);

	void AddBasicFunctions();
};