#pragma once
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>
/*
�������� ����
�������� ������� ���� � ������������ ��� ����. ��� �� �������� �� ����� ����

-AddSreenMenu(Name, ScreenMenu); ��������� �����, ���������� ��� ��������� ���
-Begin(Name);//������ �������� ������� � ���������� ������

private string Name; - ��� ���������� ����. ������ ���, ����� �����-���� ���� �������� ������
��� ��� ����������. ����� ���� �������� ���� ����� ���������� ����� ��� ���������� ���� ��� �����������, � �����
��� ������ �������� ���� ����������, �������� ���������� ����� ��������� ���� � ��������� ���.
����� ������������� ����������� ����� ����� ��������� ��������.
���� �������� ����� ������� ��� ������ � ����� ������ �� ����� ������������, ���������� ��������� ����������

-SetNextScreen(Name);//������������� ��������� �����

*/

class ScreenMenu;
class MenuManager
{
public:
	void AddScreenMenu(std::string Name, ScreenMenu *Menu);
	int Begin(std::string Name);
	void SetNextScreen(std::string Name);
private:
	std::map<std::string, std::unique_ptr<ScreenMenu>> ScreenMap;
	std::string ActiveScreen;

	int FindAndExecute(std::string Name);
};


class ScreenMenu
{
public:
	virtual int Perform()=0;

};

class LambdaScreenMenu : public ScreenMenu
{
private:
	std::function<int()> Lambda;
public:
	LambdaScreenMenu(std::function<int()> _Lambda);
	LambdaScreenMenu();
	int Perform();
};

