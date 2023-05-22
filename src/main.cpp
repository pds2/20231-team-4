#include <SFML/Graphics.hpp>
#include <sstream>
#include <iostream>
using namespace std;
int main() {
	sf::RenderWindow w(sf::VideoMode(800, 600), "Hello, world");
	w.setVerticalSyncEnabled(true);

	sf::Clock c;
	float interval = 0;

    sf::Font dosis;
    dosis.loadFromFile("assets/Dosis-Light.ttf");

	sf::Text fps;
	fps.setFont(dosis);
	fps.setPosition(10, 10);
	fps.setCharacterSize(14);
	fps.setFillColor(sf::Color::White);
	while(w.isOpen()) {
		sf::Event e;
		while(w.pollEvent(e))
			if(e.type == sf::Event::Closed) w.close();

		interval = interval * 0.9
			+ c.getElapsedTime().asMilliseconds() * 0.1;
		c.restart();

		stringstream ss;
		ss << (1000 / interval) << " FPS" << endl;
		fps.setString(ss.str());

		w.clear(sf::Color::Black);
		w.draw(fps);
		w.display();
	}
}