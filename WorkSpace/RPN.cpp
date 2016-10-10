#include "RPN.h"

bool IsNumber(std::string Str)
{
	for (int i = 0; i < Str.size(); i++) {
		switch (Str[i]) {
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '10':
		case '.':
		case'-':
		case'+': {break;}
		default: {return false;}
		}
	}
	return true;//Если мы так и не натолкнулись на что-то не числовое, значит это все таки число
}


TYPE ToNumber(std::string Str)
{
	TYPE Value = 0;

	//Используем простой вариант без проверок
	return std::strtod(Str.c_str(), nullptr);

}


std::vector<RPN_Converter::SimpleBlocks> RPN_Converter::StringToSimpleBlock(std::string Str)
{

	std::string ActualOperator = "";

	std::vector<SimpleBlocks> Output;

	/*Может случиться такая ситуация, при которой две операции будут следовать друг за другом
	Допустим "8 ^ -4", нетрудно видеть, что в данном случае, минус относится к числу, является его частью.
	Потму мы запоминаем, был ли последним внесенным значением оператор, и если да, а мы стоим на минусе или плюсе,
	значит этот минус или плюс не независимый оператор, а часть следующего значения*/
	bool OperatorBefore = false;

	for (int i = 0; i < Str.size(); i++)
	{
		

		//Если символ, на который мы попали - один из операторов
		if (Str[i] == '+' || Str[i] == '-' || Str[i] == '*' || Str[i] == '/' || Str[i] == '^')
		{
			if (OperatorBefore)//Если до этого был оператор, значит этот оператор относится к следующему значению
			{
				//Если у нас целая череда таких операторов, то последний записываем на выход, если есть
				if (!ActualOperator.empty())
				{//Если актуальный оператор был, то сначала заносим его
					Output.push_back(SimpleBlocks(ActualOperator));
					ActualOperator.clear();
				}
				//Теперь мы не снимаем флаг. Это позволит лепить последний оператор последователньости к следующему символу всегда
				ActualOperator += Str[i];//Таки добавляем его в начало актуального оператора
				}
			else
			{
				if (!ActualOperator.empty())
				{//Если актуальный оператор был, то сначала заносим его
					Output.push_back(SimpleBlocks(ActualOperator));
					ActualOperator.clear();
				}
				Output.push_back(SimpleBlocks(Str[i]));//Добавляем оператор как есть в вектор на выход
				OperatorBefore = true;
			}
		}
		//Если же это скобка
		else if (Str[i] == '(' || Str[i] == ')')
		{
			//Вся фишка в том, что скобки ни с кем не должны слипаться
			if (!ActualOperator.empty())
			{//Если актуальный оператор был, то сначала заносим его
				Output.push_back(SimpleBlocks(ActualOperator));
				ActualOperator.clear();
			}
			Output.push_back(SimpleBlocks(Str[i]));//Добавляем оператор как есть в вектор на выход
		}
		else//Если же это число или буква
		{
			OperatorBefore = false;
			ActualOperator += Str[i];//Добавляем этот символ к актуальному оператору
		}
	}

	//Заносим то, что осталось. Если осталось
	if (!ActualOperator.empty())
	{
		Output.push_back(SimpleBlocks(ActualOperator));
	}
	return Output;
}

