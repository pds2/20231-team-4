#include <sstream>
#include <iostream>
#include "loading.h"
#include "menu.h"
using namespace std;

int main() {
	Context ctx = {
		sf::RenderWindow(sf::VideoMode(800, 600), "Hello, World!")
	};

	StateManager manager(ctx, std::make_unique<MainMenu>(ctx));

	sf::Clock clock;
	f64 elapsed = 0;
	while(ctx.window.isOpen()) {
		elapsed += clock.getElapsedTime().asMilliseconds();
		if(elapsed > 50) manager.tick(), elapsed = 0;
		manager.render();
	}
}