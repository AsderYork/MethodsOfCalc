// WorkSpace.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <vector>
#include "LambdaMathClass.h"
#include <math.h>

//���������� ���������� �������� ������ ��������� ��������
//�������� �������, ��������� �����, ���������� ����� ������ � ���������� �����


//����� ��� ������ ���� ������ ����!
MathFunc GetLagrangePolynom(MathFunc Func, TYPE FirstStep, TYPE StepDelta, int AmountOfSteps)
{
	assert(AmountOfSteps>0);

//-------
//����������
//-------
	std::vector<TYPE> Nodes;
	Nodes.push_back(FirstStep);//���������� ������� ������� ����

	for (int i=1; i < AmountOfSteps; i++) {
		Nodes.push_back(FirstStep+i*StepDelta);//� ���������� ��� ���������� �����
	}

	std::vector<TYPE> NodeValues;//�������� ������� � �����

	for (auto it = Nodes.begin(); it != Nodes.end(); it++) {
		NodeValues.push_back(Func((*it)));//���������� �������� ������� � ������ ����
	}
	
//-------
//����������� ��������
//-------
	

	MathFunc Param([](TYPE x) {return x;}, "x");//��������. ����� ������� ����.
	MathFunc Invisible([](TYPE x) {return 1;}, "1");//��������� �������, ��� �� �� �������� �����

	MathFunc Result;//�������, ������� �� � ������ ��������

	MathFunc Multiplier;

	for (int i = 0; i < AmountOfSteps; i++) {
		Multiplier = Invisible;//����������� ��������� �������




		//���������������� ��������� ��������� �� �������� ���������
		for (int j = 0; j < AmountOfSteps; j++) {
			if (i != j)
			{	//
				Multiplier = Multiplier*( ( Param - MathFunc(Nodes[j]) )  / ( MathFunc(Nodes[i]) - MathFunc(Nodes[j]) ) );//���� ����� 
			}
		}
		printf("K%i  :  %s\n", i, Multiplier.GetString());
		Multiplier.DumpInFile("Multiplier.csv");

		Result = Result + (MathFunc(NodeValues[i]) * Multiplier);
	}
	
	//����� ��������� �������� �������. ���, �������, � ��� �����. ����� ��� � ���������/��������
	return Result;
}








int main()
{
	

	MathFunc F4([](TYPE x) {return tan(x);}, "tan");

	MathFunc Lagrange = GetLagrangePolynom(F4, -1.5, 0.4, 7);

	printf("\n----------------------------");
	printf("\n%s", Lagrange.GetString());

	printf("\n----------------------------");

	GraphBuilder Plot;
	Plot.AddFunc(&F4, "tanX");
	Plot.AddFunc(&Lagrange, "Lagrange");

	Plot.DumpValues(-1.8, 1.8, 500, "Data.csv");

	return 0;
}



