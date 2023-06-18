#include <iostream>
#include "assets.hpp"
#include "tilemap.hpp"

struct VertexPool: sf::Drawable {
	static std::map<const sf::Texture*, sf::VertexArray> map;
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const {
		for(auto& [text, quads]: map) {
			states.texture = text;
			quads.setPrimitiveType(sf::Quads);
			rt.draw(quads, states);
			quads.clear();
		}
	}
};
std::map<const sf::Texture*, sf::VertexArray> VertexPool::map;

TileSet::TileSet(const tmx::Tileset& inner):
	inner(inner)
{
	sf::Image img;
	img.loadFromFile(inner.getImagePath());

	if(inner.hasTransparency()) {
		tmx::Colour transparency = inner.getTransparencyColour();
		img.createMaskFromColor({ transparency.r, transparency.g, transparency.b, transparency.a });
	}

	cellSize = sf::Texture::getMaximumSize();

	sf::Vector2u size = img.getSize();
	for(u32 y = 0; y < size.y; y += cellSize) {
		for(u32 x = 0; x < size.x; x += cellSize) {
			sf::Texture texture = sf::Texture();
			sf::IntRect r(
				{(i32)x, (i32)y},
				{(i32)cellSize, (i32)cellSize}
			);
			texture.loadFromImage(img, r);
			textures.push_back(texture);
		}
	}
}

const sf::Texture& TileSet::getTexture(u32 i) const {
	u32 widthCells = (inner.getImageSize().x + cellSize - 1) / cellSize;
	tmx::Vector2u pixelCoord = inner.getTile(i)->imagePosition;
	sf::Vector2u cellCoord;
	cellCoord.y = pixelCoord.y / cellSize;
	cellCoord.x = pixelCoord.x / cellSize;
	return textures.at(cellCoord.y * widthCells + cellCoord.x);
}

std::array<sf::Vertex, 4> TileSet::getQuad(sf::Vector2f pos, u32 i, u32 flips) const {
	tmx::Vector2u pixelCoord = inner.getTile(i)->imagePosition;
	sf::Vector2f tpos = {
		(f32)(pixelCoord.x % cellSize),
		(f32)(pixelCoord.y % cellSize)
	};
	tmx::Vector2u ts = inner.getTileSize();
	sf::Vector2f
		tl = pos,
		tr = { pos.x + ts.x, pos.y },
		bl = { pos.x, pos.y + ts.y },
		br = { pos.x + ts.x, pos.y + ts.y },
		ttl = tpos,
		ttr = { tpos.x + ts.x, tpos.y },
		tbl = { tpos.x, tpos.y + ts.y },
		tbr = { tpos.x + ts.x, tpos.y + ts.y };
	std::array<sf::Vertex, 4> v = {
		sf::Vertex(tl, ttl),
		sf::Vertex(tr, ttr),
		sf::Vertex(br, tbr),
		sf::Vertex(bl, tbl),
	};
	if(flips & tmx::TileLayer::FlipFlag::Horizontal)
		std::swap(v[0].position, v[3].position),
		std::swap(v[1].position, v[2].position);
	if(flips & tmx::TileLayer::FlipFlag::Vertical)
		std::swap(v[0].position, v[1].position),
		std::swap(v[2].position, v[3].position);
	if(flips & tmx::TileLayer::FlipFlag::Diagonal)
		std::swap(v[0].position, v[2].position);
	return v;
}

TileRender::TileRender(const TileSet* tileset, sf::Vector2u position, u8 flipFlags, u32 GID):
	tileset(tileset),
	position(position),
	flipFlags(flipFlags),
	GID(GID) {}

void TileRender::prepare() const {
	auto tile = tileset->inner.getTile(GID);
	auto quad = tileset->getQuad(position, GID, flipFlags);
	auto& vertices = VertexPool::map[&tileset->getTexture(GID)];
	for(auto v: quad) vertices.append(v);
}

TileAnimation::TileAnimation(TileRender render, tmx::Tileset::Tile::Animation animation):
	TileRender(render)
{
	index = rand() % animation.frames.size();
	elapsed = rand() / (f32)RAND_MAX * frames[index].duration;
}

TileCollision::TileCollision(sf::Vector2u origin, const std::vector<tmx::Object>& objects):
	origin(origin),
	objects(objects) {}

