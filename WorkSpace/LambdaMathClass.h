#pragma once
#include <functional>
#include <math.h>
#include <fstream>
#include <assert.h>
#include <vector>

typedef float TYPE;
typedef std::function<TYPE(TYPE)> OneParamFunc;

#define CONSTANT_INIT_BUFFER_SIZE 16//����������� �� ��������� ���������� �����, ��� �� �������� ��������. ��� - ������ ����� ������


class MathFunc
{
private:
	OneParamFunc Func;

	std::string StringRepresentation;
	

public:


	//������������� ����������
	MathFunc(TYPE Initializer);

	//������������� �����. ������ ������ ����, ��� �� ����� ��������
	MathFunc();

	//������������� ������ ��������
	MathFunc(MathFunc &Parent);

	//������������� "�������". ��� �� ��������� ������, ������� �� ��������� ���������� ������, � ������...
	//���������� ��������� ������������� � ��������� �������� �� �������������!
	MathFunc(OneParamFunc InputFunc, std::string StrRep);


	//���������� ������� ��� ������� ������
	const char* GetString();
	void DumpInFile(std::string FileName);

	//�������� ��������� �������� �������. ����� ������ ����, � �����!
	TYPE operator()(TYPE x);

	//------------------
	//�������������� ��������!
	//------------------

	MathFunc operator+(MathFunc &Right);

	MathFunc operator-(MathFunc &Right);

	MathFunc operator*(MathFunc &Right);
	//������� �� ���� ��� ����� �� �������!
	MathFunc operator/(MathFunc &Right);

	MathFunc operator^(MathFunc &Right);
	//------------------
	//����� ������� ����
	//------------------
	MathFunc& operator=(MathFunc &Right);

	//����������
	MathFunc operator()(MathFunc InnerPart);

};

class Envelope
{
private:
	MathFunc InnerFunc;
public:
	void Set(MathFunc Func);
	MathFunc Get();
};

class GraphBuilder
{
private:
	std::vector<MathFunc*> Functions;
	std::vector<std::string> Shortcuts;


public:

	void AddFunc(MathFunc* Func, std::string Shortcut);

	//������� ����, � ������� ��������� �������� ������� ������� �� StartVal � �� EndVal, ����� AmountOfSlices ����������� �������
	void DumpValues(TYPE StartVal, TYPE EndVal, int AmountOfSlices, std::string FileName);
};