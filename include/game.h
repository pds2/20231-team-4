#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "state.h"

/// Armazena dados sobre um estado de jogo
class Game: public State {
	private:
	std::vector<sf::RectangleShape> rects;
	sf::View camera;
	sf::Clock clock;

	public:
	/// Cria um novo jogo em um contexto
	Game(Context& ctx);
	void restartClock();
	void tick();
	void render();
	void handleEvent(sf::Event event);
	~Game();
};

/// Contém um jogo, e mostra uma interface gráfica
class UserInterface: public State {
	private:
	Game* game;

	public:
	/// Cria uma interface de usuário para um estado de jogo
	UserInterface(Context& ctx, Game* game);
	void tick();
	void render();
	void handleEvent(sf::Event event);
	~UserInterface();
};

#endif