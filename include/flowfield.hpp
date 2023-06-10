#ifndef FLOW_H
#define FLOW_H
#include <SFML/Graphics.hpp>
#include <array>
#include "types.hpp"

const i32 FFieldM = 100;
const i32 FFieldN = 100;

template<typename T, i32 N, i32 M>
using Matrix = std::array<std::array<T, N>, M>;

/// Um mapa de fluxos que determina a direção que um inimigo deve andar
class FlowField: public sf::Drawable {
	private:
	Matrix<bool, FFieldM, FFieldN> coll;
	Matrix<i16, FFieldM, FFieldN> distances;
	Matrix<sf::Vector2f, FFieldM, FFieldN> vectors;
	bool invalid(i32 x, i32 y) const;
	sf::Vector2f direction(i32 x, i32 y) const;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	public:
	FlowField();
	/// Adiciona um obstáculo ao mapa de fluxo
	void addObstacle(i32 x, i32 y, i32 w, i32 h);
	/// Calcula os vetores para chegar a uma posição
	void calculate(i32 x, i32 y, i32 w, i32 h);
	/// Retorna a direção do fluxo em uma posição
	sf::Vector2f query(i32 x, i32 y, i32 w, i32 h) const;
};

#endif