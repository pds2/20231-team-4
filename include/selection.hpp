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
    std::unique_ptr<Weapon> weapon;
};

class CharacterSelection : public State {
	private:
	sf::Texture texture;
	sf::Sprite sprites[6];
	i32 selected = -1;
    GameSettings settings;
	void moveMouse(sf::Vector2i position);
	void moveKeys(i32 delta);

	public:
	CharacterSelection(Context& ctx, GameSettings settings);
	void render();
	void handleEvent(sf::Event event);
};

class MapSelection : public State {
	private:
	sf::Texture texture;
	sf::Sprite sprites[3];
	i32 selected = -1;
	void moveMouse(sf::Vector2i position);
	void moveKeys(i32 delta);
    GameSettings settings;
	public:
	MapSelection(Context& ctx);
	void render();
	void handleEvent(sf::Event event);
};

//class GunSelection

#endif