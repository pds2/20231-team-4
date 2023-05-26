#ifndef LOADING_H
#define LOADING_H
#include <SFML/Graphics.hpp>
#include "types.h"
#include "state.h"

/// Um ajudante que desenha uma barra de carregamento na tela
class Loading: public State {
	private:
	sf::Text text;
	f32 percent;

	public:
	/// Cria uma nova tela de carregamento em um contexto
	Loading(Context& ctx);
	void tick() {};
	void render();
	~Loading() {};
};

#endif