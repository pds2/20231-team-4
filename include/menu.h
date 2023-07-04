#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <functional>
#include "state.h"
#include "game.h"

/// Uma configuração de botão, que armazena um texto e um callback
struct Button {
	/// O texto a ser exibido para o botão
	std::string text;
	/// A função a ser chamada quando o botão for clicado
	std::function<void()> click;
};

/// Uma configuração de lore, que armazena o texto a ser disposto na tela
struct Lore {
	/// O texto a ser exibido na tela
	std::string lore;
};

/// Uma legenda, que aparecerá na parte inferior da tela
struct Subtext {
	/// O texto a ser exibido na legenda
	std::string text;
};

/// Um Menu padrão, que desenha botões na tela
class Menu: public State {
	private:
	std::vector<Button> buttons;
	std::vector<sf::Text> texts;
	i32 selected;
	Lore lore;
	Subtext subtext;

	void moveMouse(sf::Vector2i position);
	void moveKeys(i32 delta);

	public:
	/// Cria um novo Menu vazio
	Menu(Context& ctx, bool opaque = 0);
	/// Cria um novo Menu com as configurações especificadas para menus de seleção
	Menu(Context& ctx, std::vector<Button> buttons, bool opaque = 0);
	/// Cria um novo Menu com as configurações especificadas para menus de texto
	Menu(Context& ctx, Lore lore, Subtext subtext, bool opaque = 1);
	/// Muda os botões do menu para os especificados
	void setButtons(std::vector<Button> buttons);
	void setLore(Lore lore);
	void SetSubtext(Subtext subtext);
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

/// Conta a história do jogo para o jogador ao iniciar um novo jogo.
class LoreDisplay: public Menu {
	private:
	Lore lore; 

	public:
	/// Cria uma tela de história vazia.
	LoreDisplay(Context& ctx, bool opaque = 1);
	/// Cria uma série de páginas que contam a história do jogo.
	LoreDisplay(Context& ctx, Lore lore, Subtext subtext, bool opaque = 1);
	/// Avança a página até que toda a história seja contada. 
	void nextPage();
	void handleEvent(sf::Event event);
};

#endif
