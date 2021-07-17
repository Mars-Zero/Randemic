#include <iostream>
#include <fstream>



//sfml
#include <include/SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <cmath>
#include <map>

//header
#include "Oras.h"
#include "HeaderSimulari.h"
#include "CalatorOras.h"
#include <msxml.h>


namespace simOras {
	bool pauza;
	void pause() { pauza = true; }
	void resume() { pauza = false; }
	int counterMorti = 0;
	int counterVii;
	int counterInfectati = 0;
	int counterVindecati;
	void reset() {
		//chestii care se reseteaza
		counterInfectati = 0;
		counterMorti = 0;
		counterVii = 0;
		counterVindecati = 0;

	}
	sf::Text textNpc;//textul pt fiecare NPC
	sf::Text textCounter;//numarul text
	sf::Clock ceas;
	float dt;//delta time
	const float nrFPS = FPS;

	float deltaTime() {

		return ceas.restart().asSeconds();//pt miscare in functie de frameuri

	}



}

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

#pragma warning(suppress : 4996)

std::map<string, Oras> infect;
std::map<string, Oras> sigur;
std::map<string, Oras> vindec;

std::map<string, CalatorOras>calatori;

int initEcranPrincipal();
std::map<std::string, Oras> generareOrase(int nrO, int nrN);
const float PI = 3.14159265359f;

#pragma once

void drawOras(sf::RenderWindow& window)
{
	for (std::map<string, Oras>::iterator itr = sigur.begin(); itr != sigur.end(); itr++) {
		Oras om = itr->second;
		window.draw(itr->second.shape);
	}
	for (std::map<string, Oras>::iterator itr = infect.begin(); itr != infect.end(); itr++) {
		Oras om = itr->second;
		window.draw(itr->second.shape);
	}
	for (std::map<string, Oras>::iterator itr = vindec.begin(); itr != vindec.end(); itr++) {
		Oras om = itr->second;
		window.draw(itr->second.shape);
	}

}

float computeAngle(float ax, float ay, float bx, float by) {
	return atan2((by - ay), (bx - ax));
};

void trimitCalator(Oras oras)
{
	//aici calculez distanta dintre orase
	std::map<string, Oras>::iterator item = sigur.begin();
	int val = rand() % sigur.size();
	std::advance(item, val);

	//in item este orasul pe care vrem sa il cotropim
	std::map<string, CalatorOras>::iterator itr = calatori.find(item->second.getDenum());
	if (calatori.find(item->second.getDenum()) != calatori.end())
	{
		//nu mai este niciun calator spre orasul asta

		sf::Vector2f dir = sf::Vector2f(item->second.pX, item->second.pY);
		CalatorOras calator = CalatorOras(oras.pX, oras.pY, dir, item);
		calatori.insert({ item->second.getDenum() ,calator });
		return;
	}
	//trimitCalator(oras);
	return;
}

void miscareCalatori()
{
	for (std::map<string, CalatorOras>::iterator itr = calatori.begin(); itr != calatori.end(); itr++) {
		itr->second.move(simOras::dt);

		float X1, Y1, X2, Y2;
		sf::Transform matrix = itr->second.tinta->second.shape.getTransform();
		for (int i = 0; i < itr->second.tinta->second.shape.getPointCount(); ++i) {
			const auto pointInModelSpace = matrix.transformPoint(itr->second.tinta->second.shape.getPoint(i));
			switch (i) {
			case 0: {
				X1 = pointInModelSpace.x;
				break;
			}
			case 1: {
				Y1 = pointInModelSpace.y;
				break;

			}
			case 2: {
				X2 = pointInModelSpace.x;
				break;
			}
			case 3: {
				Y2 = pointInModelSpace.y;
				break;

			}
			}
		}
		if (itr->second.isTouchingOras(itr->second.razaShape, X1, Y1, X2, Y2))
		{
			std::string key = itr->second.tinta->first;
			Oras oras = itr->second.tinta->second;
			sigur.erase(key);

			oras.shape.setFillColor(sf::Color(255, 122, 0));
			oras.setInfectati(oras.getInfectati() + 1);

			std::deque<long long> nouaCota;
			nouaCota.push_back(1);
			oras.setCota(nouaCota);

			infect.insert({ key,oras });

		}
	}
}

