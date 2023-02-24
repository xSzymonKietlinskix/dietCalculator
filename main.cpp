#include <iostream>
#include "dataBase.hpp"
#include "include/imgui.h"
#include "include/imgui-SFML.h"

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

using namespace std;

int main(void) {
	const char* location = "dataBase/newDataBase.db";
	dataBase mydata(location);
	
	

    sf::RenderWindow window(sf::VideoMode(1280, 720), "Diet calculator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

   

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
        if (ImGui::Button("Creat new table")) {
            mydata.creatTable();
        }
        if (ImGui::Button("Add new record"))
        {
            mydata.addSingleRecord();
        }
        ImGui::End();

        window.clear();
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    

	return 0;
}