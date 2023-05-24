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

/// Uma mensagem que altera o estado de jogo
struct StateMessage {
	/// O tipo de mensagem
	/// Alguns tipos não contém \ref StateMessage.state
	StateChange type;
	union {
		/// Um estado de destino
		State* state;
	};
	/// Uma mensagem vazia
	static StateMessage None();
	/// Uma mensagem para adicionar um novo estado de jogo à pilha
	static StateMessage Push(State* s);
	/// Uma mensagem para retornar a um estado anterior (Deleta o estado atual)
	static StateMessage Pop();
	/// Uma mensagem para transformar o estado atual (Deleta o estado atual)
	static StateMessage Into(State* s);
	/// Uma mensagem para limpar a pilha de estados e começar com um novo
	static StateMessage Set(State* s);
};

/// \brief Uma interface que representa um estado de jogo.
/// Os estados devem herdar dessa classe, e manter \ref StateMessage "mensagens" para o \ref StateManager "gerenciador"
struct State {
	/// A opacidade desse estado. Se o estado for transparente, o estado anterior é renderizado.
	bool opaque;
	/// A mensagem a ser lida pelo gerenciador de estados
	StateMessage message;
	/// Cria um novo estado com a opacidade especificada
	State(bool opaque);
	/// Calcula um próximo estado de jogo
	virtual void tick() {};
	/// Desenha o estado na janela
	virtual void render(sf::RenderWindow& window) = 0;
	/// Recebe um evento da janela
	virtual void handleEvent(sf::Event event) = 0;
	virtual ~State() = 0;
};

/// Um gerenciador de estados de jogo
class StateManager {
	private:
	std::vector<State*> stack;
	sf::RenderWindow& window;
	void clear();

	public:
	/// Cria um novo gerenciador de estados com o estado especificado
	StateManager(State* initial, sf::RenderWindow& window);
	/// Renderiza os estados, em ordem de inserção, obedecendo transparência
	void render();
	/// Calcula o próximo estado de jogo
	void tick();
	~StateManager();
};

#endif