void drawCalatori(sf::RenderWindow& window)
{
	for (std::map<string, CalatorOras>::iterator itr = calatori.begin(); itr != calatori.end(); itr++) {
		CalatorOras om = itr->second;
		window.draw(itr->second.shape);
	}
}

int simulareOras() {

input:
	srand(time(0));
	simOras::pauza = true;


	sigur.clear();
	infect.clear();
	vindec.clear();

	Oras orastest("craiova", 100, 300, 300);
	orastest.shape.setSize(sf::Vector2f(60, 60));
	orastest.shape.setFillColor(sf::Color(0, 255, 0, 100));
	//orastest.shape.setPosition(300, 300);
	sigur.insert({ "1",orastest });

	Oras orastest2("galati", 100, 600.0, 300.0);
	orastest2.shape.setSize(sf::Vector2f(60, 60));
	orastest2.shape.setFillColor(sf::Color(0, 255, 0, 100));
	//orastest2.shape.setPosition(600, 300);
	sigur.insert({ "2",orastest2 });
	//sigur = generareOrase(nrNpc, nrOrase);

	int tipOrasClick = 0;//0 neinfectat 1 infectat  2 vindecat
	bool amClick = false;
	std::map<string, Oras>::iterator lastClick;


window:
	//aici bagam window-ul
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Randemic", sf::Style::Default);
	window.setFramerateLimit(simOras::nrFPS);


background:
	//fundalul
	sf::Texture backTexture;
	backTexture.loadFromFile("Sprites/fundal.png");
	sf::Sprite backSprite(backTexture);
	window.clear();
	window.draw(backSprite);


font_text:
	sf::Font font;
	font.loadFromFile("Fonts/KarmaFuture.ttf");


	simOras::textNpc.setFont(font);
	simOras::textCounter.setFont(font);


	simOras::textNpc.setCharacterSize(20);
	simOras::textCounter.setCharacterSize(20);

	simOras::textNpc.setFillColor(sf::Color::Black);
	simOras::textCounter.setFillColor(sf::Color::Black);

	simOras::textNpc.setStyle(sf::Text::Regular);
	simOras::textCounter.setStyle(sf::Text::Regular);

	simOras::textNpc.setPosition(995, 95);
	simOras::textCounter.setPosition(995, 300);

	// inside the main loop, between window.clear() and window.display()
	window.draw(simOras::textNpc);
	window.draw(simOras::textCounter);


adaugOmase:
	//generam 


	//desenam orasele
	for (std::map<string, Oras>::iterator itr = infect.begin(); itr != infect.end(); itr++) {

		Oras oras = itr->second;
		oras.shape.setFillColor(sf::Color(255, 0, 0, 110));
		window.draw(oras.shape);
	}

	for (std::map<string, Oras>::iterator itr = sigur.begin(); itr != sigur.end(); itr++) {

		Oras oras = itr->second;
		oras.shape.setFillColor(sf::Color(0, 255, 0, 110));
		window.draw(oras.shape);
	}
	for (std::map<string, Oras>::iterator itr = vindec.begin(); itr != vindec.end(); itr++) {

		Oras oras = itr->second;
		oras.shape.setFillColor(sf::Color(0, 0, 255, 110));
		window.draw(oras.shape);
	}



display:
	window.display();

	//aici incepe nebunia dar tot Strafer e mai misto #quierres?
	simOras::deltaTime();


	int frames = 0;
	while (window.isOpen())
	{

		sf::Event event;
		window.clear();
		window.draw(backSprite);//fundalul

		while (window.pollEvent(event))
		{
			//eventuri

			switch (event.type) {
			case sf::Event::KeyPressed: {
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				else if (event.key.code == sf::Keyboard::R)
				{
					simOras::reset();
					goto input;

				}
				else if (event.key.code == sf::Keyboard::T)
				{
					window.close();
					initEcranPrincipal();

				}
				else if (event.key.code == sf::Keyboard::Enter)
				{
					simOras::pauza = !simOras::pauza;

				}

				break;
			}
			case sf::Event::Closed: {
				window.close();
				break;
			}

			default: {
				//ca sa poti sa dai click pe orase

				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				{
					bool amOras = false;
					tipOrasClick = -1;

					for (std::map<string, Oras>::iterator itr = sigur.begin(); itr != sigur.end(); itr++) {

						auto mouse_pos = sf::Mouse::getPosition(window);
						auto translated_pos = window.mapPixelToCoords(mouse_pos);
						Oras oras = itr->second;

						if (oras.shape.getGlobalBounds().contains(translated_pos)) {
							amClick = true;
							lastClick = itr;
							tipOrasClick = 1;
							amOras = true;
							break;
						}
					}
					if (amOras == true && simOras::pauza == true)
					{
						//adaug infectat
						std::string key = lastClick->first;
						Oras oras = lastClick->second;
						sigur.erase(key);

						oras.shape.setFillColor(sf::Color(255, 122, 0));
						oras.setInfectati(oras.getInfectati() + 1);

						std::deque<long long> nouaCota;
						nouaCota.push_back(1);
						oras.setCota(nouaCota);

						infect.insert({ key,oras });
						lastClick = infect.find(key);
						tipOrasClick = 1;

						break;
					}
					for (std::map<string, Oras>::iterator itr = infect.begin(); itr != infect.end(); itr++) {

						auto mouse_pos = sf::Mouse::getPosition(window);
						auto translated_pos = window.mapPixelToCoords(mouse_pos);
						Oras oras = itr->second;

						if (oras.shape.getGlobalBounds().contains(translated_pos)) {
							amClick = true;
							lastClick = itr;
							tipOrasClick = 1;
							amOras = true;
							break;
						}
					}
					if (amOras == true && simOras::pauza == true)
					{
						//adaug infectat
						lastClick->second.setInfectati(lastClick->second.getInfectati() + 1);
						std::deque<long long> nouaCota = lastClick->second.getNumVindZilnic();
						nouaCota.push_back(1);
						lastClick->second.setCota(nouaCota);
						break;
					}
					for (std::map<string, Oras>::iterator itr = vindec.begin(); itr != vindec.end(); itr++) {

						auto mouse_pos = sf::Mouse::getPosition(window);
						auto translated_pos = window.mapPixelToCoords(mouse_pos);
						Oras oras = itr->second;

						if (oras.shape.getGlobalBounds().contains(translated_pos)) {
							amClick = true;
							lastClick = itr;
							tipOrasClick = 2;
							amOras = true;
							break;
						}
					}
					if (amOras == true && simOras::pauza == true)
					{
						//adaug un infectat
						lastClick->second.setInfectati(lastClick->second.getInfectati() + 1);
						std::deque<long long> nouaCota = lastClick->second.getNumVindZilnic();
						nouaCota.push_back(1);
						lastClick->second.setCota(nouaCota);
						break;
					}
					break;
				}
				else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				{
					bool amOras = false;
					tipOrasClick = -1;

					for (std::map<string, Oras>::iterator itr = sigur.begin(); itr != sigur.end(); itr++) {

						auto mouse_pos = sf::Mouse::getPosition(window);
						auto translated_pos = window.mapPixelToCoords(mouse_pos);
						Oras oras = itr->second;

						if (oras.shape.getGlobalBounds().contains(translated_pos)) {
							amClick = true;
							lastClick = itr;
							tipOrasClick = 1;
							amOras = true;
							break;
						}
					}
					if (amOras == true && simOras::pauza == true)
					{
						//nu fac nimic
						break;
					}
					for (std::map<string, Oras>::iterator itr = infect.begin(); itr != infect.end(); itr++) {

						auto mouse_pos = sf::Mouse::getPosition(window);
						auto translated_pos = window.mapPixelToCoords(mouse_pos);
						Oras oras = itr->second;

						if (oras.shape.getGlobalBounds().contains(translated_pos)) {
							amClick = true;
							lastClick = itr;
							tipOrasClick = 1;
							amOras = true;
							break;
						}
					}
					if (amOras == true && simOras::pauza == true)
					{
						//sterg infectat
						lastClick->second.setInfectati(lastClick->second.getInfectati() - 1);
						std::deque<long long> nouaCota = lastClick->second.getNumVindZilnic();
						long long val = nouaCota.at(nouaCota.size() - 1);
						nouaCota.pop_back();
						nouaCota.push_back(val);
						lastClick->second.setCota(nouaCota);

						if (lastClick->second.getInfectati() == 0) {
							//trec in alta categorie
							std::string key = lastClick->first;
							Oras oras = lastClick->second;
							infect.erase(lastClick);
							if (oras.getPopulatie() / 2 < oras.getVindecati())
							{
								//am mai multi vindecati decat susceptibili
								oras.shape.setFillColor(sf::Color(0, 0, 255));
								vindec.insert({ key,oras });
								lastClick = vindec.find(key);

								tipOrasClick = 2;
							}
							else {
								//inca e susceptibil
								oras.shape.setFillColor(sf::Color(0, 255, 0));
								sigur.insert({ key,oras });
								lastClick = sigur.find(key);

								tipOrasClick = 0;
							}
						}

						break;
					}
					for (std::map<string, Oras>::iterator itr = vindec.begin(); itr != vindec.end(); itr++) {

						auto mouse_pos = sf::Mouse::getPosition(window);
						auto translated_pos = window.mapPixelToCoords(mouse_pos);
						Oras oras = itr->second;

						if (oras.shape.getGlobalBounds().contains(translated_pos)) {
							amClick = true;
							lastClick = itr;
							tipOrasClick = 2;
							amOras = true;
							break;
						}
					}
					if (amOras == true && simOras::pauza == true)
					{
						//nu sterg nimic

						break;
					}

				}
				break;

			}


			}




		}
		if (simOras::pauza == false) {
			//ruleaza

			simOras::dt = simOras::deltaTime();

			//merg in fiecare oras si ii dau update
			for (std::map<string, Oras>::iterator itr = infect.begin(); itr != infect.end(); itr++) {
				Oras oras = itr->second;
				if (frames == 0)
				{
					oras.update();
					//trimitCalator(oras);
					
					
					std::map<string, Oras>::iterator item = sigur.begin();
					sf::Vector2f origin = sf::Vector2f(oras.pX, oras.pY);

					//Calculate the direction vector
					sf::Vector2f dirVec = sf::Vector2f(item->second.pX - origin.x, item->second.pY - origin.y);

					//Calculate the length^2
					float magSquare = std::sqrt((dirVec.x * dirVec.x) + (dirVec.y * dirVec.y));

					//Change the mag to 1 (you dont need the y for getting the angle
					dirVec.x = (dirVec.x) / magSquare;

					//Get the angle and change it to deg (SFML need deg)
					float rotAngle = std::acos(dirVec.x) * (180 / PI);

					//float angle = computeAngle(oras.pX, oras.pY, item->second.pX, item->second.pY);

                    sf::Vector2f dir;
					dir.y = sin( rotAngle) * 100;
					dir.x = cos(rotAngle) * 100;
					/*//normalize(dir);*/
					
					CalatorOras calator = CalatorOras(oras.pX, oras.pY, dir, item);
					calatori.insert({ item->second.getDenum() ,calator });
					if (calatori.find(item->second.getDenum()) != calatori.end())
					{
						//nu mai este niciun calator spre orasul asta
					   
						//return;
					}
					drawCalatori(window);

					if (itr->second.trimit > 0)
					{
						
						itr->second.trimit = 0;
					}

					itr->second.trimit++;
				}

				itr->second = oras;
			}
			miscareCalatori();
		}
		drawCalatori(window);
		drawOras(window);
		if (amClick == true) {
			//am dat click pe cineva

			simOras::textNpc.setString((lastClick->second).getDenum() + "\n Populatie: " + to_string((lastClick->second).getPopulatie())
				+ "\n Infectati: " + to_string((lastClick->second).getInfectati())
				+ "\n Vindecati: " + to_string((lastClick->second).getVindecati())
				+ "\n Decedati: " + to_string((lastClick->second).getDeced()));
		}
		window.draw(simOras::textNpc);

		window.display();
		frames++;
		if (frames >= FPS)
		{
			frames = 0;
		}
	}
	return 0;
}