#ifndef FLOW_H
#define FLOW_H
#include <SFML/Graphics.hpp>
#include <array>
#include "types.hpp"

const i32 FieldSize = 256;

/// Uma array de duas dimensões
template<typename T, size_t S>
struct Array2: std::array<std::array<T, S>, S> {
	/// Constrói a array com um valor inicial
	Array2(const T& initial);
	/// Preenche a array com um valor inicial
	void fill(const T& initial);
	/// Acessa um item na array em uma dada posição
	template <typename I>
	T& operator[](sf::Vector2<I> pos);
	/// Acessa um item na array em uma dada posição
	template <typename I>
	const T& operator[](sf::Vector2<I> pos) const;
	/// Acessa uma coluna na array em uma dada posição
	template <typename I>
	std::array<T, S>& operator[](I index);
	/// Acessa uma coluna na array em uma dada posição
	template <typename I>
	const std::array<T, S>& operator[](I index) const;
};
/// Uma array 2D com versionamento, que faz com que o preenchimento seja feito em tempo constante
template <typename T, size_t S>
class VArray2 {
	private:
	T def;
	Array2<T, S> inner;
	Array2<u32, S> versions;
	u32 version;

	public:
	/// Constrói uma array com o valor inicial
	VArray2(const T& initial);
	/// Preenche a array com o valor inicial
	void fill(const T& initial);
	/// Acessa um item na array em uma dada posição
	template <typename I>
	T& operator[](sf::Vector2<I> pos);
	/// Acessa um item na array em uma dada posição
	template <typename I>
	const T& operator[](sf::Vector2<I> pos) const;
};

/// Um mapa de fluxos que determina a direção que um inimigo deve andar
class FlowField: public sf::Drawable {
	private:
	Array2<bool, FieldSize> coll;
	VArray2<i16, FieldSize> distances;
	VArray2<u8, FieldSize> enemies;
	u32 enemyCount;

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