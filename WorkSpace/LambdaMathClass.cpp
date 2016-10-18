#include "LambdaMathClass.h"

MathFunc::MathFunc(TYPE Initializer)
{
	//Приведение типа к String с последующим сохранением
	char Buff[CONSTANT_INIT_BUFFER_SIZE];
	snprintf(Buff, CONSTANT_INIT_BUFFER_SIZE, "%f", Initializer);
	StringRepresentation = Buff;

	Func = [Initializer](TYPE x) {return Initializer;};
}

MathFunc::MathFunc()
{//Нулевая константа. К слову теперь производные констант на неё пологаются
	Func = [](TYPE x) {return 0;};

	StringRepresentation = "0";


}

MathFunc::MathFunc(MathFunc &Parent)
{
	Func = Parent.Func;
	StringRepresentation = Parent.StringRepresentation;
}


MathFunc::MathFunc(OneParamFunc InputFunc, std::string StrRep)
{
	Func = InputFunc;
	StringRepresentation = StrRep;
}


const char* MathFunc::GetString()
{
	return StringRepresentation.c_str();
}

void MathFunc::DumpInFile(std::string FileName)
{
	FILE *f = fopen(FileName.c_str(), "w");
	fprintf(f, "%s", StringRepresentation.c_str());
	fclose(f);
}

TYPE MathFunc::operator()(TYPE x)
{
	return Func(x);
}

MathFunc MathFunc::operator+(MathFunc &Right)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	

	auto RetLambda = [A = LocalFunc, B = Right.Func](TYPE x) {return A(x) + B(x);};

	std::string RetStr;
	RetStr += "(";
	RetStr += StringRepresentation.c_str();
	RetStr += "+";
	RetStr += Right.StringRepresentation.c_str();
	RetStr += ")";

	MathFunc Ret(RetLambda, RetStr);

	return Ret;
}

MathFunc MathFunc::operator-(MathFunc &Right)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	auto RetLambda = [A = LocalFunc, B = Right.Func](TYPE x) {return A(x) - B(x);};
	std::string RetStr;
	RetStr += "(";
	RetStr += StringRepresentation.c_str();
	RetStr += "-";
	RetStr += Right.StringRepresentation.c_str();
	RetStr += ")";

	return MathFunc(RetLambda, RetStr);
}

MathFunc MathFunc::operator*(MathFunc &Right)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	auto RetLambda = [A = LocalFunc, B = Right.Func](TYPE x) {return A(x) * B(x);};
	std::string RetStr;
	RetStr += "(";
	RetStr += StringRepresentation.c_str();
	RetStr += "*";
	RetStr += Right.StringRepresentation.c_str();
	RetStr += ")";

	return MathFunc(RetLambda, RetStr);
}

//Деление на ноль еще никто не отменял!
MathFunc MathFunc::operator/(MathFunc &Right)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	auto RetLambda = [A = LocalFunc, B = Right.Func](TYPE x) {return A(x) / B(x);};
	std::string RetStr;
	RetStr += "(";
	RetStr += StringRepresentation.c_str();
	RetStr += "/";
	RetStr += Right.StringRepresentation.c_str();
	RetStr += ")";

	return MathFunc(RetLambda, RetStr);
}

MathFunc MathFunc::operator^(MathFunc &Right)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	auto RetLambda = [A = LocalFunc, B = Right.Func](TYPE x) {return pow(A(x), B(x));};
	std::string RetStr;
	RetStr += "(";
	RetStr += StringRepresentation.c_str();
	RetStr += "^";
	RetStr += Right.StringRepresentation.c_str();
	RetStr += ")";

	return MathFunc(RetLambda, RetStr);
}

MathFunc& MathFunc::operator=(MathFunc &Right)
{
	this->Func = Right.Func;
	this->StringRepresentation = Right.StringRepresentation;
	return *this;
}

//Композиция не разбирается, правда ли одно можно встроить в другое, так что наслаждайтесь 0(x(cos(x(4))))
MathFunc MathFunc::operator()(MathFunc InnerPart)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	auto RetLambda = [A = LocalFunc, B = InnerPart.Func](TYPE x) {return A(B(x));};

	std::string RetStr;

	RetStr += StringRepresentation.c_str();
	RetStr += "(";
	RetStr += InnerPart.StringRepresentation.c_str();
	RetStr += ")";

	return MathFunc(RetLambda, RetStr);
}

MathFunc MathFunc::Diff(TYPE Step)
{
	auto LocalFunc = Func;//Лямбды не могут захватить поля класса. Создаем локальную копию
	auto RetLambda = [A = LocalFunc, Step](TYPE x) {return ((A(x+Step)-A(x-Step))/(2*Step));};

	std::string RetStr;
	RetStr += "Diff(";
	RetStr += StringRepresentation.c_str();
	RetStr += ")";

	return MathFunc(RetLambda, RetStr);
}

MathFunc MathFunc::Max(TYPE Start, TYPE End, TYPE Precision)
{
	assert(Start < End);
	assert(Precision > 0);
	TYPE Iterator = Start;

	TYPE RetVal = Func(Iterator);
	while (Iterator <= End)
	{
		if (Func(Iterator) > RetVal) {
			RetVal = Func(Iterator);
		}
		Iterator += Precision;
	}

	return MathFunc(RetVal);
}





void Envelope::Set(MathFunc Func)
{
	InnerFunc = Func;
}
MathFunc Envelope::Get()
{
	return InnerFunc;
}

//Плоттер


void GraphBuilder::AddFunc(MathFunc* Func, std::string Shortcut)
{
	Functions.push_back(Func);
	Shortcuts.push_back(Shortcut);
}

//Создает файл, в который сохраняет значения функции начиная со StartVal и по EndVal, делая AmountOfSlices равномерных слайсов
void GraphBuilder::DumpValues(TYPE StartVal, TYPE EndVal, int AmountOfSlices, std::string FileName)
{
	assert(AmountOfSlices != 0);
	FILE *f = fopen(FileName.c_str(), "w");
	TYPE Delta = (EndVal - StartVal) / AmountOfSlices;
	TYPE TmpPos = StartVal;

	//----------------------------------
	//Заполняем заголовок
	fprintf(f, "x");
	for (auto it = Shortcuts.begin(); it != Shortcuts.end(); it++) {
		fprintf(f, ",%s", it->c_str());
	}

	//----------------------------------
	//заполняем значения

	//Меньше или равно для того, что бы записать на один слайс больше, а именно значение на границе
	for (int i = 0; i <= AmountOfSlices; i++) {

		fprintf(f, "\n%f", TmpPos);//Значение X. К слову оно отвечает за перенос строки

		for (auto it = Functions.begin(); it != Functions.end(); it++) {
			fprintf(f, ",%f", (**it)(TmpPos));//Значения всех остальных функций
		}

		TmpPos += Delta;
	}

	fclose(f);
}
