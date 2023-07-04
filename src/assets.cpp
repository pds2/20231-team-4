#include "assets.hpp"
#include "error.hpp"

void Assets::init() {
	if(!font.loadFromFile("assets/Dosis-Light.ttf"))
		throw FontLoadError();
	if(!bugol.loadFromFile("assets/bugol.png"))
		throw ImageLoadError();
}

sf::Font Assets::font;
sf::Texture Assets::bugol;