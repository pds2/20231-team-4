#ifndef SFML_TILEMAP_H
#define SFML_TILEMAP_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <array>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "types.hpp"

class TileSet {
	private:
	std::vector<sf::Texture> textures;
	u32 cellSize;

	public:
	const tmx::Tileset& inner;
	TileSet(const tmx::Tileset& inner);
	const sf::Texture& getTexture(u32 i) const;
	std::array<sf::Vertex, 4> getQuad(sf::Vector2f pos, u32 index, u32 flips) const;
};

struct TileRender {
	const TileSet* tileset;
	sf::Vector2f position;
	u8 flipFlags;
	u32 GID;
	TileRender(const TileSet* tileset, sf::Vector2u position, u8 flipFlags, u32 GID);
	void prepare() const;
};

struct TileAnimation: TileRender {
	std::vector<tmx::Tileset::Tile::Animation::Frame> frames;
	u32 index;
	f64 elapsed;
	TileAnimation(TileRender render, tmx::Tileset::Tile::Animation animation);
};

class TileLayer: public sf::Drawable {
	private:
	const std::map<u32, TileSet>& tilesets;
	std::vector<TileAnimation> animations;
	std::unique_ptr<sf::RenderTexture> t_static, t_dynamic;
	sf::Sprite s_static, s_dynamic;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

	public:
	const tmx::TileLayer& inner;
	void update(sf::Time time);
	TileLayer(const tmx::TileLayer& layer, const tmx::Map& map, const std::map<u32, TileSet>& tilesets);
};

class TileMap: public sf::Drawable {
	friend class TileLayer;
	private:
	tmx::Map inner;
	std::vector<TileLayer> layers;
	std::map<u32, TileSet> tilesets;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

	public:
	void update(sf::Time time);
	TileMap(tmx::Map&& map);
};

#endif