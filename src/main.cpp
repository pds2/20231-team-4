#include <sstream>
#include <iostream>
#include "loading.h"
#include "menu.h"
using namespace std;

int main() {
	Context ctx = {
		sf::RenderWindow(sf::VideoMode(800, 600), "Semester's End (v.1)")
	};

	sf::Image icon;
	icon.loadFromFile("assets/game_icon.png");
	ctx.window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	ctx.window.setFramerateLimit(60);
	ctx.window.setVerticalSyncEnabled(true);

	StateManager manager(ctx, std::make_unique<MainMenu>(ctx));

	sf::Clock clock;
	while(ctx.window.isOpen()) {
		manager.tick();
		manager.render();
	}
}