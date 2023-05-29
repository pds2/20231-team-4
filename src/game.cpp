#include <iostream>
#include "game.h"
#include "menu.h"

Game::Game(Context& ctx):
	State(ctx, 1)
{
	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(ws.x, ws.y);
	message = StateMessage::Push(std::make_unique<UserInterface>(ctx, this));
}

void Game::restartClock() {
	clock.restart();
}

Object::Object(CollisionObject collision):
	collision(collision)
{
	rectangle.setSize({50, 50});
}

void Game::tick() {
	sf::Vector2i mp = sf::Mouse::getPosition(ctx.window);
	for(auto& o: objects) {
		o.collision.updatePosition(o.rectangle);
		auto center = o.collision.getCenter();
		sf::Vector2f dir = {
			mp.x - center.x,
			mp.y - center.y
		};
		f32 norm = hypot(dir.x, dir.y);
		f32 t = PPM * 5 / (norm != 0 ? norm : 1);
		dir.x *= t, dir.y *= t;
		o.collision.setVelocity(dir);
	}
	f32 elapsed = clock.getElapsedTime().asSeconds();
	cworld.step(elapsed);
	clock.restart();
}

void Game::render() {
	ctx.window.setView(camera);
	for(auto& o: objects) ctx.window.draw(o.rectangle);
}
void Game::handleEvent(sf::Event event) {
	sf::Vector2i mp = sf::Mouse::getPosition(ctx.window);
	if(event.type == sf::Event::MouseWheelMoved) {
		Object obj(cworld.spawnEnemy({(f32)mp.x, (f32)mp.y}));
		objects.push_back(obj);
	}
	if(event.type == sf::Event::MouseButtonPressed) {
		Object obj(cworld.spawnObstacle({(f32)mp.x, (f32)mp.y}));
		obj.rectangle.setFillColor(sf::Color::Green);
		objects.push_back(obj);
	}
	if(event.type == sf::Event::Resized) {
		sf::Vector2u ws = ctx.window.getSize();
		camera.setSize(ws.x, ws.y);
	}
}
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
