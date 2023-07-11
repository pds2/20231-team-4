#include "assets.hpp"
#include "error.hpp"

void Assets::init() {
	if(!font.loadFromFile("assets/Dosis-Light.ttf"))
		throw FontLoadError();
	if(!terminal.loadFromFile("assets/Terminal.ttf"))
		throw FontLoadError();
	if(!buttonBackground.loadFromFile("assets/GUI/PNG/Main menu/Button BG.png"))
		throw ImageLoadError();
}

sf::Font Assets::font;
sf::Font Assets::terminal;
sf::Texture Assets::buttonBackground;