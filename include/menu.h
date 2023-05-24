#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <functional>
#include "state.h"

struct Button {
	std::string text;
	std::function<void()> click;
};

class Buttons: public State {
	private:
	std::vector<Button> buttons;
	std::vector<sf::Text> texts;
	i32 selected;

	void moveMouse(sf::Vector2i position);
	void moveKeys(i32 delta);

	protected:
	void update();

	public:
	Buttons(std::initializer_list<Button> buttons);
	~Buttons();
	void tick();
	void render(sf::RenderWindow& window);
	void handleEvent(sf::Event event);
};

class MainMenu: public Buttons {
	private:
	void handleEvent(sf::Event event);

	public:
	MainMenu();
};

#endif
