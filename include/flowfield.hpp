#ifndef FLOW_H
#define FLOW_H
#include <SFML/Graphics.hpp>
#include <array>
#include "types.hpp"

const i32 FieldSize = 256;

template<typename T, size_t S>
struct Array2: std::array<std::array<T, S>, S> {
	Array2(const T& initial);
	void fill(const T& initial);
	template <typename I>
	T& operator[](sf::Vector2<I> pos);
	template <typename I>
	const T& operator[](sf::Vector2<I> pos) const;
	template <typename I>
	std::array<T, S>& operator[](I index);
	template <typename I>
	const std::array<T, S>& operator[](I index) const;
};

/// Um mapa de fluxos que determina a direção que um inimigo deve andar
class FlowField: public sf::Drawable {
	private:
	Array2<bool, FieldSize> coll;
	Array2<i16, FieldSize> distances;
	Array2<i16, FieldSize> version;
	Array2<u8, FieldSize> enemies;
	std::vector<sf::Vector2i> enemyPositions;
	u32 enemyCount;
	u32 currentVersion;

	i16 distance(sf::Vector2i tile) const;
	sf::Vector2f direction(sf::Vector2i tile) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
	FlowField();
	/// Adiciona um inimigo ao mapa. Os inimigos são esquecidos a cada chamada de \ref calculate
	template <typename I>
	void addEnemy(sf::Vector2<I> pos, sf::Vector2<I> size);
	/// Adiciona um obstáculo ao mapa de fluxo
	template <typename I>
	void addObstacle(sf::Vector2<I> pos, sf::Vector2<I> size);
	/// Calcula os vetores para chegar a uma posição
	template <typename I>
	void calculate(sf::Vector2<I> pos, sf::Vector2<I> size);
	/// Retorna a direção do fluxo em uma posição
	template <typename I>
	sf::Vector2f query(sf::Vector2<I> pos, sf::Vector2<I> size) const;
};

#endif