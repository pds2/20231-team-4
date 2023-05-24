#ifndef LOADING_H
#define LOADING_H
#include "types.h"

/// Um ajudante que desenha uma barra de carregamento na tela
class Loading {
	private:
	sf::Text text;
	f32 percent;

	public:
	Loading();
	/// Renderiza a tela de carregamento na janela
	void render(sf::RenderWindow& window);
};

#endif