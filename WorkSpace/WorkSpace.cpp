// WorkSpace.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vector>
#include "LambdaMathClass.h"
#include <math.h>
#include <iostream>
#include "RPN.h"
#include "Menu.h"

//Вычисление Многочлена Лагранжа самыми страшными методами
//Получает функцию, начальную точку, расстояние между шагами и количевтво узлов


//Число нод должно быть больше нуля!
MathFunc GetLagrangePolynom(MathFunc Func, TYPE FirstStep, TYPE StepDelta, int AmountOfSteps)
{
	assert(AmountOfSteps>0);

//-------
//Подготовка
//-------
	std::vector<TYPE> Nodes;
	Nodes.push_back(FirstStep);//записываем позицию первого узла

	for (int i=1; i < AmountOfSteps; i++) {
		Nodes.push_back(FirstStep+i*StepDelta);//И продолжаем для оставшихся узлов
	}

	std::vector<TYPE> NodeValues;//Значение функции в нодах

	for (auto it = Nodes.begin(); it != Nodes.end(); it++) {
		NodeValues.push_back(Func((*it)));//Записываем значение функции в каждом узле
	}
	
//-------
//Составление полинома
//-------
	

	MathFunc Param([](TYPE x) {return x;}, "x");//Параметр. Очень простая вещь.
	MathFunc Invisible([](TYPE x) {return 1;}, "1");//Невидимая единица, что бы не мазолить глаза

	MathFunc Result;//Полином, который мы и хотели получить

	MathFunc Multiplier;

	for (int i = 0; i < AmountOfSteps; i++) {
		Multiplier = Invisible;//Присваиваем множителю единицу




		//Мультиплекативно заполняем множитель до готового состояния
		for (int j = 0; j < AmountOfSteps; j++) {
			if (i != j)
			{	//
				Multiplier = Multiplier*( ( Param - MathFunc(Nodes[j]) )  / ( MathFunc(Nodes[i]) - MathFunc(Nodes[j]) ) );//Один кусок 
			}
		}
		printf("K%i  :  %s\n", i, Multiplier.GetString());
		Multiplier.DumpInFile("Multiplier.csv");

		Result = Result + (MathFunc(NodeValues[i]) * Multiplier);
	}
	
	//Метод получился ужасающе сложным. Как, впрочем, и все здесь. Удачи мне с проверкой/отладкой
	return Result;
}

MathFunc GetLagrangeReminder(MathFunc Func, MathFunc Diff, TYPE FirstStep, TYPE StepDelta, int AmountOfSteps)
{
	assert(AmountOfSteps >= 1);
	MathFunc JustX([](TYPE x) {return x;}, "x");


	std::vector<TYPE> Nodes;
	Nodes.push_back(FirstStep);//записываем позицию первого узла

	for (int i = 1; i < AmountOfSteps; i++) {
		Nodes.push_back(FirstStep + i*StepDelta);//И продолжаем для оставшихся узлов
	}


	MathFunc MulPart = JustX - MathFunc(Nodes[0]);
	//Составляем произведение частей, зависящих от узлов
	for (int i = 1; i < AmountOfSteps; i++) {
		MulPart = MulPart*(JustX - MathFunc(Nodes[i]));
	}

	int Fractorial = 1;
	for (int i = 2; i <= AmountOfSteps; i++) {
		Fractorial *= i;
	}

	MathFunc Result = Diff / MathFunc(Fractorial);
	Result = Result*MulPart;

	
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
	printf("Choose function for lagranging. Btw, if yow want a reminder, you'll better prepare the diff func before lagranging\n");

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


	printf("Maybe you also want to claclulate and save the remainder? [Y-Continue/N-Leave]?'\n");
	printf("Chose diff func for the one you've been lagranging. It should be a %i-th one\n", AmountOfNodes);

	std::cin >> Tmp;

	MathFunc Diff = Converter->GetFuncFromLibrary(Tmp);
	printf("--------------------------\n");
	printf("%s\n", Func.GetString());

	printf("Is that Correct [Y-Continue/N-Leave]?'\n");
	OneSym = 0;
	std::cin >> OneSym;
	switch (OneSym)
	{
	case 'Y': {break;}
	case 'N': {return;}
	default: {return;}
	}
	MathFunc *Remainder = new MathFunc(GetLagrangeReminder(Func, Diff, StartPos, Step, AmountOfNodes));

	printf("Insert remainder function name'\n");
	std::cin >> Tmp;
	Converter->AddFunctionToLibrary(Remainder, Tmp);
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



