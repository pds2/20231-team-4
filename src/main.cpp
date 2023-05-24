#include <sstream>
#include <iostream>
#include "state.h"
#include "loading.h"
#include "menu.h"
using namespace std;
int main() {
	sf::RenderWindow window(sf::VideoMode(800, 600), "Hello, World!");
	StateManager manager(new MainMenu(), window);
	while(window.isOpen()) {
		manager.tick();
		manager.render();
	}
}