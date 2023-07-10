#include <sstream>
#include <iostream>
#include "loading.hpp"
#include "menu.hpp"
using namespace std;

int main() {
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();
	mode.width *= 0.8, mode.height *= 0.8;
	Context ctx = {

		sf::RenderWindow(mode, "Semester's End (v.1)"),
		b2Vec2(0,0)
	};

	sf::Image icon;
	icon.loadFromFile("assets/game_icon.png");
	ctx.window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

	ctx.window.setFramerateLimit(60);
	ctx.window.setVerticalSyncEnabled(true);

	MyContactListener contactListener;
	ctx.world.SetContactListener(&contactListener);

	try {
		Assets::init();
	} catch(std::exception& e) {
		std::cerr << "Error initializing assets" << std::endl;
		std::cerr << e.what() << std::endl;
		return 1;
	}
	try {
		StateManager manager(ctx, std::make_unique<MainMenu>(ctx));
		sf::Clock clock;
		while(ctx.window.isOpen()) {
			manager.tick();
			manager.render();
		}
	} catch(std::exception& e) {
		std::cerr << "Error during game loop" << std::endl;
		std::cerr << e.what() << std::endl;
		return 2;
	}
}