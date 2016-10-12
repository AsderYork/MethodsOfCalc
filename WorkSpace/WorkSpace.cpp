// WorkSpace.cpp: ���������� ����� ����� ��� ����������� ����������.
//

#include "stdafx.h"
#include <vector>
#include "LambdaMathClass.h"
#include <math.h>
#include <iostream>
#include "RPN.h"
#include "Menu.h"

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




void MainOptions()
{
	printf("-----------------------\nChoose one of the following options'\n");
	printf("0 - Create new Function'\n");
	printf("1 - Show list of existed functions'\n");
	printf("2 - Lagranging'\n");
	printf("3 - Plotting of existed functions'\n");
	printf("4 - Find f(x)'\n");
	printf("5 - Differentiate a function'\n");
	printf("X - Exit'\n");
}

void AddNewFunction(RPN_Converter* Converter)
{
	printf("--------Creating function--------\n");
	printf("Insert Function'\n")
		;
	char Tmp[512];
	std::cin >> Tmp;

	std::string MathStr = Tmp;
	MathFunc *Func;
	Func = new MathFunc(Converter->StringToMathFunc(Tmp));
	printf("---------------------------------'\n");
	printf("%s\n", Func->GetString());
	printf("Is that Correct [Y-Continue/N-Leave]?'\n");

	char OneSym = 0;
	std::cin >> OneSym;

	switch (OneSym)
	{
	case 'Y':{
		printf("Insert function name. You can access it in later functions, using it name'\n");
		std::cin >> Tmp;
		Converter->AddFunctionToLibrary(Func, Tmp);
		return;
	}
	case 'N': {return;}
	default:
		return;
	}

}

void Lagranging(RPN_Converter* Converter)
{
	printf("--------Lagranging--------\n");
	printf("Choose function for lagranging\n");

	char Tmp[512];
	std::cin >> Tmp;

	MathFunc Func = Converter->GetFuncFromLibrary(Tmp);
	printf("--------------------------\n");
	printf("%s\n", Func.GetString());

	printf("Is that Correct [Y-Continue/N-Leave]?'\n");
	char OneSym = 0;
	std::cin >> OneSym;
	switch (OneSym)
	{
	case 'Y': {break;}
	case 'N': {return;}
	default: {return;}
	}

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
	MathFunc *Lagrange = new MathFunc(GetLagrangePolynom(Func, StartPos, Step, AmountOfNodes));
	Lagrange->DumpInFile("Lagrange.txt");
	printf("Polynom is dumped in Lagrange.txt\n");

	printf("Do you want to save it as function so it could be accessed later? [Y-Continue/N-Leave]?'\n");
	OneSym = 0;
	std::cin >> OneSym;
	switch (OneSym)
	{
	case 'Y': {break;}
	case 'N': {return;}
	default: {return;}
	}

	printf("Insert function name'\n");
	std::cin >> Tmp;
	Converter->AddFunctionToLibrary(Lagrange, Tmp);
	return;
}

void Plotting(RPN_Converter* Converter)
{
	printf("--------Plotting--------\n");
	printf("How many fuctions do you want to plot?\n");

	char Tmp[512];
	std::cin >> Tmp;

	int Amount = std::stoi(Tmp);

	if (Amount <= 0) { return; }

	GraphBuilder Plotter;
	MathFunc Func;

	for (int i = 0; i < Amount; i++) {
		printf("---------=?FUNC?=--------\n");
		std::cin >> Tmp;
		Func = Converter->GetFuncFromLibrary(Tmp);
		printf("--------------------------\n");
		printf("%s\n", Func.GetString());
		printf("Is that Correct [Y-Continue/N-Leave]?'\n");
		char OneSym = 0;
		std::cin >> OneSym;
		switch (OneSym)
		{
			case 'Y': {break;}
			case 'N': {return;}
			default: {return;}
		}
		Plotter.AddFunc(new MathFunc(Func), Tmp);
	}

	printf("Now let's draw it. Insert starting position\n");
	std::cin >> Tmp;
	float StartPos = atof(Tmp);

	printf("End position?\n");
	std::cin >> Tmp;
	float EndPos = atof(Tmp);

	printf("Amount of Slices?\n");
	std::cin >> Tmp;
	int Slices = atoi(Tmp);

	Plotter.DumpValues(StartPos, EndPos, Slices, "Plot.csv");

	printf("file dumped as Plot.csv\n");
}

void FindValueOfFunc(RPN_Converter* Converter) {
	char Tmp[512];
	printf("---------=?FUNC?=--------\n");
	std::cin >> Tmp;
	MathFunc Func = Converter->GetFuncFromLibrary(Tmp);
	printf("--------------------------\n");
	printf("%s\n", Func.GetString());
	printf("Is that Correct [Y-Continue/N-Leave]?'\n");
	char OneSym = 0;
	std::cin >> OneSym;
	switch (OneSym)
	{
	case 'Y': {break;}
	case 'N': {return;}
	default: {return;}
	}

	printf("x=");
	std::cin >> Tmp;
	float ValueOfX = atof(Tmp);

	printf("f(x)=%f\n", Func(ValueOfX));

}


void DifferentiateAFunc(RPN_Converter* Converter) {
	char Tmp[512];
	printf("---------=?FUNC?=--------\n");
	std::cin >> Tmp;
	MathFunc Func = Converter->GetFuncFromLibrary(Tmp);
	printf("--------------------------\n");
	printf("%s\n", Func.GetString());
	printf("Is that Correct [Y-Continue/N-Leave]?'\n");
	char OneSym = 0;
	std::cin >> OneSym;
	switch (OneSym)
	{
	case 'Y': {break;}
	case 'N': {return;}
	default: {return;}
	}

	printf("Step of differentiation?\n");
	std::cin >> Tmp;
	float Step = atof(Tmp);

	
	printf("Now name the function differential'\n");
	std::cin >> Tmp;	

	Converter->AddFunctionToLibrary(new MathFunc(Func.Diff(Step)), Tmp);
	return;

}


int main()
{
	RPN_Converter Converter;
	Converter.AddBasicFunctions();

	printf("Welcome to Multifunctional Calculator 'MathCalc'\n");

	char ReturnValue = 0;
	while (true)
	{
		MainOptions();
		std::cin >> ReturnValue;
		switch (ReturnValue)
		{
		case 'x': {return 0;}
		case '0': {AddNewFunction(&Converter); break;}
		case '1': {Converter.ListFuncsInLibrary(); break;}
		case '2': {Lagranging(&Converter); break;}
		case '3': {Plotting(&Converter); break;}
		case '4': {FindValueOfFunc(&Converter); break;}
		case '5': {DifferentiateAFunc(&Converter); break;}
		}
	}

	return 0;
}