std::vector<RPN_Converter::SimpleBlocks> RPN_Converter::SimpleBlocksToPRN(std::vector<SimpleBlocks> Input) {
	std::stack<SimpleBlocks> Stack;
	std::vector<SimpleBlocks> Output;


	for (auto it = Input.begin(); it != Input.end(); it++) {
		//Если это открывающая скобка
		if (it->Value.compare("(") == 0) {
			it->Type = -2;//Указываем тип
						 //Точно добавляем в стек
			Stack.push(*it);
		}

		//Если это возведение в степень
		else if (it->Value.compare("^") == 0) {
			it->Type = 3;
			//Выбрасываем из стека наружу все, что есть с более высоким приоритетом
			while ((!Stack.empty()) && (Stack.top().Type >= it->Type)) {
				Output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(*it);
		}


		//Если это умножение или деление
		else if (it->Value.compare("*") == 0 || it->Value.compare("/") == 0) {
			it->Type = 2;
			while ((!Stack.empty()) && (Stack.top().Type >= it->Type)) {
				Output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(*it);
		}

		//Если плюс или минус
		else if (it->Value.compare("+") == 0 || it->Value.compare("-") == 0) {
			it->Type = 1;
			while ((!Stack.empty()) && (Stack.top().Type >= it->Type)) {
				Output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(*it);
		}

		//Если закрывающая скобка
		else if (it->Value.compare(")") == 0) {

			//Используем флаг, что бы узнать в конце, кончился ли у нас стак, или мы таки нашли скобку
			bool CloserFind = false;

			while (!Stack.empty()) {

				//Выбрасываем из стека все до первой открывающей скобки
				if ((Stack.top().Type != -2)) {
					Output.push_back(Stack.top());
					Stack.pop();
				}
				else {
					CloserFind = true;
					Stack.pop();//Удаляем найденную скобку
					break;
				}
			}

			if (!CloserFind) {
				//Если такой нет - ошибка
				printf("Mismatch of brackets\n");
				std::vector<SimpleBlocks> Empty;
				return Empty;
			}
		}

		//Если мы здесь, значит у нас функция, переменная или число

		//Это переменная? (Их список наверху)
		else if (it->Value.compare("x") == 0) {
			//Если да, то сразу выбрасываем на выход
			Output.push_back(*it);
		}

		//Пробуем преобразовать к числу
		else if (IsNumber(it->Value)) {
			//Если получается, тоже выбрасываем на выход
			Output.push_back(*it);			
		}

		//Если совсем ничего не получается - значит это функция. Ищем в баке функций
		else {//Если находим, то
			  //Пока что ничего не ищем. Просто заносим в стек
			it->Type = 6;
			Stack.push(*it);
		}

	}

	//А теперь выкидываем из стека что осталось
	while (!Stack.empty())
	{
		Output.push_back(Stack.top());
		Stack.pop();
	}

 	return Output;
}

MathFunc RPN_Converter::RPN_To_MathFunc(std::vector<SimpleBlocks> Input) {
	std::stack<MathFunc> Funcs;
	MathFunc Operand1, Operand2;


	for (auto it = Input.begin(); it != Input.end(); it++) {
		if (it->Type == -1)//Если это число или переменная
		{
			//Если таки переменная
			if (it->Value.compare("x") == 0) {
				Funcs.push(MathFunc([](TYPE x) {return x;}, "x"));
			}
			//Если таки число
			else {
				Funcs.push(MathFunc(ToNumber(it->Value)));
			}

		}
		//Дальше проверяем посимвольно.

		//Если это сумма
		else if (it->Value.compare("+") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//Если не получилось в первый, не получится и во второй
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//Если же операнды достать удалось
			Funcs.push(MathFunc(Operand2 + Operand1));
		}
		else if (it->Value.compare("-") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//Если не получилось в первый, не получится и во второй
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//Если же операнды достать удалось
			Funcs.push(MathFunc(Operand2 - Operand1));
		}
		else if (it->Value.compare("*") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//Если не получилось в первый, не получится и во второй
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//Если же операнды достать удалось
			Funcs.push(MathFunc(Operand2 * Operand1));
		}
		else if (it->Value.compare("/") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//Если не получилось в первый, не получится и во второй
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//Если же операнды достать удалось
			Funcs.push(MathFunc(Operand2 / Operand1));
		}
		else if (it->Value.compare("^") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//Если не получилось в первый, не получится и во второй
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			Funcs.push(MathFunc(Operand2 ^ Operand1));
		}
		else if (it->Type == 6)
		{//Если у нас функция, пытаемся найти её в базе. Если не выходит, значит заменяем нулем
			MathFunc Function;
			try {
				Function = *FuncMap.at(it->Value);
			}
			catch (const std::out_of_range& oor)
			{
				printf("Func %s is not found. Use zero instead!\n", it->Value.c_str());
				Function = MathFunc();
			}
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			Funcs.push(MathFunc(Function(Operand2)));

		}
		else {
			printf("ERROR: Statement is incorrect. Return zero");
			return MathFunc();
		}
	}

	//Если мы закончили, а в стеке оказалось не одно значение
	if (Funcs.size() != 1)
	{
		printf("Calculation stack error. Return zero");
		return MathFunc();
	}
	else {
		return Funcs.top();
	}

}

MathFunc RPN_Converter::StringToMathFunc(std::string Str)
{
	return RPN_Converter::RPN_To_MathFunc(RPN_Converter::SimpleBlocksToPRN(RPN_Converter::StringToSimpleBlock(Str)));

};

void RPN_Converter::AddFunctionToLibrary(MathFunc* Func, std::string Name)
{
	FuncMap.insert(std::pair<std::string, std::unique_ptr<MathFunc>>(Name, Func));
}

void RPN_Converter::AddBasicFunctions()
{
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return cos(x);}, "cos"),"cos");
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return sin(x);}, "sin"), "sin");
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return tan(x);}, "tan"), "tan");

	AddFunctionToLibrary(new MathFunc([](TYPE x) {return acos(x);}, "acos"), "acos");
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return asin(x);}, "asin"), "asin");
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return atan(x);}, "atan"), "atan");


	AddFunctionToLibrary(new MathFunc([](TYPE x) {return exp(x);}, "exp"), "exp");
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return log(x);}, "log"), "log");
	AddFunctionToLibrary(new MathFunc([](TYPE x) {return log10(x);}, "lg"), "lg");


	AddFunctionToLibrary(new MathFunc([](TYPE x) {return sqrt(x);}, "sqrt"), "sqrt");

}