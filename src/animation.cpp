#include "animation.hpp"

Animation::Animation(Context& ctx, bool opaque):
	State(ctx, 0) {
		sf::Vector2u ws = ctx.window.getSize();
	}


void Animation::render(){
	sf::Vector2u s_window = ctx.window.getSize();
	sf::FloatRect v_rect(0, 0, s_window.x, s_window.y);
	ctx.window.setView(sf::View(v_rect));

	sf::RectangleShape backg;
	backg.setPosition({v_rect.left, v_rect.top});
	backg.setSize({v_rect.width, v_rect.height});
	backg.setFillColor(sf::Color(50, 50, 50, 122));
	ctx.window.draw(backg);

	tempo_ = 0;
	character_ = 0;
	axis_ = 0;
	u32 RectLeft_ = tempo_*32;
	u32 RectTop_ = character_*50 + axis_*50;
	texture_.loadFromFile("assets/char_texture.png");
	sprite_.setTexture(texture_);
	sprite_.setTextureRect(sf::IntRect(RectLeft_, RectTop_, 32, 50));

	sprite_.setPosition(s_window.x*0.5, s_window.y*0.5);
	sprite_.setScale(1, 1);
	ctx.window.draw(sprite_);
}

void Animation::handleEvent(sf::Event event){
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A)
        axis_ = 1;
    if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::D)
        axis_ = 0;
}