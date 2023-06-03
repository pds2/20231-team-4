#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <functional>
#include "state.hpp"
#include "game.hpp"

/// Uma configuração de botão, que armazena um texto e um callback
struct Button {
	/// O texto a ser exibido para o botão
	std::string text;
	/// A função a ser chamada quando o botão for clicado
	std::function<void()> click;
};

/// Um Menu padrão, que desenha botões na tela
class Menu: public State {
	private:
	std::vector<Button> buttons;
	std::vector<sf::Text> texts;
	i32 selected;

	void moveMouse(sf::Vector2i position);
	void moveKeys(i32 delta);

	public:
	/// Cria um novo Menu vazio
	Menu(Context& ctx, bool opaque = 0);
	/// Cria um novo Menu com as configurações especificadas
	Menu(Context& ctx, std::vector<Button> buttons, bool opaque = 0);
	/// Muda os botões do menu para os especificados
	void setButtons(std::vector<Button> buttons);
	void render();
	void handleEvent(sf::Event event);
};

/// Um menu principal, responsável por iniciar o jogo
class MainMenu: public Menu {
	private:
	void handleEvent(sf::Event event);

	public:
	/// Cria um novo menu principal em um contexto
	MainMenu(Context& ctx);
};

/// Armazena um jogo pausado, e mostra um menu
class PauseMenu: public Menu {
	private:
	Game* game;

	public:
	/// Cria um menu de pause para um estado de jogo
	PauseMenu(Context& ctx, Game* game);
	/// Retorna ao jogo
	void resume();
	void handleEvent(sf::Event event);
};

#endif