TileLayer::TileLayer(const tmx::TileLayer& layer, const tmx::Map& map, const std::map<u32, TileSet>& tilesets):
	inner(layer),
	tilesets(tilesets)
{
	for(auto& prop: inner.getProperties())
		if(prop.getName() == "z") z = prop.getIntValue();
	auto& tiles = inner.getTiles();
	auto& size = inner.getSize();
	for(u32 i = 0; i < tiles.size(); i += 1) {
		u32 GID = tiles[i].ID;
		if(GID == 0) continue;

		auto& [initialGID, tileset] = *--tilesets.upper_bound(GID);
		auto& tileSize = tileset.inner.getTileSize();
		sf::Vector2u pos;
		pos.y = i / size.x * tileSize.y;
		pos.x = i % size.x * tileSize.x;

		TileRender render(&tileset, pos, tiles[i].flipFlags, tiles[i].ID);
		auto tile = tileset.inner.getTile(GID);
		auto& obj = tile->objectGroup.getObjects();
		if(!obj.empty()) collisions.emplace_back(pos, obj);
		if(tile->animation.frames.size())
			animations.push_back({ render, tile->animation });
		else
			render.prepare();
	}
	tmx::FloatRect bounds = map.getBounds();
	t_static = std::make_unique<sf::RenderTexture>();
	t_dynamic = std::make_unique<sf::RenderTexture>();
	t_static->create(bounds.width, bounds.height);
	t_dynamic->create(bounds.width, bounds.height);
	s_static = sf::Sprite(t_static->getTexture());
	s_dynamic = sf::Sprite(t_dynamic->getTexture());
	t_static->draw(VertexPool());
	t_static->display();
}

void TileLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const {
	rt.draw(s_static, states);
	for(const TileRender& render: animations) render.prepare();
	t_dynamic->clear(sf::Color::Transparent);
	t_dynamic->draw(VertexPool(), states);
	t_dynamic->display();
	rt.draw(s_dynamic, states);
}

void TileLayer::update(sf::Time time) {
	for(auto& a: animations) {
		auto frame = a.frames[a.index];
		a.elapsed += time.asMilliseconds();
		if(a.elapsed < frame.duration) continue;
		a.index = (a.index + 1) % a.frames.size();
		a.elapsed -= frame.duration;
		auto& [initialGID, tileset] = *--tilesets.upper_bound(a.index);
		a.tileset = &tileset;
	}
}

TileMap::TileMap(tmx::Map&& map): inner(std::move(map)) {
	if(inner.getOrientation() != tmx::Orientation::Orthogonal)
		throw std::string("Not orthogonal");

	for(auto& tileset: inner.getTilesets()) {
		tilesets.insert({tileset.getFirstGID(), tileset});
	}

	tmx::Vector2u tileSize = inner.getTileSize();
	for(auto& layer: inner.getLayers()) {
		switch(layer->getType()) {
			case tmx::Layer::Type::Tile:
				layers.emplace_back(layer->getLayerAs<tmx::TileLayer>(), inner, tilesets);
				break;
			case tmx::Layer::Type::Object:
				appendObjects(layer->getLayerAs<tmx::ObjectGroup>().getObjects());
				break;
			// TODO
			case tmx::Layer::Type::Group:
				break;
			case tmx::Layer::Type::Image:
				break;
		}
	}
}

void TileMap::appendObjects(const std::vector<tmx::Object>& objs) {
	objects.insert(objects.end(), objs.begin(), objs.end());
}

std::vector<tmx::FloatRect> TileMap::collisions() const {
	std::vector<tmx::FloatRect> v;
	for(const TileLayer& l: layers) {
		for(auto& c: l.getCollisions()) {
			for(auto& o: c.objects) {
				auto& r = v.emplace_back(o.getAABB());
				r.left += c.origin.x;
				r.top += c.origin.y;
			}
		}
	}
	for(const tmx::Object& o: objects)
		v.push_back(o.getAABB()), std::cerr << o.getAABB().width << std::endl;
	return v;
}

void TileMap::update(sf::Time time) {
	for(TileLayer& l: layers) l.update(time);
}

void TileMap::render(ZRenderer& renderer) const {
	for(const auto& l: layers)
		renderer.insert(l.getZ(), std::cref(l));
}
