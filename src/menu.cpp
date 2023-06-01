#include <iostream>
#include "state.h"
#include "menu.h"
#include "types.h"
#include "assets.h"
#include "game.h"

Menu::Menu(Context& ctx, bool opaque):
	Menu(ctx, {}, opaque) {}
Menu::Menu(Context& ctx, std::vector<Button> button_list, bool opaque):
	State(ctx, opaque),
	buttons(button_list),
	selected(-1)
{
	setButtons(buttons);
}

void Menu::setButtons(std::vector<Button> buttons_) {
	buttons = buttons_;
	texts.resize(buttons.size());
	for(u32 i = 0; i < buttons.size(); i += 1) {
		sf::Text& text = texts[i];
		Button& button = buttons[i];
		text.setFont(Assets::font);
		text.setString(button.text);
	}
	selected = -1;
}

void Menu::moveKeys(i32 delta) {
	i32 mod = buttons.size();
	selected = (delta + selected + mod) % mod;
}

void Menu::moveMouse(sf::Vector2i p_mouse) {
	selected = -1;
	for(u32 i = 0; i < buttons.size(); i += 1) {
		sf::Text& text = texts[i];
		sf::FloatRect rect = text.getGlobalBounds();
		if(rect.contains(p_mouse.x, p_mouse.y)) {
			selected = i;
			break;
		}
	}
}

void Menu::render() {
	sf::Vector2u s_window = ctx.window.getSize();
	sf::FloatRect v_rect(0, 0, s_window.x, s_window.y);
	ctx.window.setView(sf::View(v_rect));
	
	sf::RectangleShape backg;
	backg.setPosition({v_rect.left, v_rect.top});
	backg.setSize({v_rect.width, v_rect.height});
	backg.setFillColor(sf::Color(50, 50, 50, 122));
	ctx.window.draw(backg);

	for(sf::Text& text: texts) text.setFillColor(sf::Color::White);
	if(selected >= 0) texts[selected].setFillColor(sf::Color::Yellow);

	f32 size_y = 0;
	for(u32 i = 0; i < texts.size(); i += 1) {
		sf::Text& text = texts[i];
		sf::FloatRect rect = text.getLocalBounds();
		text.setPosition({
			(s_window.x - rect.width) * 0.5f,
			size_y
		});
		size_y += text.getCharacterSize() + 8;
	}
	size_y -= 8;

	f32 offset = (s_window.y - size_y) * 0.5f;
	for(u32 i = 0; i < texts.size(); i += 1) {
		sf::Text& text = texts[i];
		sf::Vector2f pos = text.getPosition();
		text.setPosition(pos.x, pos.y += offset);
		ctx.window.draw(text);
	}
}

void Menu::handleEvent(sf::Event event) {
	if(event.type == sf::Event::MouseMoved) {
		sf::Event::MouseMoveEvent p = event.mouseMove;
		moveMouse({p.x, p.y});
	}
	if(event.type == sf::Event::KeyPressed) {
		if(event.key.code == sf::Keyboard::Up) moveKeys(-1);
		if(event.key.code == sf::Keyboard::Down) moveKeys(1);
	}

	bool enter = event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter;
	if(event.type == sf::Event::MouseButtonPressed || enter)
		if(selected >= 0) buttons[selected].click();
}

MainMenu::MainMenu(Context& ctx_):
	Menu(ctx_, {
		{
			.text = "new game",
			.click = [this]() {
				message = StateMessage::Into(std::make_unique<Game>(ctx));
			}
		},
		{
			.text = "quit",
			.click = [this]() {
				message = StateMessage::Pop();
			}
		}
	}) {}

void MainMenu::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
		message = StateMessage::Pop();
	Menu::handleEvent(event);
}

PauseMenu::PauseMenu(Context& ctx_, Game* game_):
	game(game_),
	Menu(ctx_, {
		{
			.text = "continue",
			.click = [this]() {
				resume();
			}
		},
		{
			.text = "main menu",
			.click = [this]() {
				message = StateMessage::Set(std::make_unique<MainMenu>(ctx));
			}
		}
	}) {}

void PauseMenu::resume() {
	game->restartClock();
	message = StateMessage::Into(std::make_unique<UserInterface>(ctx, game));
}

void PauseMenu::handleEvent(sf::Event event) {
	if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) 
		resume();
	Menu::handleEvent(event);
}
