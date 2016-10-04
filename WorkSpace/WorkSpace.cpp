// WorkSpace.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <vector>
#include "LambdaMathClass.h"
#include <math.h>

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



