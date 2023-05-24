#ifndef LOADING_H
#define LOADING_H
#include "types.h"

class Loading {
	private:
	sf::Text text;
	f32 percent;

	public:
	Loading();
	void render(sf::RenderWindow& window);
};

#endif