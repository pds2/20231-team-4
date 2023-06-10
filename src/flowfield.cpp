#include <iostream>
#include <cmath>
#include <map>
#include <numeric>
#include "assets.hpp"
#include "flowfield.hpp"
const i32 SCALE = 16;
const i32 MAX16 = std::numeric_limits<i16>::max();
FlowField::FlowField() {
	coll[0].fill(0);
	coll.fill(coll[0]);
	distances[0].fill(MAX16);
	distances.fill(distances[0]);
}

bool FlowField::invalid(i32 x, i32 y) const {
	return x < 0 || x >= FFieldM || y < 0 || y >= FFieldN;
}

void FlowField::addObstacle(i32 x, i32 y, i32 w, i32 h) {
	x /= SCALE, y /= SCALE, w /= SCALE, h /= SCALE;
	for(i32 i = x; i < x+w; i += 1)
		for(i32 j = y; j < y+w; j += 1)
			if(!invalid(i, j)) coll[i][j] = 1;
}

sf::Vector2f FlowField::direction(i32 x, i32 y) const {
	sf::Vector2f v;
	v.x += invalid(x-1, y) ? MAX16 : distances[x-1][y];
	v.x -= invalid(x+1, y) ? MAX16 : distances[x+1][y];
	v.y += invalid(x, y-1) ? MAX16 : distances[x][y-1];
	v.y -= invalid(x, y+1) ? MAX16 : distances[x][y+1];
	if(v.x || v.y) v /= hypotf(v.x, v.y);
	return v;
}

void FlowField::calculate(i32 x, i32 y, i32 w, i32 h) {
	x /= SCALE, y /= SCALE, w /= SCALE, h /= SCALE;
	distances[0].fill(MAX16);
	distances.fill(distances[0]);

	std::multimap<f32, std::pair<i32, i32>> queue;
	for(i32 i = x; i < x+w; i += 1)
		for(i32 j = y; j < y+w; j += 1)
			queue.insert({0, {i, j}});
	while(!queue.empty()) {
		auto it = queue.begin();
		auto [d, p] = *it;
		queue.erase(it);
		auto [x, y] = p;

		if(invalid(x, y) || coll[x][y]) continue;
		bool worse = distances[x][y] != -1 && d >= distances[x][y];
		bool tooFar = d > 20;
		if(worse || tooFar) continue;

		distances[x][y] = d;
		if(!invalid(x-1, y)) queue.insert({d+1, {x-1, y}});
		if(!invalid(x, y-1)) queue.insert({d+1, {x, y-1}});
		if(!invalid(x+1, y)) queue.insert({d+1, {x+1, y}});
		if(!invalid(x, y+1)) queue.insert({d+1, {x, y+1}});
	}
	for(i32 i = 0; i < FFieldM; i += 1)
		for(i32 j = 0; j < FFieldN; j += 1)
			if(!invalid(i, j)) vectors[i][j] = direction(i, j);
}

sf::Vector2f FlowField::query(i32 x, i32 y, i32 w, i32 h) const {
	x /= SCALE, y /= SCALE, w /= SCALE, h /= SCALE;
	sf::Vector2f v;
	for(i32 i = x; i < x+w; i += 1)
		for(i32 j = y; j < y+h; j += 1)
			v += vectors[i][j];
	if(v.x || v.y) v /= hypotf(v.x, v.y);
	return v;
}

void FlowField::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	auto v = target.getView();
	auto s = v.getSize();
	auto p = v.getCenter() - s * 0.5f;
	i32 x = p.x / SCALE, y = p.y / SCALE, w = s.x / SCALE, h = s.y / SCALE;
	for(i32 i = x; i < x+w; i += 1) {
		for(i32 j = y; j < y+h; j += 1) {
			if(invalid(i, j)) continue;
			sf::Text text;
			text.setFont(Assets::font);
			text.setString(std::to_string(distances[i][j]));
			text.setPosition(i*SCALE, j*SCALE);
			text.setScale(0.2, 0.2);
			target.draw(text, states);
		}
	}
}