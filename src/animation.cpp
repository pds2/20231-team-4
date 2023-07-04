#include "animation.h"

Animation::Animation(Context& ctx, Game& game, bool opaque):
  game(game),
	opaque(opaque),
	ctx(ctx),
	message(StateMessage::None()) {}


void Animation::render(){
  u32 tempo_ = 0;
  u32 RectLeft_ = (tempo_+1)*16 + tempo*32 + (axis_*16);
  u32 RectTop_ = character_*16 + (character_-1)*50;
  u32 RectWidth_ = 32*(axis_*-1);
  texture_.loadFromFile("assets/char_texture_test.png");
	sprite_.setTexture(texture_);
	sprite_.setTextureRect(sf::IntRect(RectLeft_, RectTop_, RectWidth_, 50));
  sprite_.setPosition(0, 0);
	sprite_.setScale(1, 1);
	ctx.window.draw(sprite_);
}
 
void Animation::switchGun(){

}

void Animation::handleEvent(sf::Event event){
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
        axis_ = 1;
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
        axis_ = 0;
    
}