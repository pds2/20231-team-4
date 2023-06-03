#include "assets.hpp"

sf::Font loadFont() {
	sf::Font font;
	font.loadFromFile("assets/Dosis-Light.ttf");
	return font;
}
sf::Font Assets::font = loadFont();