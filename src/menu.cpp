#include <iostream>
#include "state.hpp"
#include "menu.hpp"
#include "types.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "characters.hpp"

Menu::Menu(Context& ctx, bool opaque):
	Menu(ctx, {}, opaque) {}
Menu::Menu(Context& ctx, std::vector<Button> button_list, bool opaque):
	State(ctx, opaque),
	buttons(button_list),
	backgrounds(button_list.size()),
	selected(-1)
{
	setButtons(buttons);
	for(sf::Sprite &s: backgrounds) s.setTexture(Assets::buttonBackground);
}

void Menu::setButtons(std::vector<Button> buttons_) {
	buttons = buttons_;
	texts.resize(buttons.size());
	for(u32 i = 0; i < buttons.size(); i += 1) {
		sf::Text& text = texts[i];
		Button& button = buttons[i];
		text.setFont(Assets::terminal);
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
	std::vector<sf::Vector2f> centers(texts.size());

	sf::Vector2u s_window = ctx.window.getSize();
	sf::FloatRect v_rect(0, 0, s_window.x, s_window.y);
	ctx.window.setView(sf::View(v_rect));
	
	sf::RectangleShape backg;
	backg.setPosition({v_rect.left, v_rect.top});
	backg.setSize({v_rect.width, v_rect.height});
	backg.setFillColor(sf::Color(50, 50, 50, 122));
	ctx.window.draw(backg);

	for(sf::Text& text: texts) text.setFillColor(sf::Color(100,100,50));
	if(selected >= 0) texts[selected].setFillColor(sf::Color::Yellow);

	f32 size_y = 0;
	f32 backgroundy = Assets::buttonBackground.getSize().y;
	for(u32 i = 0; i < texts.size(); i += 1) {
		sf::Text& text = texts[i];
		sf::FloatRect rect = text.getLocalBounds();
		centers[i] = {
			s_window.x * 0.5f,
			size_y
		};
		text.setPosition({
			(s_window.x - rect.width) * 0.5f,
			size_y - backgroundy/2
		});
		size_y += backgroundy + 8;
	}
	size_y -= 8;

	f32 offset = (s_window.y - size_y) * 0.5f;
	for(u32 i = 0; i < texts.size(); i += 1) {
		sf::Sprite& background = backgrounds[i];
		sf::Vector2f center = centers[i];
		sf::Vector2u size = background.getTexture()->getSize();
		ctx.window.draw(background);
		background.setOrigin(size.x/2, size.y/2);
		background.setPosition(center.x, center.y += offset);
	}
	for(u32 i = 0; i < texts.size(); i += 1) {
		sf::Text& text = texts[i];
		sf::Vector2f pos = text.getPosition();
		pos.y += offset;
		text.setPosition(pos.x, pos.y);
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
				message = StateMessage::Into(std::make_unique<CharacterSelection>(ctx));
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

CharacterSelection::CharacterSelection(Context& ctx):
	State(ctx, true)
{
	texture.loadFromFile("assets/character_icons.png");
	for(int x=0; x < 6; x++) {
		sprites[x].setTexture(texture);
		sprites[x].setTextureRect({x*64, 0, 64, 64});
	}
}

void CharacterSelection::render(){
	int size_x = 0;
	int offset[6];
	for(int i = 0; i<6; i++) {
		offset[i] = size_x;
		size_x +=128+8;
	}
	size_x -= 8;

	sf::Vector2f pos;
	sf::Vector2u wsize = ctx.window.getSize();
	sf::FloatRect v_rect(0, 0, wsize.x, wsize.y);
	ctx.window.setView(sf::View(v_rect));
	pos.x = (wsize.x - size_x) / 2;
	pos.y = (wsize.y - 128) / 2;

	for(int i=0; i<6; i++){
		sprites[i].setScale(2,2);
		sprites[i].setPosition(pos.x + offset[i], pos.y);
		ctx.window.draw(sprites[i]);
	}
}

void CharacterSelection::handleEvent(sf::Event event){
	if(event.type == sf::Event::MouseMoved) {
		sf::Event::MouseMoveEvent p = event.mouseMove;
		moveMouse({p.x, p.y});
	}
	if(event.type == sf::Event::KeyPressed) {
		if(event.key.code == sf::Keyboard::Up) moveKeys(-1);
		if(event.key.code == sf::Keyboard::Down) moveKeys(1);
	}

	bool enter = event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter;
	if((event.type == sf::Event::MouseButtonPressed || enter) && selected != -1){
		std::unique_ptr<Player> character;
		switch(selected) {
			case 0:
			character = std::make_unique<CVince>(0, 0, &ctx.world, WeaponType::GUN);
			break;
			case 1:
			character = std::make_unique<CNate>(0, 0, &ctx.world, WeaponType::GUN);
			break;
			case 2:
			character = std::make_unique<CMatt>(0, 0, &ctx.world, WeaponType::GUN);
			break;
			case 3:
			character = std::make_unique<CLena>(0, 0, &ctx.world, WeaponType::GUN);
			break;
			case 4:
			character = std::make_unique<CJoy>(0, 0, &ctx.world, WeaponType::GUN);
			break;
			case 5:
			character = std::make_unique<CDany>(0, 0, &ctx.world, WeaponType::GUN);
			break;
		}
		message = StateMessage::Set(std::make_unique<Game>(ctx, std::move(character)));
	}
}

void CharacterSelection::moveMouse(sf::Vector2i position){
	selected = -1;
	for(u32 i = 0; i < 6; i += 1) {
		sf::Sprite& sprite = sprites[i];
		sf::FloatRect rect = sprite.getGlobalBounds();
		if(rect.contains(position.x, position.y)) {
			selected = i;
			break;
		}
	}
}

void CharacterSelection::moveKeys(i32 delta) {
	selected = (delta + selected + 6) % 6;
}
