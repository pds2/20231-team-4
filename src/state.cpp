#include <iostream>
#include "state.h"

State::State(Context& ctx, bool opaque):
	opaque(opaque),
	ctx(ctx),
	message(StateMessage::None()) {}
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

StateManager::StateManager(Context& ctx, State* initial):
	stack({initial}),
	ctx(ctx) {}
StateManager::~StateManager() {
	clear();
}
void StateManager::clear() {
	for(State* s: stack) delete s;
}

void StateManager::tick() {
	if(stack.empty()) {
		ctx.window.close();
		return;
	}
	State* state = stack.back();

	sf::Event event;
	while(ctx.window.pollEvent(event)) {
		if(event.type == sf::Event::Closed) ctx.window.close();
		state->handleEvent(event);
	}
	state->tick();

	StateMessage msg = state->message;
	switch(msg.type) {
		case StateChange::Push:
			stack.push_back(msg.state);
			state->message = StateMessage::None();
			break;
		case StateChange::Pop:
			stack.pop_back();
			delete state;
			break;
		case StateChange::Into:
			stack.back() = msg.state;
			delete state;
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
		ctx.window.close();
		return;
	}
	ctx.window.clear(sf::Color::Black);

	u32 i = stack.size() - 1;
	while(i && !stack[i]->opaque) i -= 1;
	while(i < stack.size())
		stack[i++]->render();

	ctx.window.display();
}