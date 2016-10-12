#pragma once
#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>
/*
Менеджер окон
Вызывает главное окно и регистрирует все окна. Так же отвечает за смену окон

-AddSreenMenu(Name, ScreenMenu); Добавляет экран, присваивая ему указанное имя
-Begin(Name);//Начало экранной чихарды с указанного экрана

private string Name; - Имя следующего окна. Всякий раз, когда какое-либо окно начинает работу
Это имя обнуляется. После чего активное окно может установить здесь имя следующего окна для отображения, и тогда
как только активное окно завершится, менеджер попытается найти указанное окно и запустить его.
Окнам рекомендуется завершаться сразу после установки значения.
Если значение будет нулевым или экрана с таким именем не будет существовать, исполнение программы завершится

-SetNextScreen(Name);//Устанавливает следующий экран

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

