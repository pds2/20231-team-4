#ifndef RENDER_H
#define RENDER_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include "types.hpp"

class ZRenderer: public sf::Drawable {
	private:
	std::multimap<u32, std::reference_wrapper<const sf::Drawable>> objects;

	public:
	void clear();
	void insert(u32 z, const sf::Drawable& drawable);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif