#include "render.hpp"

void ZRenderer::clear() {
	objects.clear();
}

void ZRenderer::insert(u32 z, const sf::Drawable& drawable) {
	objects.insert({z, std::cref(drawable)});
}

void ZRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	for(auto& [_, o]: objects) target.draw((sf::Drawable&)o.get(), states);
}
