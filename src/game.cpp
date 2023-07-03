#include <tmxlite/ObjectGroup.hpp>
#include <tmxlite/Map.hpp>
#include <unordered_map>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include "game.hpp"
#include "menu.hpp"
#include "assets.hpp"
#include "error.hpp"

Game::Game(Context& ctx):
	State(ctx, 1),
	map("assets/cave.tmx"),
	avgFrame(0)
{
	sf::Vector2u ws = ctx.window.getSize();
	camera.setCenter({ ws.x * 0.5f, ws.y * 0.5f });
	camera.setSize(256.0f * ws.x / ws.y, 256.0f);
	testPlayer.setSize({ 32, 64 });
	for(auto& c: map.collisions()) ff.addObstacle<f32>({c.left, c.top}, {c.width, c.height});
	message = StateMessage::Push(std::make_unique<UserInterface>(ctx, this));
}

void Game::restartClock() {
	clock.restart();
}

f32 ffCalcTime = 0;
void Game::tick() {
	auto elapsed = clock.getElapsedTime();
	clock.restart();
	avgFrame = avgFrame * 0.9 + elapsed.asSeconds() * 0.1;
	map.update(elapsed);

	sf::Vector2f speed;
	auto isPressed = sf::Keyboard::isKeyPressed;
	using Key = sf::Keyboard::Key;
	if(isPressed(Key::Up) || isPressed(Key::W))
		speed.y -= 1;
	if(isPressed(Key::Down) || isPressed(Key::S))
		speed.y += 1;
	if(isPressed(Key::Left) || isPressed(Key::A))
		speed.x -= 1;
	if(isPressed(Key::Right) || isPressed(Key::D))
		speed.x += 1;
	if(speed.x || speed.y) speed *= 128 * elapsed.asSeconds() / hypotf(speed.x, speed.y);

	sf::Vector2f ps = testPlayer.getSize();
	sf::Vector2f pp = testPlayer.getPosition() + speed;
	sf::Vector2f pc = pp + ps * 0.5f;
	testPlayer.setPosition(pp);

	sf::Vector2f cs = camera.getSize();
	sf::Vector2f cc = camera.getCenter();
	sf::Vector2f tol = (cs - ps) * 0.5f - sf::Vector2f(32, 32);
	cc.x = std::clamp(cc.x, pc.x - tol.x, pc.x + tol.x);
	cc.y = std::clamp(cc.y, pc.y - tol.y, pc.y + tol.y);
	camera.setCenter(cc);

	sf::Clock ffClock;
	for(auto& e: testEnemies) ff.addEnemy(e.getPosition(), e.getSize());
	ff.calculate(pc, ps);
	ffCalcTime = ffCalcTime * 0.9 + 0.1 * ffClock.getElapsedTime().asMilliseconds();
	for(auto& e: testEnemies) {
		auto es = e.getSize();
		auto ep = e.getPosition();
		auto s = ff.query(ep, es);
		s *= 64 * elapsed.asSeconds();
		e.setPosition(ep + s);
	}
}

void Game::render() {
	renderer.clear();
	map.render(renderer);
	for(auto& e: testEnemies) renderer.insert(0, e);
	renderer.insert(0, testPlayer);

	ctx.window.setView(camera);
	ctx.window.draw(renderer);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		ctx.window.draw(ff);

	ctx.window.setView(camera);
	auto mp = sf::Mouse::getPosition(ctx.window);
	sf::Vector2f pos = ctx.window.mapPixelToCoords(mp);

	sf::Text debug;
	debug.setPosition(20, 20);
	debug.setFont(Assets::font);

	std::stringstream ss;
	ss << std::fixed << std::setprecision(2)
		<< pos.x << ' ' << pos.y << std::endl
		<< (1 / avgFrame) << " fps" << std::endl
		<< ffCalcTime << "ms ff calc" << std::endl;
	debug.setString(ss.str());

	sf::View v;
	v.setViewport({{0, 0}, {1, 1}});
	ctx.window.setView(v);
	ctx.window.draw(debug);
}
void Game::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseWheelScrolled) {
		auto cs = camera.getSize();
		float perc = event.mouseWheelScroll.delta > 0 ? 0.9 : 1.1;
		cs.x *= perc, cs.y *= perc;
		camera.setSize(cs);
	}
	if(event.type == sf::Event::MouseButtonPressed) {
		ctx.window.setView(camera);
		auto mp = sf::Mouse::getPosition(ctx.window);
		auto pos = ctx.window.mapPixelToCoords(mp);
		sf::RectangleShape rect;
		rect.setPosition(pos);
		rect.setSize({32, 32});
		testEnemies.push_back(rect);
	}
	if(event.type == sf::Event::Resized) {
		sf::Vector2u ws = ctx.window.getSize();
		camera.setSize(ws.x, ws.y);
	}
}
Game::~Game() {}

UserInterface::UserInterface(Context& ctx, Game* game):
	game(game),
	State(ctx, 0)
{
	if(!heart.loadFromFile("assets/GUI/PNG/HUD/CHARACTER HUD/HP Icon.png"))
		throw ImageLoadError();
	for(sf::Sprite& s: hearts) s.setTexture(heart);
}
void UserInterface::tick() {
	game->tick();
}
void UserInterface::render() {
	f32 x = 10;
	for(sf::Sprite& s: hearts) {
		s.setPosition(x, 10);
		x += 28;
		s.setScale(2,2);
		ctx.window.draw(s);
	}
	f32 hp = 0.6;
	for(u32 i = 0; i < 10*hp; i += 1)
		hearts[i].setColor(sf::Color::White);
	for(u32 i = 10*hp; i < 10; i += 1)
		hearts[i].setColor(sf::Color(0,0,0));
}
void UserInterface::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		message = StateMessage::Into(std::make_unique<PauseMenu>(ctx, game));
	game->handleEvent(event);
}
UserInterface::~UserInterface() {}
