#ifndef RENDER_H
#define RENDER_H
#include <SFML/Graphics.hpp>
#include <memory>
#include <map>
#include "types.hpp"

/// Guarda itens para serem renderizados na ordem de profundidade
class ZRenderer: public sf::Drawable {
	private:
	std::multimap<u32, std::reference_wrapper<const sf::Drawable>> objects;

	public:
	/// Limpa a lista de itens a serem renderizados
	void clear();
	/// Insere um item à lista para ser renderizado
	void insert(u32 z, const sf::Drawable& drawable);
	/// Desenha os itens que foram inseridos
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
};

#endif