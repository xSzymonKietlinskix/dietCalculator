#include <iostream>
#include "dataBase.hpp"
#include "imGui/imgui.h"
#include "imGUi/imgui-SFML.h"
//#include "products.hpp"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>


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
	void toggle(bool& stat);
	flags() : showData(false),butDataBaseMenu(false), butAddNewRecord(false), menu(true), NewRecord(false), butConfirm(false), countDiet(false){};
};

void flags::toggle(bool& stat) {
	if (stat == true)
		stat = false;
	else
		stat = true;
}


void dataBaseMenu(dataBase& dB, flags &fl) {
	if (ImGui::Button("Creat new table", ImVec2(600, 100)))
		dB.creatDefaultTable();
	if (ImGui::Button("Show data", ImVec2(600, 100)))
		dB.showBase();
	if (ImGui::Button("Add new record", ImVec2(600, 100))) {
		fl.toggle(fl.butAddNewRecord);
		cout << "clicked" << endl;
	}
	if (fl.butAddNewRecord == true) {
		ImGui::Begin("Add new record",&fl.butAddNewRecord, ImGuiWindowFlags_NoMove);
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
		if (fl.butConfirm) {
			product pr(t, n, stof(c), stof(p));
			dB.addSingleRecord(pr);
			fl.toggle(fl.butConfirm);
		}
		ImGui::End();
	}
}

void countDiet(dataBase& dB, flags& fl) {
	;
}

int main(void) {
	const char* location = "dataBase/newDataBase.db";
	dataBase dB(location);
	flags fl;

	sf::RenderWindow window(sf::VideoMode(2000, 1000), "Diet calculator");
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window);
	ImGuiStyle* style = &ImGui::GetStyle();
	style->FramePadding = ImVec2(4, 2);
	//style->Colors[ImGuiCol_WindowBg] = ImVec4(255, 255, 0, 192);
	//style->Colors[ImGuiCol_Button] = ImVec4(255, 0, 0, 192);
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontDefault();
	ImFont * defFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 18.5f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(defFont != NULL);
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

			ImGui::Begin("Menu", &fl.menu, ImGuiWindowFlags_NoMove);
			ImGui::Columns(2);
			ImGui::SetColumnOffset(1, 630);

			//ImGui::PushFont(defFont);
			if (ImGui::Button("Data base menu",ImVec2(600,100)))
				fl.toggle(fl.butDataBaseMenu);
			if (ImGui::Button("Count diet", ImVec2(600, 100))) {
				fl.toggle(fl.countDiet);
			}
			//ImGui::PopFont();
		}
		//ImGui::PopFont();
		
		//**right column**//

		ImGui::NextColumn();
		if (fl.butDataBaseMenu)
			dataBaseMenu(dB, fl);
		if (fl.countDiet)
			countDiet(dB, fl);


		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();


	return 0;
}