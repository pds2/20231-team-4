#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <tmxlite/Map.hpp>
#include <vector>
#include "state.hpp"
#include "flowfield.hpp"
#include "tilemap.hpp"

/// Armazena dados sobre um estado de jogo
class Game: public State {
	private:
	ZRenderer renderer;
	sf::View camera;
	sf::Clock clock;
	f64 avgFrame;
	TileMap map;
	std::vector<sf::RectangleShape> testEnemies;
	sf::RectangleShape testPlayer;
	FlowField ff;

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
	sf::Texture heart;
	sf::Sprite hearts[10];

	public:
	/// Cria uma interface de usuário para um estado de jogo
	UserInterface(Context& ctx, Game* game);
	void tick();
	void render();
	void handleEvent(sf::Event event);
	~UserInterface();
};

#endif