# Grupo 4
## Integrantes
- Daniel Vítor Rabelo Rodrigues
- Iago Nathan Cardoso Araújo
- Mateus Vitor Mota Vasconcelos
- Matheus Costa Faria
## Projeto
Um jogo RPG roguelike
## User Stories
- Como jogador, quero poder escolher meus personagens.
- Como jogador, quero poder escolher uma arma inicial.
- Como jogador, quero poder melhorar meu personagem.
- Como jogador, quero obter itens melhores ao longo do jogo.
- Como jogador, quero ter diferentes inimigos.
- Como jogador, quero ter vários locais para visitar.
- Como jogador, quero poder salvar meu progresso.

## Sistema
O jogo será desenvolvido na linguagem c++ com a biblioteca SFML. Teremos personagens, background e animações pixeladas.

### Criação da partida
Ao iniciar o programa, o usuário deverá ter uma escolha entre criar um novo jogo ou, se houver, continuar de onde salvou anteriormente. 

Após isso o usuário será direcionado a uma tela de opções, onde o usuário poderá escolher um personagem. Cada personagem terá propriedades e aparências únicas.

Então o usuário será direcionado a outra tela de opções, onde o usuário poderá escolher uma arma. Cada arma terá uma habilidade única.

Por fim o usuário será direcionado a tela onde a primeira fase do jogo ocorre.

### Mecânicas do jogo
- O jogador poderá se mover pelo mapa em um plano (2D).
- Os inimigos irão spawnar das bordas da tela e continuamente perseguiram o jogador, o jogador terá que sobreviver o quanto possível fugindo e matando os inimigos com sua(s) arma(s);
- Cada inimigo morto irá dar o personagem uma quantidade de xp aleatória, quando mais forte o inimigo mais xp ele oferece;
- A cada nível que o jogador alcança ele terá opções de evoluir uma de suas propriedades;
    - Cada personagem terá maior afinidade com certas propriedades e menos com outras (ex.: Brawler tem mais afinidade com defesa e menos afinidade com agilidade).
    - Em ocasiões mais raras o jogador terá o opção de adicionar uma nova arma a suas habilidades, que terá propriedades únicas e atacará automaticamente os inimigos.
        - A cada arma nova escolhida as mais opções de evolução serão adicionadas relacionadas à nova arma.
- A ***frequência*** e ***força*** dos inimigos aumentarão conforme o personagem evolui;
- A variedade de inimigos aumentará em função do tempo;
    - Cada espécie e subespécie de inimigo terá propriedades e afinidades diferentes.
- O jogador de tempos em tempos terá a opção de **salvar seu progresso**.

Todo **jogador** terá as seguintes propriedades:

- Vida;
- Defesa;
- Agilidade;
- Uma arma.

Toda **arma** terá as seguintes propriedades:

- Dano;
- Velocidade;
- Frequencia de tiros;
- Projeteis:
    
    cada projetel terá por sua vez uma propriedade única (ex.: maior knockback, congelamento, queima).
    

Todo **inimigo** terá as seguintes propriedades:

- Vida;
- Dano;
- Defesa;
- Agilidade.


### Teclas de Controle
- W - mover para cima;
- S - mover para baixo;
- A - mover para a esquerda;
- D - mover para a direita;
- Espaço - atira projeteis;
- Ponteiro do mouse - ajusta a direção dos projeteis.


## Compilando
O Makefile está configurado para baixar e compilar as dependências, isso só acontecerá uma vez.

Para rodar testes:
```sh
make test
```
Para rodar o programa:
```sh
make debug && target/debug/main
make release && target/release/main
```
