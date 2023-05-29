#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "state.h"
#include "collision.h"

struct Object {
	CollisionObject collision;
	sf::RectangleShape rectangle;
	Object(CollisionObject collision);
};

/// Armazena dados sobre um estado de jogo
class Game: public State {
	private:
	CollisionWorld cworld;
	std::vector<Object> objects;
	sf::View camera;
	sf::Clock clock;

	public:
	/// Cria um novo jogo em um contexto
	Game(Context& ctx);
	/// Reinicia o relógio do jogo. Deve ser usado após despausar.
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