#pragma once
#include <iostream>
#include <fstream>



//sfml
#include <include/SFML/Graphics.hpp>
#include <chrono>
#include <thread>
#include <cmath>
#include <map>

//header
#include "HeaderSimulari.h"
#include "HeaderVirus.h"
#include "Buton.h"
#include "TextBox.h"

//.

int simulare1();


namespace start {
	/*void logo() {
		//functie de logo ca sa fie frumos
		cout << R"(
 ____                             __
/\  _`\                          /\ \                          __
\ \ \L\ \       __       ___     \_\ \      __     ___ ___    /\_\     ___
 \ \ ,  /     /'__`\   /' _ `\   /'_` \   /'__`\ /' __` __`\  \/\ \   /'___\
  \ \ \\ \   /\ \L\.\_ /\ \/\ \ /\ \L\ \ /\  __/ /\ \/\ \/\ \  \ \ \ /\ \__/
   \ \_\ \_\ \ \__/.\_\\ \_\ \_\\ \___,_\\ \____\\ \_\ \_\ \_\  \ \_\\ \____\
	\/_/\/ /  \/__/\/_/ \/_/\/_/ \/__,_ / \/____/ \/_/\/_/\/_/   \/_/ \/____/
_____________________________________________________________________________
Pentru a vedea detalii despre o persoana da CLICK pe aceasta.
Pentru a opri simularea apasa ESC.
Pentru a reseta simularea apasa R.


Introdu in ordine infectabilitatea bolii(default 8), timpul de recuperare al bolii(in frameuri_default 1000)
si mortalitatea(in procente_default 20):

)";
	}*/

	bool pauza;
	void pause() { pauza = true; }
	void resume() { pauza = false; }
	void reset() {

	}
	sf::Text textSimulare1;//text simulare
	sf::Clock ceas;
	float dt;//delta time

	float deltaTime() {

		return ceas.restart().asSeconds();//pt miscare in functie de frameuri

	}

	const float nrFPS = FPS;
	int nrNpcSource;
	int infectabilitateSource;
	int mortalitateSource;
	
}

using namespace std;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds


