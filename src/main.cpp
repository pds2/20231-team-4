#include <sstream>
#include <iostream>
#include "loading.hpp"
#include "menu.hpp"
using namespace std;

int main() {
	auto mode = sf::VideoMode::getDesktopMode();
	mode.width *= 0.8, mode.height *= 0.8;
	Context ctx = {
		sf::RenderWindow(mode, "Hello, World!")
	};
	ctx.window.setFramerateLimit(60);
	ctx.window.setVerticalSyncEnabled(true);

	StateManager manager(ctx, std::make_unique<MainMenu>(ctx));

	sf::Clock clock;
	while(ctx.window.isOpen()) {
		manager.tick();
		manager.render();
	}
}