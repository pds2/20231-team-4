#include "state.h"
#include "menu.h"
#include "types.h"
#include "assets.h"
#include <iostream>

Menu::Menu(std::initializer_list<Button> button_list):
	State(0),
	buttons(button_list),
	selected(-1)
{
	texts.resize(buttons.size());
	for(u32 i = 0; i < buttons.size(); i += 1) {
		sf::Text& text = texts[i];
		Button& button = buttons[i];
		text.setFont(Assets::font);
		text.setString(button.text);
	}
}

Menu::~Menu() {}

void Menu::moveKeys(i32 delta) {
	i32 mod = buttons.size();
	selected = (delta + selected + mod) % mod;
}

void Menu::moveMouse(sf::Vector2i p_mouse) {
	selected = -1;
	for(u32 i = 0; i < buttons.size(); i += 1) {
		sf::Text& text = texts[i];
		sf::FloatRect rect = text.getGlobalBounds();
		bool within = p_mouse.x >= rect.left &&
			p_mouse.x <= rect.left + rect.width &&
			p_mouse.y >= rect.top &&
			p_mouse.y <= rect.top + rect.height;
		if(within) {
			selected = i;
			break;
		}
	}
}

void Menu::tick() {}

void Menu::render(sf::RenderWindow& window) {
	sf::Vector2u s_window = window.getSize();
	sf::FloatRect v_rect(0, 0, s_window.x, s_window.y);
	window.setView(sf::View(v_rect));

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
		window.draw(text);
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

MainMenu::MainMenu():
	Menu({
		{
			.text = "hello",
			.click = [this]() {
				std::cerr << "hello" << std::endl;
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