int main() {

input:
	srand(time(0));
	start::pauza = false;
	
	//logo();
	//cin >> infectabilitate >> timpRecuperare >> mortalitate;
	//cout << "Introdu numarul de persoane:";
	/*infectabilitate = 8;
	timpRecuperare = 1000;
	mortalitate = 20;
	//cin>> nrNpcuri;
	nrNpcuri = 100;
	counterVii = nrNpcuri;

	bool amClick = false;*/



window:
	//aici bagam window-ul
	sf::RenderWindow window(sf::VideoMode(1280, 720), "Randemic", sf::Style::Default);
	window.setFramerateLimit(start::nrFPS);


background:
	//fundalul
	sf::Texture backTexture;
	backTexture.loadFromFile("Sprites/fundalStart.png");
	sf::Sprite backSprite(backTexture);
	window.clear();
	window.draw(backSprite);

font_text:
	sf::Font font;
	font.loadFromFile("Fonts/KarmaFuture.ttf");

butoane:
	Buton butonStart(0,"", "Sprites/butonStart.png", 920, 390);
	Buton butonScenariuStanga(-1, "", "Sprites/selectStanga.png", 390, 200);
	Buton butonScenariuDreapta(0, "", "Sprites/selectDreapta.png", 805, 200);

textBoxuri:
	Buton butonText1(-5, "Numar Oameni",		"Sprites/textBox.png", 100, 480);
	Buton butonText2(-5, "Infectabilitate %",	"Sprites/textBox.png", 300, 480);
	Buton butonText3(-5, "Mortalitate %",		"Sprites/textBox.png", 500, 480);
	Buton butonText4(-5, "Timp de recuperare",	"Sprites/textBox.png", 100, 600);

	sf::Text textBox1;	textBox1.setPosition(120, 495);	textBox1.setColor(sf::Color::Black); textBox1.setFont(font);
	sf::Text textBox2;	textBox2.setPosition(320, 495);	textBox2.setColor(sf::Color::Black); textBox2.setFont(font);
	sf::Text textBox3;	textBox3.setPosition(520, 495);	textBox3.setColor(sf::Color::Black); textBox3.setFont(font);
	sf::Text textBox4;	textBox4.setPosition(120, 615);	textBox4.setColor(sf::Color::Black); textBox4.setFont(font);


	butonText1.labelText.setCharacterSize(20);	butonText1.labelText.setFont(font);	butonText1.labelText.setColor(sf::Color::Black);	butonText1.labelText.setPosition(butonText1.pozx, butonText1.pozy - 25);
	butonText2.labelText.setCharacterSize(20);	butonText2.labelText.setFont(font);	butonText2.labelText.setColor(sf::Color::Black);	butonText2.labelText.setPosition(butonText2.pozx, butonText2.pozy - 25);
	butonText3.labelText.setCharacterSize(20);	butonText3.labelText.setFont(font);	butonText3.labelText.setColor(sf::Color::Black);	butonText3.labelText.setPosition(butonText3.pozx, butonText3.pozy - 25);
	butonText4.labelText.setCharacterSize(20);	butonText4.labelText.setFont(font);	butonText4.labelText.setColor(sf::Color::Black);	butonText4.labelText.setPosition(butonText4.pozx, butonText4.pozy - 25);
	

display:
	window.display();

	//aici incepe nebunia, dar tot Strafer e mai misto #quierres?
	bool simOpen;
	simOpen = false;


test:
	


	start::deltaTime();
	while (window.isOpen())
	{

		if (simOpen == true) {
			window.close();
		}

		sf::Event event;
		while (window.pollEvent(event))
		{
			//eventuri

			switch (event.type) {
			case sf::Event::KeyPressed: {
				if (event.key.code == sf::Keyboard::Escape)
				{
					window.close();
				}
				else if (event.key.code == sf::Keyboard::F1)
				{
					start::reset();
					goto input;

				}

				break;
			}
			case sf::Event::Closed: {
				window.close();
				break;
			}
			case sf::Event::LostFocus: {
				start::pause();
				break;
			}
			case sf::Event::GainedFocus: {
				start::resume();
				break;
			}

									   /*default: {
										   //ca sa poti sa dai click pe oamenii astia simulati
										   if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
										   {
											   window.close();
											   simulare1();
											   return 0;
										   }
										   break;


									   }*/


			}

		}
		if (start::pauza == false) {
			//ruleaza
			window.clear();

			//fundalul
			window.draw(backSprite);
			start::dt = start::deltaTime();
			//fundalul



			//butoane

			window.draw(butonStart.butonSprite);
			if (butonStart.apasat == true) {
				window.close();
			}
			butonStart.clic(event);
			butonStart.update(sf::Vector2f(sf::Mouse::getPosition(window)));


			window.draw(butonScenariuStanga.butonSprite);
			butonScenariuStanga.clic(event);
			butonScenariuStanga.update(sf::Vector2f(sf::Mouse::getPosition(window)));

			window.draw(butonScenariuDreapta.butonSprite);
			butonScenariuDreapta.clic(event);
			butonScenariuDreapta.update(sf::Vector2f(sf::Mouse::getPosition(window)));

			int localScenariuCount = butonScenariuDreapta.counter + butonScenariuStanga.counter;

			if (localScenariuCount < 1) {
				localScenariuCount = 1;
			}
			if (localScenariuCount > 2) {
				localScenariuCount = 2;
			}

			butonStart.tipButon = localScenariuCount;

			switch (localScenariuCount) {
			case 1: {//niste convertiri super ciudatele ca nu mergea stoi()
				std::string nN = butonText1.getText();
				stringstream strnN(nN);
				//nrNpc= std::stoi(nN);
				strnN>>nrNpc;

				std::string inf = butonText2.getText();
				stringstream strinf(inf);
				strinf>>infectabilitate;

				std::string mrt = butonText3.getText();
				stringstream strmrt(mrt);
				strmrt>>mortalitate;

				std::string rec = butonText4.getText();
				stringstream strrec(rec);
				strrec >>timpRecuperare;
				
				break;
			}

			case 2: {
			//pt oras
			}

			}

			


			butonText1.renderTextBox(window, event);	window.draw(butonText1.labelText);
			butonText2.renderTextBox(window, event);	window.draw(butonText2.labelText);
			butonText3.renderTextBox(window, event);	window.draw(butonText3.labelText);
			butonText4.renderTextBox(window, event);	window.draw(butonText4.labelText);

			textBox1.setString(butonText1.getText());	window.draw(textBox1);
			textBox2.setString(butonText2.getText());	window.draw(textBox2);
			textBox3.setString(butonText3.getText());	window.draw(textBox3);
			textBox4.setString(butonText4.getText());	window.draw(textBox4);
			//butoane

			window.display();



		}



	}
	return 0;
}