#pragma once
#include <functional>
#include <math.h>
#include <fstream>
#include <assert.h>
#include <vector>

typedef float TYPE;
typedef std::function<TYPE(TYPE)> OneParamFunc;

#define CONSTANT_INIT_BUFFER_SIZE 16//Конструктор по константе использует буфер, что бы привести значения. Это - размер этого буфера


class MathFunc
{
private:
	OneParamFunc Func;

	std::string StringRepresentation;
	

public:


	//Инициализация константой
	MathFunc(TYPE Initializer);

	//Инициализация нулем. Выбран именно ноль, что бы сразу палиться
	MathFunc();

	//Инициализация другой функцией
	MathFunc(MathFunc &Parent);

	//Инициализация "лямбдой". Так же требуется строка, которая бы описывала содержание лямбды, а значит...
	//СОВПАДЕНИЕ СТРКОВОГО ПРЕДСТАВЛЕНИЯ И РЕАЛЬНОГО ЗНАЧЕНИЯ НЕ ГАРАНТИРУЕТСЯ!
	MathFunc(OneParamFunc InputFunc, std::string StrRep);


	//Возвращает функцию как простую строку
	const char* GetString();
	void DumpInFile(std::string FileName);

	//Оператор получения значения функции. Очень важная вещь, к слову!
	TYPE operator()(TYPE x);

	//------------------
	//Арифметические действия!
	//------------------

	MathFunc operator+(MathFunc &Right);

	MathFunc operator-(MathFunc &Right);

	MathFunc operator*(MathFunc &Right);
	//Деление на ноль еще никто не отменял!
	MathFunc operator/(MathFunc &Right);

	MathFunc operator^(MathFunc &Right);
	//------------------
	//Более сложные вещи
	//------------------
	MathFunc& operator=(MathFunc &Right);

	//Композиция
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

	//Создает файл, в который сохраняет значения функции начиная со StartVal и по EndVal, делая AmountOfSlices равномерных слайсов
	void DumpValues(TYPE StartVal, TYPE EndVal, int AmountOfSlices, std::string FileName);
};