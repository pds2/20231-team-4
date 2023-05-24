#include <iostream>
#include "state.h"

State::State(bool opaque):
	opaque(opaque),
	message({ .type=StateChange::None }) {}
State::~State() {}

StateMessage StateMessage::None() {
	return { .type=StateChange::None };
}
StateMessage StateMessage::Push(State* s) {
	return { .type=StateChange::Push, .state=s };
}
StateMessage StateMessage::Pop() {
	return { .type=StateChange::Pop };
}
StateMessage StateMessage::Into(State* s) {
	return { .type=StateChange::Into, .state=s };
}
StateMessage StateMessage::Set(State* s) {
	return { .type=StateChange::Set, .state=s };
}

StateManager::StateManager(State* initial, sf::RenderWindow& window):
	stack({initial}),
	window(window) {}
StateManager::~StateManager() {
	clear();
}
void StateManager::clear() {
	for(State* s: stack) delete s;
}

void StateManager::tick() {
	if(stack.empty()) {
		window.close();
		return;
	}
	State* state = stack.back();

	sf::Event event;
	while(window.pollEvent(event)) {
		if(event.type == sf::Event::Closed) window.close();
		state->handleEvent(event);
	}
	state->tick();

	StateMessage msg = state->message;
	switch(msg.type) {
		case StateChange::Push:
			stack.push_back(msg.state);
			break;
		case StateChange::Pop:
			delete state;
			stack.pop_back();
			break;
		case StateChange::Into:
			delete state;
			stack.back() = msg.state;
			break;
		case StateChange::Set:
			clear();
			stack = {msg.state};
			break;
		case StateChange::None:
			break;
	}
}

void StateManager::render() {
	if(stack.empty()) {
		window.close();
		return;
	}
	window.clear(sf::Color::Black);

	u32 i = stack.size() - 1;
	while(i && !stack[i]->opaque) i -= 1;
	while(i < stack.size())
		stack[i++]->render(window);

	window.display();
}