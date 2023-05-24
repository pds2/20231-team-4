#ifndef STATE_H
#define STATE_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "types.h"

enum class StateChange {
	None,
	Push,
	Pop,
	Into,
	Set
};

struct State;

struct StateMessage {
	StateChange type;
	union {
		State* state;
	};
	static StateMessage None();
	static StateMessage Push(State* s);
	static StateMessage Pop();
	static StateMessage Into(State* s);
	static StateMessage Set(State* s);
};

struct State {
	bool opaque;
	StateMessage message;
	State(bool opaque);
	virtual void tick() = 0;
	virtual void render(sf::RenderWindow& window) = 0;
	virtual void handleEvent(sf::Event event) = 0;
	virtual ~State() = 0;
};

class StateManager {
	private:
	std::vector<State*> stack;
	sf::RenderWindow& window;
	void clear();

	public:
	StateManager(State* initial, sf::RenderWindow& window);
	void render();
	void tick();
	~StateManager();
};

#endif