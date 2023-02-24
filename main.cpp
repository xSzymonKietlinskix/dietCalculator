#include <iostream>
#include "dataBase.hpp"
#include "imGui/imgui.h"
#include "imGUi/imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

using namespace std;

void dataBaseMenu(dataBase& dB, bool &isClicked) {

	if (ImGui::Button("Creat new table"))
		dB.creatDefaultTable();
	if (ImGui::Button("Add new record")) {
		isClicked = true;
		ImGui::End();
	}
	cout << isClicked;
	if (isClicked) {
		
		ImGui::Begin("test");
		if (ImGui::Button("Creat new table"))
			cout << "works";
		ImGui::End();
	}
}

int main(void) {
	const char* location = "dataBase/newDataBase.db";
	dataBase dB(location);
	bool isClicked = false;


	sf::RenderWindow window(sf::VideoMode(1280, 720), "Diet calculator");
	window.setFramerateLimit(60);

	ImGui::SFML::Init(window);

	enum menuOptions { none, dataBaseOptions };

	sf::Clock deltaClock;
	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(window, event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		ImGui::SFML::Update(window, deltaClock.restart());
		ImGui::Begin("Menu");
		ImGui::Columns(2);
		ImGui::SetColumnOffset(1, 230);
		int choosenButton = none;
		bool isSelceted;
		ImGui::Checkbox("Data Base options", &isSelceted);
		if (isSelceted == true)
			choosenButton = dataBaseOptions;
		else
			choosenButton = none;
		ImGui::NextColumn();
		switch (choosenButton) {
		case none:
			break;
		case dataBaseOptions:
			dataBaseMenu(dB, isClicked);
			break;
		default:
			break;

		}


		window.clear();
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();


	return 0;
}