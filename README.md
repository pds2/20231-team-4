# Grupo 4
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