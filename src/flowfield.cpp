#include <iostream>
#include <cmath>
#include <map>
#include <numeric>
#include "assets.hpp"
#include "flowfield.hpp"

const i32 SCALE = 16;
const i32 INF = std::numeric_limits<i16>::max();

template <typename T, size_t S>
Array2<T, S>::Array2(const T& initial) {
	fill(initial);
}

template <typename T, size_t S>
void Array2<T, S>::fill(const T& initial) {
	for(auto& a: *this) a.fill(initial);
}

template <typename T, size_t S>
template <typename I>
T& Array2<T, S>::operator[](sf::Vector2<I> pos) {
	return this->at(pos.x).at(pos.y);
}

template <typename T, size_t S>
template <typename I>
const T& Array2<T, S>::operator[](sf::Vector2<I> pos) const {
	return this->at(pos.x).at(pos.y);
}

template <typename T, size_t S>
template <typename I>
std::array<T, S>& Array2<T, S>::operator[](I index) {
	return this->at(index);
}

template <typename T, size_t S>
template <typename I>
const std::array<T, S>& Array2<T, S>::operator[](I index) const {
	return this->at(index);
}

template <typename T, size_t S>
VArray2<T, S>::VArray2(const T& initial): versions(0), version(0), inner(initial), def(initial) {}

template <typename T, size_t S>
void VArray2<T, S>::fill(const T& initial) {
	def = initial;
	version += 1;
}

template <typename T, size_t S>
template <typename I>
T& VArray2<T, S>::operator[](sf::Vector2<I> pos) {
	if(versions[pos] != version) {
		versions[pos] = version;
		return inner[pos] = def;
	}
	return inner[pos];
}


template <typename T, size_t S>
template <typename I>
const T& VArray2<T, S>::operator[](sf::Vector2<I> pos) const {
	return versions[pos] == version ? inner[pos] : def;
}

FlowField::FlowField(): coll(false), distances(INF), enemies(0), enemyCount(0) {}

std::array<sf::Vector2i, 2> bounds(sf::Vector2f a, sf::Vector2f b) {
	sf::Vector2i v(a.x, a.y), w(ceil(b.x), ceil(b.y));
	v.x = std::clamp(v.x, 0, FieldSize);
	v.y = std::clamp(v.y, 0, FieldSize);
	w.x = std::clamp(w.x, 0, FieldSize);
	w.y = std::clamp(w.y, 0, FieldSize);
	return { v, w };
}

template <>
void FlowField::addObstacle<f32>(sf::Vector2f pos, sf::Vector2f size) {
	pos /= (f32)SCALE, size /= (f32)SCALE;
	auto [v, w] = bounds(pos, pos + size);
	for(i32 i = v.x; i < w.x; i += 1)
		for(i32 j = v.y; j < w.y; j += 1)
			coll[i][j] = 1;
}
template <>
void FlowField::addObstacle<u32>(sf::Vector2u pos, sf::Vector2u size) {
	addObstacle(sf::Vector2f(pos.x, pos.y), sf::Vector2f(size.x, size.y));
}

constexpr bool invalid(const sf::Vector2i& pos) {
	auto& [x, y] = pos;
	return x < 0 || y < 0 || x >= FieldSize || y >= FieldSize;
}

sf::Vector2f FlowField::direction(sf::Vector2i tile) const {
	sf::Vector2f v;
	auto [x, y] = tile;
	v.x += distances[sf::Vector2u(x-1, y)];
	v.x -= distances[sf::Vector2u(x+1, y)];
	v.y += distances[sf::Vector2u(x, y-1)];
	v.y -= distances[sf::Vector2u(x, y+1)];
	if(v.x || v.y) v /= hypotf(v.x, v.y);
	return v;
}

template <>
void FlowField::addEnemy<f32>(sf::Vector2f pos, sf::Vector2f size) {
	pos /= (f32)SCALE, size /= (f32)SCALE;
	auto [v, w] = bounds(pos, pos+size);
	for(i32 i = v.x; i < w.x; i += 1)
		for(i32 j = v.y; j < w.y; j += 1)
			if(!coll[i][j]) enemies[sf::Vector2u(i,j)] += 1, enemyCount += 1;
}

template <>
void FlowField::calculate<f32>(sf::Vector2f pos, sf::Vector2f size) {
	static std::vector<sf::Vector2i> now, next;
	now.clear(), next.clear();

	pos /= (f32)SCALE, size /= (f32)SCALE;
	auto [v, w] = bounds(pos, pos + size);
	for(i32 i = v.x; i < w.x; i += 1)
		for(i32 j = v.y; j < w.y; j += 1)
			now.push_back({ i, j });

	i32 d = 0;
	distances.fill(INF);
	while(!now.empty() && enemyCount > 0) {
		for(auto& p: now) {
			if(invalid(p) || coll[p] || distances[p] <= d) continue;
			distances[p] = d;
			enemyCount -= enemies[p], enemies[p] = 0;
			for(auto q: {
				sf::Vector2i(p.x, p.y-1),
				sf::Vector2i(p.x, p.y+1),
				sf::Vector2i(p.x-1, p.y),
				sf::Vector2i(p.x+1, p.y)
			}) next.push_back(q);
		}
		d += 1;
		now.clear();
		swap(now, next);
	}
	enemies.fill(0);
	enemyCount = 0;
}

template <>
sf::Vector2f FlowField::query<f32>(sf::Vector2f pos, sf::Vector2f size) const {
	pos /= (f32)SCALE, size /= (f32)SCALE;
	sf::Vector2f v;
	for(i32 i = pos.x; i < pos.x + size.x; i += 1) {
		for(i32 j = pos.y; j < pos.y + size.y; j += 1) {
			v += direction({i, j});
		}
	}
	if(v.x || v.y) v /= hypotf(v.x, v.y);
	
	return v;
}
template <>
sf::Vector2f FlowField::query<i32>(sf::Vector2i pos, sf::Vector2i size) const {
	return query(sf::Vector2f(pos.x, pos.y), sf::Vector2f(size.x, size.y));
}

void FlowField::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto v = target.getView();
	auto s = v.getSize();
	auto p = v.getCenter() - s * 0.5f;
	p /= (f32)SCALE, s /= (f32)SCALE;
	auto [a, b] = bounds(p, p+s);
	for(i32 i = a.x; i <= b.x; i += 1) {
		for(i32 j = a.y; j <= b.y; j += 1) {
			sf::Text text;
			text.setFont(Assets::font);
			text.setString(std::to_string(distances[sf::Vector2u(i, j)]));
			text.setPosition(i*SCALE, j*SCALE);
			text.setScale(0.2, 0.2);
			target.draw(text, states);
		}
	}
}
