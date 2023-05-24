#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include "loading.h"
#include "assets.h"

Loading::Loading() {
	text.setFont(Assets::font);
	text.setCharacterSize(14);
	text.setFillColor(sf::Color::White);
	percent = 0;
}

void Loading::render(sf::RenderWindow& window) {
	sf::Vector2u s_window = window.getSize();
	sf::FloatRect v_rect(0, 0, s_window.x, s_window.y);
	window.setView(sf::View(v_rect));

	sf::Vector2u wsize = window.getSize();
	sf::Vector2f size = { wsize.x * 0.6f, wsize.y * 0.05f };
	sf::Vector2f pos = {
		(wsize.x - size.x) * 0.5f,
		(wsize.y - size.y) * 0.5f
	};

	sf::RectangleShape loading;
	loading.setPosition(pos);
	loading.setSize(size);
	loading.setFillColor(sf::Color(30, 30, 30, 255));
	window.draw(loading);

	size.x *= percent;
	loading.setSize(size);
	loading.setFillColor(sf::Color::White);
	window.draw(loading);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2) << percent * 100.0f << "%";
	text.setString(ss.str());

	sf::FloatRect tsize = text.getGlobalBounds();
	sf::Vector2f tpos = {
		(wsize.x - tsize.width) * 0.5f,
		(wsize.y - tsize.height) * 0.5f
	};
	text.setPosition(tpos);
	window.draw(text);
}
