#include "assets.hpp"
#include "error.hpp"

void Assets::init() {
	if(!font.loadFromFile("assets/Dosis-Light.ttf"))
		throw FontLoadError();
}

sf::Font Assets::font;