#ifndef SFML_TILEMAP_H
#define SFML_TILEMAP_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <array>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>
#include "render.hpp"
#include "types.hpp"

/// Guarda um tileset e as texturas utilizadas por ele
class TileSet {
	private:
	std::vector<sf::Texture> textures;
	u32 cellSize;

	public:
	/// Tipo interior
	const tmx::Tileset& inner;
	/// Cria um novo TileSet a partir de um Tileset do tmxlite
	TileSet(const tmx::Tileset& inner);
	/// Retorna a textura associada ao tile com o id especificado
	const sf::Texture& getTexture(u32 i) const;
	/// Retorna os quatro vértices que formam a textura desse tile, na posição especificada
	std::array<sf::Vertex, 4> getQuad(sf::Vector2f pos, u32 index, u32 flips) const;
};

/// Guarda dados relevantes à renderização de tiles estáticos
struct TileRender {
	/// O Tileset que contém este tile
	const TileSet* tileset;
	/// A origem, no ponto superior esquerdo do tile
	sf::Vector2f position;
	/// Flags de rotação. Veja [FlipFlag](https://codedocs.xyz/fallahn/tmxlite/classtmx_1_1TileLayer.html#a64883c49573c7766a86a9823b5b8f31b)
	u8 flipFlags;
	/// O ID global deste tile, que o identifica dentre todos os tilesets
	u32 GID;
	/// Guarda as informações para renderizar um tile
	TileRender(const TileSet* tileset, sf::Vector2u position, u8 flipFlags, u32 GID);
	/// Coloca vertices em um buffer para serem renderizados
	void prepare() const;
};

/// Guarda dados relevantes à renderização de tiles animados
struct TileAnimation: TileRender {
	/// A lista de frames contidos nesta animação
	std::vector<tmx::Tileset::Tile::Animation::Frame> frames;
	/// O índice do frame atual
	u32 index;
	/// O tempo gasto no frame atual
	f64 elapsed;
	/// Guarda as informações para renderizar um tile animado
	TileAnimation(TileRender render, tmx::Tileset::Tile::Animation animation);
};

/// Guarda dados relevantes à colisão de tiles
struct TileCollision {
	/// A origem deste tile
	sf::Vector2u origin;
	/// As caixas de colisão contidas nesse tile
	const std::vector<tmx::Object>& objects;
	/// Guarda as informações para registrar a colisão de um tile
	TileCollision(sf::Vector2u origin, const std::vector<tmx::Object>& objects);
};

/// Guarda uma camada de tiles e as texturas, animações e colisões associadas
class TileLayer: public sf::Drawable {
	private:
	const std::map<u32, TileSet>& tilesets;
	std::vector<TileAnimation> animations;
	std::vector<TileCollision> collisions;
	std::unique_ptr<sf::RenderTexture> t_static, t_dynamic;
	sf::Sprite s_static, s_dynamic;
	u32 z;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

	public:
	/// O tipo interior
	const tmx::TileLayer& inner;
	/// Atualiza animações
	void update(sf::Time time);
	/// Lista as colisões da camada
	const std::vector<TileCollision>& getCollisions() const { return collisions; }
	/// Retorna a posição Z da camada
	const u32& getZ() const { return z; }
	/// Cria uma nova layer de tiles a partir de um tipo interior e dos tilesets associados a um mapa
	TileLayer(const tmx::TileLayer& layer, const tmx::Map& map, const std::map<u32, TileSet>& tilesets);
};

/// Guarda um mapa de tiles e as camadas associadas
class TileMap {
	friend class TileLayer;
	private:
	tmx::Map inner;
	std::vector<TileLayer> layers;
	std::vector<tmx::Object> objects;
	std::map<u32, TileSet> tilesets;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const;

	public:
	/// Atualiza animações
	void update(sf::Time time);
	/// Adiciona o mapa ao renderizador
	void render(ZRenderer& renderer) const;
	/// Lista as colisões do mapa
	std::vector<tmx::FloatRect> collisions() const;
	/// Cria um novo tilemap a partir do caminho de um arquivo
	TileMap(std::string location);
};

#endif