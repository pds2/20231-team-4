#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <functional>
#include "state.h"

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
	/// Cria um novo Menu com as configurações especificadas
	Menu(Context& ctx, std::initializer_list<Button> buttons);
	~Menu();
	void tick();
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

#endif
