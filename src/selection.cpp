#include <iostream>
#include "state.hpp"
#include "selection.hpp"
#include "types.hpp"
#include "assets.hpp"
#include "game.hpp"
#include "characters.hpp"

SelectionMenu::SelectionMenu(Context& ctx, GameSettings settings):
	State(ctx, true),
    settings(std::move(settings)) {}

CharacterSelection::CharacterSelection(Context& ctx, GameSettings settings):
	SelectionMenu(ctx, std::move(settings))
{
	scale = 2;
	sprites.resize(6);
	texture.loadFromFile("assets/character_icons.png");
	for(int x=0; x < 6; x++) {
		sprites[x].setTexture(texture);
		sprites[x].setTextureRect({x*64, 0, 64, 64});
	}
}

void CharacterSelection::click(){
	switch(selected) {
		case 0:
		settings.player = std::make_unique<CVince>(0, 0, &ctx.world, settings.weapon);
		break;
		case 1:
		settings.player = std::make_unique<CNate>(0, 0, &ctx.world, settings.weapon);
		break;
		case 2:
		settings.player = std::make_unique<CMatt>(0, 0, &ctx.world, settings.weapon);
		break;
		case 3:
		settings.player = std::make_unique<CLena>(0, 0, &ctx.world, settings.weapon);
		break;
		case 4:
		settings.player = std::make_unique<CJoy>(0, 0, &ctx.world, settings.weapon);
		break;
		case 5:
		settings.player = std::make_unique<CDany>(0, 0, &ctx.world, settings.weapon);
		break;
	}
	message = StateMessage::Set(std::make_unique<Game>(ctx, std::move(settings)));
}

void SelectionMenu::render(){
	int len = sprites.size();
	int size_x = 0;
	std::vector<int> offset(len);
	for(int i = 0; i<len; i++) {
		sprites[i].setScale(scale, scale);
		offset[i] = size_x;
		size_x +=sprites[i].getLocalBounds().width*scale;
	}
	size_x -= 8;

	sf::Vector2f pos;
	sf::Vector2u wsize = ctx.window.getSize();
	sf::FloatRect v_rect(0, 0, wsize.x, wsize.y);
	ctx.window.setView(sf::View(v_rect));
	pos.x = (wsize.x - size_x) / 2;
	pos.y = (wsize.y - sprites[0].getLocalBounds().height*scale) / 2;

	for(int i=0; i<len; i++){
		sprites[i].setPosition(pos.x + offset[i], pos.y);
		ctx.window.draw(sprites[i]);
	}
}

void SelectionMenu::handleEvent(sf::Event event){
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
		click();
	}
}

void SelectionMenu::moveMouse(sf::Vector2i position){
	selected = -1;
	for(u32 i = 0; i < sprites.size(); i += 1) {
		sf::Sprite& sprite = sprites[i];
		sf::FloatRect rect = sprite.getGlobalBounds();
		if(rect.contains(position.x, position.y)) {
			selected = i;
			break;
		}
	}
}

void SelectionMenu::moveKeys(i32 delta) {
	int mod = sprites.size();
	selected = (delta + selected + mod) % mod;
}

MapSelection::MapSelection(Context& ctx):
	SelectionMenu(ctx, {})
{
	sprites.resize(3);
	texture.loadFromFile("assets/map_icons.png");
	for(int x=0; x < 3; x++) {
		sprites[x].setTexture(texture);
		sprites[x].setTextureRect({x*256, 0, 256, 256});
	}
}
void MapSelection::click(){
	switch(selected) {
		case 0:
		settings.map = std::make_unique<TileMap>("assets/isle.tmx");
		break;
		case 1:
		settings.map = std::make_unique<TileMap>("assets/cave.tmx");
		break;
		case 2:
		settings.map = std::make_unique<TileMap>("assets/forest.tmx");
		break;
	}
	message = StateMessage::Set(std::make_unique<WeaponSelection>(ctx, std::move(settings)));
}

WeaponSelection::WeaponSelection(Context& ctx, GameSettings settings):
	SelectionMenu(ctx, std::move(settings))
{
	scale = 4;
	sprites.resize(3);
	texture.loadFromFile("assets/GunsIcons.png");
	for(int x=0; x < 3; x++) {
		sprites[x].setTexture(texture);
		sprites[x].setTextureRect({x*64, 0, 64, 64});
	}
}
void WeaponSelection::click(){
	switch(selected) {
		case 0:
		settings.weapon = WeaponType::GUN;
		break;
		case 1:
		settings.weapon = WeaponType::SHOTGUN;
		break;
		case 2:
		settings.weapon = WeaponType::MACHINEGUN;
		break;
	}
	message = StateMessage::Set(std::make_unique<CharacterSelection>(ctx, std::move(settings)));
}