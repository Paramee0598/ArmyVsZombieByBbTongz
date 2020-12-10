#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>
#include <sstream>
#include "ZombieShoot.cpp"
#include "ZombieShoot.h"

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

class Textbox
{
public:
	Textbox(int size, sf::Color color, bool sel, sf::Font& font, sf::Vector2f position);

	void setSelected(bool sel);
	void Draw(sf::RenderWindow& window);
	void typedOn(sf::Event input);
	void Update(sf::Vector2i mousePos);
	void resetString();

	bool isActiveDelayButton() { return activeDelayButton; }

	std::string getText() { return text.str(); }
	std::string getName() { return name; }

private:
	sf::Text textbox;
	sf::RectangleShape body;
	std::string name;
	std::ostringstream text;

	
	bool inputText ;
	bool isSelectedBool;
	bool hasLimit;
	bool activeDelayButton;
	int limit;

	void inputLogic(int charTyped);
	void deleteLastChar();
};

