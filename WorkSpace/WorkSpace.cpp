// WorkSpace.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <vector>
#include "LambdaMathClass.h"
#include <math.h>
#include <iostream>
#include "RPN.h"

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


//� ������ ������� � ����� �������



int main()
{
	

	/*MathFunc F4([](TYPE x) {return tan(x);}, "tan");

	MathFunc Lagrange = GetLagrangePolynom(F4, -1.5, 0.4, 7);

	printf("\n----------------------------");
	printf("\n%s", Lagrange.GetString());

	printf("\n----------------------------");

	GraphBuilder Plot;
	Plot.AddFunc(&F4, "tanX");
	Plot.AddFunc(&Lagrange, "Lagrange");

	Plot.DumpValues(-1.8, 1.8, 500, "Data.csv");*/

	std::string Input;

	char Tmp[256];

	RPN_Converter Converter;
	Converter.AddBasicFunctions();

	while (true) {
		printf("Insert Function\n");
		std::cin >> Tmp;
		Input = Tmp;
		printf("--------------------------\n");
		MathFunc Ret = Converter.StringToMathFunc(Input);
		printf("%s\n",Ret.GetString());
		printf("--------------------------\n");

		printf("Amount of nodes?\n");
		std::cin >> Tmp;
		int AmountOfNodes = atoi(Tmp);

		printf("Starting Position?\n");
		std::cin >> Tmp;
		float StartPos = atof(Tmp);

		printf("Step size?\n");
		std::cin >> Tmp;
		float Step = atof(Tmp);

		printf("--------------------------\n");
		printf("      Lagranging...\n");
		printf("       Please wait\n");
		printf("--------------------------\n");
		MathFunc Lagrange = GetLagrangePolynom(Ret, StartPos, Step, AmountOfNodes);
		Lagrange.DumpInFile("Lagrange.txt");
		printf("Polynom is dumped in Lagrange.txt\n");

		GraphBuilder Plotter;
		Plotter.AddFunc(&Ret, "OriginalFunc");
		Plotter.AddFunc(&Lagrange, "Lagrange");

		printf("--------------------------\n");
		printf("Now let's draw it. Insert starting position\n");
		std::cin >> Tmp;
		StartPos = atof(Tmp);

		printf("End position?\n");
		std::cin >> Tmp;
		float EndPos = atof(Tmp);

		printf("Amount of Slices?\n");
		std::cin >> Tmp;
		int Slices = atoi(Tmp);

		Plotter.DumpValues(StartPos, EndPos, Slices, "Plot.csv");

		printf("file dumped as Plot.csv\n");


		printf("\n==========================\n");
	}
	return 0;
}



