#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include <deque>

class Oras
{
private:
	long long int populatie;
	long long int infectati;
	long long int decedati;
	long long int vindecati;
   
	std::deque<long long int> cotaZilnic;
	std::string denumire;
	
public:
	float pX, pY;//pozitia orasului pe harta
	int trimit;//nr de fps-uri in care se trimit calatori
	sf::Sprite orasSprite;//orasul
	mutable sf::RectangleShape shape;

	static long long populatii[101];

	Oras(std::string denumire,long long int populatie,float pozX,float pozY);
	~Oras() noexcept;

	
	sf::Texture textura;
	//gettere
	long long getPopulatie() {
		return populatie;
	}
	long long getInfectati(){
		return infectati;
	}
	long long getDeced(){
		return decedati;
	}
	long long getVindecati(){
		return vindecati;
	}
	std::string getDenum(){
		return denumire;
	}
	std::deque<long long int> getNumVindZilnic(){
		return cotaZilnic;
	}


	//settere
	void setInfectati(long long int modif){
		infectati = modif;
	}
	void setDeced(long long int modif){
		decedati = modif;
	}
	void setVindec(long long int modif){
		vindecati = modif;
	}
	void setCota(std::deque<long long int> modif){
		cotaZilnic = modif;
	}
	void setPopulatie(long long pop) {
		this->populatie=pop;
	}
	
	
	void sortarePopulatie(long long pop[101], int limita);

	void update();
	void logicOras();
};

