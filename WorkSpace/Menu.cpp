#include "Menu.h"


void MenuManager::AddScreenMenu(std::string Name, ScreenMenu *Menu) {
	ScreenMap.insert(std::pair<std::string, std::unique_ptr<ScreenMenu>>(Name, Menu));
	}
int  MenuManager::Begin(std::string Name) {

	int Ret = FindAndExecute(Name);
	std::string TmpStr;
	while (ActiveScreen.compare("") != 0) {
		TmpStr = ActiveScreen;//«ануление активного экрана, что бы не возник непредвиденный бесконечный цикл
		ActiveScreen = "";

		Ret = FindAndExecute(TmpStr);
	}

	return Ret;
	}
void  MenuManager::SetNextScreen(std::string Name) {
	ActiveScreen = Name;
	}

int  MenuManager::FindAndExecute(std::string Name){
	ScreenMenu *MenuToPerform;
	try {
		MenuToPerform = ScreenMap.at(Name).get();
	}
	catch(...)
	{
		return 0;
	}
	MenuToPerform->Perform();
}


LambdaScreenMenu::LambdaScreenMenu(){
	Lambda = []() -> int	{
		std::cout << "== ** == ERROR! Lambda menu screen created empty == ** ==\n";
		return 0;
	};
}

LambdaScreenMenu::LambdaScreenMenu(std::function<int()> _Lambda) {
	Lambda = _Lambda;
}

int LambdaScreenMenu::Perform() {
	return Lambda();
}