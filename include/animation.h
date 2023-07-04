#ifndef ANIMATION_H
#define ANIMATION_H
#include <SFML/Graphics.hpp>
#include "types.h"
#include "game.h"
#include "state.h"

class Animation : public State {
    public:
    /// Cria um novo render de personagem vazio
	Animation(Context& ctx, bool opaque = 0); 
    /// cria a animação do personagem controlado pelo usuário, recebendo o personagem selecionado e a arma inicial selecionada
    Animation(Context& ctx, Game& game, bool opaque = 0);
    ///
    void render();
    /// troca a arma do usuário
    void switchGun(); /* deve ser chamada na implementação do "level up"
                      ou de uma ocasional loja de itens */
    /// lida com  os inputs de mouse e teclado, alterando o estado do personagem e arma de acord com a direção do cursor e movimento
    void handleEvent(sf::Event event);

    private: 
    /// contador de tempo
    u32 tempo;
    /// textura do personagem
    sf::Texture texture_;
    /// sprite do personagem com a arma
    sf::Sprite sprite_;
    /// armazena o número correpondente ao personagem na spritesheet
    u32 character_;
    /// arquivo da arma selecionada
    std::string gun_texture_file;
    /// coeficiente da direção horizontal do personagem
    u32 axis_; /* valor = 0 mantém o eixo y, valor = 1 inverte o eixo y */
    /// coeficiente da direção do cursor
    u32 direction_; /* será subsituído por uma implementação que 
                    limita a animação do personagem a um número
                    menor de direções paa a arma */
};

#endif