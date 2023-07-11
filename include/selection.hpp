#ifndef SELECTION_H
#define SELECTION_H

#include <SFML/Graphics.hpp>
#include <functional>
#include "state.hpp"
#include "player.hpp"
#include "tilemap.hpp"
#include "weapon.hpp"

struct GameSettings {
    std::unique_ptr<Player> player;
    std::unique_ptr<TileMap> map;
    WeaponType weapon;
};

class SelectionMenu : public State {
	protected:
	sf::Texture texture;
	std::vector<sf::Sprite> sprites;
	i32 selected = -1;
    GameSettings settings;
	void moveMouse(sf::Vector2i position);
	void moveKeys(i32 delta);
	virtual void click() = 0;
	f32 scale = 1;

	public:
	SelectionMenu(Context& ctx, GameSettings settings);
	void render();
	void handleEvent(sf::Event event);
};

class CharacterSelection : public SelectionMenu {
	protected:
	void click() override;

	public:
	CharacterSelection(Context& ctx, GameSettings settings);
};

class MapSelection : public SelectionMenu {
	protected:
	void click() override;

	public:
	MapSelection(Context& ctx);
};

class WeaponSelection : public SelectionMenu {
	protected:
	void click() override;

	public:
	WeaponSelection(Context& ctx, GameSettings settings);
};

#endif