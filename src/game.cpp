#include <iostream>
#include "game.h"
#include "menu.h"

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
	message = StateMessage::Push(std::make_unique<LoreDisplay>(ctx, this));

	character_texture.loadFromFile("assets/char_texture_test.png");
	character_sprite.setTexture(character_texture);
	character_sprite.setTextureRect(sf::IntRect(16, 16, 32, 50));
}

void Game::restartClock() {
	clock.restart();
}

void Game::tick() {

}

void Game::render() {
	ctx.window.setView(camera);
	for(sf::RectangleShape& r: rects)
		ctx.window.draw(r);
	
	character_sprite.setPosition(0, 0);
	character_sprite.setScale(1, 1);
	ctx.window.draw(character_sprite);
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
		message = StateMessage::Into(std::make_unique<PauseMenu>(ctx, game));
	game->handleEvent(event);
}
UserInterface::~UserInterface() {}
