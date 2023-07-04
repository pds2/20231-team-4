#include <iostream>
#include "state.hpp"

State::State(Context& ctx, bool opaque):
	opaque(opaque),
	ctx(ctx),
	message(StateMessage::None()) {}

StateMessage StateMessage::None() {
	return { StateChange::None, nullptr };
}
StateMessage StateMessage::Push(unique_state s) {
	return { StateChange::Push, std::move(s) };
}
StateMessage StateMessage::Pop() {
	return { StateChange::Pop, nullptr };
}
StateMessage StateMessage::Into(unique_state s) {
	return { StateChange::Into, std::move(s) };
}
StateMessage StateMessage::Set(unique_state s) {
	return { StateChange::Set, std::move(s) };
}
StateMessage::StateMessage(StateChange type, unique_state state):
	type(type),
	state(std::move(state)) {}
StateMessage::StateMessage(StateMessage&& sm):
	type(StateChange::None),
	state(nullptr)
{
	*this = std::move(sm);
}
StateMessage& StateMessage::operator=(StateMessage&& sm) {
	type = sm.type;
	std::unique_ptr<State> p = sm.takeState();
	state.swap(p);
	return *this;
}
std::unique_ptr<State> StateMessage::takeState() {
	type = StateChange::None;
	return std::move(state);
}
StateMessage::~StateMessage() {
	type = StateChange::None;
}

StateManager::StateManager(Context& ctx, std::unique_ptr<State> initial):
	stack(),
	ctx(ctx)
{
	stack.push_back(std::move(initial));
}

void StateManager::tick() {
	if(stack.empty()) {
		ctx.window.close();
		return;
	}
	std::unique_ptr<State>& state = stack.back();

	sf::Event event;
	while(ctx.window.pollEvent(event)) {
		if(event.type == sf::Event::Closed) ctx.window.close();
		state->handleEvent(event);
	}
	state->tick();

	StateMessage msg = std::move(state->message);
	switch(msg.type) {
		case StateChange::Push:
			stack.push_back(std::move(msg.takeState()));
			break;
		case StateChange::Pop:
			stack.pop_back();
			break;
		case StateChange::Into:
			stack.back() = msg.takeState();
			break;
		case StateChange::Set:
			stack.clear();
			stack.push_back(std::move(msg.takeState()));
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
