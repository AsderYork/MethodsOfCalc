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
	return true;//���� �� ��� � �� ������������ �� ���-�� �� ��������, ������ ��� ��� ���� �����
}


TYPE ToNumber(std::string Str)
{
	TYPE Value = 0;

	//���������� ������� ������� ��� ��������
	return std::strtod(Str.c_str(), nullptr);

}


std::vector<RPN_Converter::SimpleBlocks> RPN_Converter::StringToSimpleBlock(std::string Str)
{

	std::string ActualOperator = "";

	std::vector<SimpleBlocks> Output;

	/*����� ��������� ����� ��������, ��� ������� ��� �������� ����� ��������� ���� �� ������
	�������� "8 ^ -4", �������� ������, ��� � ������ ������, ����� ��������� � �����, �������� ��� ������.
	����� �� ����������, ��� �� ��������� ��������� ��������� ��������, � ���� ��, � �� ����� �� ������ ��� �����,
	������ ���� ����� ��� ���� �� ����������� ��������, � ����� ���������� ��������*/
	bool OperatorBefore = false;

	for (int i = 0; i < Str.size(); i++)
	{
		

		//���� ������, �� ������� �� ������ - ���� �� ����������
		if (Str[i] == '+' || Str[i] == '-' || Str[i] == '*' || Str[i] == '/' || Str[i] == '^')
		{
			if (OperatorBefore)//���� �� ����� ��� ��������, ������ ���� �������� ��������� � ���������� ��������
			{
				//���� � ��� ����� ������ ����� ����������, �� ��������� ���������� �� �����, ���� ����
				if (!ActualOperator.empty())
				{//���� ���������� �������� ���, �� ������� ������� ���
					Output.push_back(SimpleBlocks(ActualOperator));
					ActualOperator.clear();
				}
				//������ �� �� ������� ����. ��� �������� ������ ��������� �������� ������������������ � ���������� ������� ������
				ActualOperator += Str[i];//���� ��������� ��� � ������ ����������� ���������
				}
			else
			{
				if (!ActualOperator.empty())
				{//���� ���������� �������� ���, �� ������� ������� ���
					Output.push_back(SimpleBlocks(ActualOperator));
					ActualOperator.clear();
				}
				Output.push_back(SimpleBlocks(Str[i]));//��������� �������� ��� ���� � ������ �� �����
				OperatorBefore = true;
			}
		}
		//���� �� ��� ������
		else if (Str[i] == '(' || Str[i] == ')')
		{
			//��� ����� � ���, ��� ������ �� � ��� �� ������ ���������
			if (!ActualOperator.empty())
			{//���� ���������� �������� ���, �� ������� ������� ���
				Output.push_back(SimpleBlocks(ActualOperator));
				ActualOperator.clear();
			}
			Output.push_back(SimpleBlocks(Str[i]));//��������� �������� ��� ���� � ������ �� �����
		}
		else//���� �� ��� ����� ��� �����
		{
			OperatorBefore = false;
			ActualOperator += Str[i];//��������� ���� ������ � ����������� ���������
		}
	}

	//������� ��, ��� ��������. ���� ��������
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
		//���� ��� ����������� ������
		if (it->Value.compare("(") == 0) {
			it->Type = -2;//��������� ���
						 //����� ��������� � ����
			Stack.push(*it);
		}

		//���� ��� ���������� � �������
		else if (it->Value.compare("^") == 0) {
			it->Type = 3;
			//����������� �� ����� ������ ���, ��� ���� � ����� ������� �����������
			while ((!Stack.empty()) && (Stack.top().Type >= it->Type)) {
				Output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(*it);
		}


		//���� ��� ��������� ��� �������
		else if (it->Value.compare("*") == 0 || it->Value.compare("/") == 0) {
			it->Type = 2;
			while ((!Stack.empty()) && (Stack.top().Type >= it->Type)) {
				Output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(*it);
		}

		//���� ���� ��� �����
		else if (it->Value.compare("+") == 0 || it->Value.compare("-") == 0) {
			it->Type = 1;
			while ((!Stack.empty()) && (Stack.top().Type >= it->Type)) {
				Output.push_back(Stack.top());
				Stack.pop();
			}
			Stack.push(*it);
		}

		//���� ����������� ������
		else if (it->Value.compare(")") == 0) {

			//���������� ����, ��� �� ������ � �����, �������� �� � ��� ����, ��� �� ���� ����� ������
			bool CloserFind = false;

			while (!Stack.empty()) {

				//����������� �� ����� ��� �� ������ ����������� ������
				if ((Stack.top().Type != -2)) {
					Output.push_back(Stack.top());
					Stack.pop();
				}
				else {
					CloserFind = true;
					Stack.pop();//������� ��������� ������
					break;
				}
			}

			if (!CloserFind) {
				//���� ����� ��� - ������
				printf("Mismatch of brackets\n");
				std::vector<SimpleBlocks> Empty;
				return Empty;
			}
		}

		//���� �� �����, ������ � ��� �������, ���������� ��� �����

		//��� ����������? (�� ������ �������)
		else if (it->Value.compare("x") == 0) {
			//���� ��, �� ����� ����������� �� �����
			Output.push_back(*it);
		}

		//������� ������������� � �����
		else if (IsNumber(it->Value)) {
			//���� ����������, ���� ����������� �� �����
			Output.push_back(*it);			
		}

		//���� ������ ������ �� ���������� - ������ ��� �������. ���� � ���� �������
		else {//���� �������, ��
			  //���� ��� ������ �� ����. ������ ������� � ����
			it->Type = 6;
			Stack.push(*it);
		}

	}

	//� ������ ���������� �� ����� ��� ��������
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
		if (it->Type == -1)//���� ��� ����� ��� ����������
		{
			//���� ���� ����������
			if (it->Value.compare("x") == 0) {
				Funcs.push(MathFunc([](TYPE x) {return x;}, "x"));
			}
			//���� ���� �����
			else {
				Funcs.push(MathFunc(ToNumber(it->Value)));
			}

		}
		//������ ��������� �����������.

		//���� ��� �����
		else if (it->Value.compare("+") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//���� �� ���������� � ������, �� ��������� � �� ������
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//���� �� �������� ������� �������
			Funcs.push(MathFunc(Operand2 + Operand1));
		}
		else if (it->Value.compare("-") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//���� �� ���������� � ������, �� ��������� � �� ������
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//���� �� �������� ������� �������
			Funcs.push(MathFunc(Operand2 - Operand1));
		}
		else if (it->Value.compare("*") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//���� �� ���������� � ������, �� ��������� � �� ������
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//���� �� �������� ������� �������
			Funcs.push(MathFunc(Operand2 * Operand1));
		}
		else if (it->Value.compare("/") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//���� �� ���������� � ������, �� ��������� � �� ������
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			//���� �� �������� ������� �������
			Funcs.push(MathFunc(Operand2 / Operand1));
		}
		else if (it->Value.compare("^") == 0) {
			if (!Funcs.empty()) { Operand1 = Funcs.top();Funcs.pop(); }
			if (!Funcs.empty()) { Operand2 = Funcs.top();Funcs.pop(); }//���� �� ���������� � ������, �� ��������� � �� ������
			else { printf("ERROR! Statement is incorrect. Return zero;\n"); return MathFunc(); }
			Funcs.push(MathFunc(Operand2 ^ Operand1));
		}
		else if (it->Type == 6)
		{//���� � ��� �������, �������� ����� � � ����. ���� �� �������, ������ �������� �����
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

	//���� �� ���������, � � ����� ��������� �� ���� ��������
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