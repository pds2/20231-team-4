#include <iostream>
#include "game.h"

Game::Game(Context& ctx):
	State(ctx, 1)
{
	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(ws.x, ws.y);

	for(int i = 0; i < 10; i += 1) {
		sf::RectangleShape r;
		r.setPosition(i*50, i*50);
		r.setSize({50, 50});
		rects.push_back(r);
	}
	message = StateMessage::Push(new UserInterface(ctx, this));
}

void Game::tick() {

}

void Game::render() {
	ctx.window.setView(camera);
	for(sf::RectangleShape& r: rects)
		ctx.window.draw(r);
}
void Game::handleEvent(sf::Event event) {}
Game::~Game() {}

UserInterface::UserInterface(Context& ctx, Game* game):
	game(game),
	State(ctx, 0) {}
void UserInterface::tick() {
	game->tick();
}
void UserInterface::render() {}
void UserInterface::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		message = StateMessage::Into(new PauseMenu(ctx, game));
	game->handleEvent(event);
}
UserInterface::~UserInterface() {}

PauseMenu::PauseMenu(Context& ctx_, Game* game_):
	game(game_),
	Menu(ctx_, {
		{
			.text = "continue",
			.click = [this]() {
				message = StateMessage::Into(new UserInterface(ctx, game));
			}
		},
		{
			.text = "main menu",
			.click = [this]() {
				message = StateMessage::Set(new MainMenu(ctx));
			}
		}
	}) {}
void PauseMenu::tick() {}
void PauseMenu::render() {
	Menu::render();
}
void PauseMenu::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		message = StateMessage::Into(new UserInterface(ctx, game));
	Menu::handleEvent(event);
}
PauseMenu::~PauseMenu() {}
