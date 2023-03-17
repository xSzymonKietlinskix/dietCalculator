#include <iostream>
#include "dataBase.hpp"
#include "imGui/imgui.h"
#include "imGUi/imgui-SFML.h"
//#include "products.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#define defWindowFlags ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse
using namespace std;

class flags {
public:
	bool menu;
	bool NewRecord;
	bool butDataBaseMenu;
	bool butAddNewRecord;
	bool showData;
	bool butConfirm;
	bool countDiet;
	bool numOfDays;
	void toggle(bool& stat);
	flags() : showData(false),butDataBaseMenu(false), butAddNewRecord(false), menu(true), NewRecord(false), butConfirm(false), countDiet(false), numOfDays(false){};
};

void flags::toggle(bool& stat) {
	if (stat == true)
		stat = false;
	else
		stat = true;
}


void dataBaseMenu(dataBase& dB, flags &fl) {
	if (ImGui::Button("Creat default table (if not exists)", ImVec2(600, 100)))
		dB.creatDefaultTable();
	if (ImGui::Button("Show data", ImVec2(600, 100)))
		fl.toggle(fl.showData);
	if (ImGui::Button("Count Rows", ImVec2(600, 100)))
		dB.countProf();
		
	if (fl.showData) {
		ImGui::SetNextWindowSize(ImVec2(1900, 900));
		ImGui::Begin("Data Base", &fl.showData, defWindowFlags);
		dB.showBase();
		if (ImGui::Button("Close", ImVec2(600, 100)))
			fl.toggle(fl.showData);
		ImGui::End();
	}
	if (ImGui::Button("Add new record", ImVec2(600, 100))) {
		fl.toggle(fl.butAddNewRecord);
		cout << "clicked" << endl;
	}
	if (fl.butAddNewRecord == true) {
		ImGui::SetNextWindowSize(ImVec2(1900, 900));
		ImGui::Begin("Add new record",&fl.butAddNewRecord, defWindowFlags);
		static char t[128] = "";
		static char n[128] = "";
		static char p[20] = "";
		static char c[20] = "";
		ImGui::InputText("type",t, sizeof(t));
		ImGui::InputText("name", n, sizeof(n));
		ImGui::InputText("price", p, sizeof(p));
		ImGui::InputText("cal", c, sizeof(c));
		if (ImGui::Button("Confirm", ImVec2(600, 100))) {
			cout << t << endl << n << endl << p << endl << c << endl;;
			fl.toggle(fl.butConfirm);
		}
		if (ImGui::Button("Cancel", ImVec2(600, 100)))
			fl.toggle(fl.butAddNewRecord);

		if (fl.butConfirm) {
			product pr(t, n, stof(c), stof(p));
			dB.addSingleRecord(pr);
			fl.toggle(fl.butConfirm);
			fl.butAddNewRecord = false;
		}
		ImGui::End();
	}
}

void countDiet(dataBase& dB, flags& fl) {
	ImGui::SetNextWindowSize(ImVec2(1900, 900));
	ImGui::Begin("Count Diet", &fl.countDiet, defWindowFlags);
	static int cal = 0;
	ImGui::InputInt("Calories", &cal, sizeof(cal));

	if (ImGui::Button("Number of days", ImVec2(600, 100)))
		fl.toggle(fl.numOfDays);

	if (fl.numOfDays) {
		ImGui::ListBoxHeader("");
		ImGui::ListBoxFooter();
	}
	if (ImGui::Button("Type of diet", ImVec2(600, 100))) {
		fl.toggle(fl.butAddNewRecord);
	}
	ImGui::End();
}

int main(void) {
	
	
	const char* location = "dataBase/newDataBase.db";
	dataBase dB(location);
	flags fl;

	sf::RenderWindow window(sf::VideoMode(2000, 1000), "Diet calculator");
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window,false);
	
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 24.0f);
	ImGui::SFML::UpdateFontTexture();

	ImGuiStyle* style = &ImGui::GetStyle();
	style->FramePadding = ImVec2(4, 2);
	//style->Colors[ImGuiCol_WindowBg] = ImVec4(255, 255, 0, 192);
	//style->Colors[ImGuiCol_Button] = ImVec4(255, 0, 0, 192);
	
	style->WindowRounding = 0.40;

	
	
	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		ImGui::SFML::Update(window, deltaClock.restart());
		
		//***menu window***//
		//**left column**//
		if (fl.menu) {
		
			ImGui::SetNextWindowSize(ImVec2(1900, 900));
			ImGui::Begin(" ", &fl.menu, defWindowFlags);
		
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 600);

			if (ImGui::Button("Data base menu",ImVec2(600,100)))
				fl.toggle(fl.butDataBaseMenu);
			if (ImGui::Button("Count diet", ImVec2(600, 100))) 
				fl.toggle(fl.countDiet);
			if (ImGui::Button("Exit", ImVec2(600, 100))) 
				return 0;
			
			
		}
		
		//**right column**//

		ImGui::NextColumn();
		if (fl.butDataBaseMenu)
			dataBaseMenu(dB, fl);
		if (fl.countDiet)
			countDiet(dB, fl);
		ImGui::End();
		
		
		window.clear();
		ImGui::SFML::Render(window);
		window.display();
		
	}

	ImGui::SFML::Shutdown();

	return 0;
}