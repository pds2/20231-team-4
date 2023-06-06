#include <iostream>

#include "collidable.hpp"
#include "player.hpp"
#include "enemy.hpp"

using namespace std;
using namespace sf;

void renderMovement(Collidable &obj, RenderWindow &window) {
	double body_position_x = obj.get_body()->GetPosition().x * PPM;
	double body_position_y = obj.get_body()->GetPosition().y * PPM;
	double rotation = -1*obj.get_body()->GetAngle() * DEG_PER_RAD;

	if(obj.get_sprite().getTexture() != nullptr) {
		obj.get_sprite().setPosition(body_position_x, body_position_y);
		obj.get_sprite().setRotation(rotation);

		window.draw(obj.get_sprite());
	} else {
		obj.get_sfml_shape()->setPosition(body_position_x, body_position_y);
		obj.get_sfml_shape()->setRotation(rotation);
			
		window.draw(*obj.get_sfml_shape());
	}
}

int main() {
	/*
	 * World b2 configurations
	 */
	b2World world(b2Vec2(0,0));
	MyContactListener contactListener;
	world.SetContactListener(&contactListener);
	
	/*
	 * SFML Window configurations
	 */
	RenderWindow window(VideoMode(1600, 900), "Game Test", Style::Fullscreen);
	window.setFramerateLimit(60);


	/*
	 * Objects declarations
	 */
	//Player
	Player player(1600/2, 900/2, &world, new Box(30, 30, 100.f), b2_dynamicBody, "frog.png", PlayerProperties(100, 10, 5), WeaponType::GUN);
	
	//Enemy
	vector<shared_ptr<Enemy>> enemies;
	int counter = 30;
	
	//Static
	Collidable _static(500, 500, &world, new Box(200, 200, 1.f), b2_staticBody, "", Color::White);
	Collidable _static2(1000, 300, &world, new Circle(100, 1.f), b2_staticBody, "", Color::White);
	//Start of the window loop
	while(window.isOpen()) {
		Event event;

		while(window.pollEvent(event)) {
			if(event.type == Event::Closed)
				window.close();
			if(event.type == Event::KeyPressed && Keyboard::isKeyPressed(Keyboard::Escape))
				window.close();
		}

		if(window.hasFocus()) {
			player._move(window);
			
			player._attack();
			for(auto &enemy: enemies)
				enemy->_move(player);
		} else {
			player.get_body()->SetAwake(false);
			for(auto &enemy: enemies)
				enemy->get_body()->SetAwake(false);
			for(weak_ptr projectile: player.get_weapon()->get_cartridge())
				projectile.lock()->get_body()->SetAwake(false);
		}
		if(counter >= 30) {
			enemies.push_back(make_shared<Enemy>((rand()%1000), (rand()%900), &world, new Box(20, 20, 1.f), b2_dynamicBody, "bugol.png", EnemyProperties(100, 10, 10, 3+(rand()%6-3+1))));
			counter = 0;
		} else 
			counter++;
		window.clear(Color::Black);
		world.Step(1/60.f, 6, 3);

		window.draw(*_static.get_sfml_shape());
		window.draw(*_static2.get_sfml_shape());

		renderMovement(player, window);

		auto &cartridge = player.get_weapon()->get_cartridge();

		auto it = cartridge.begin();
		while(it != cartridge.end()) {
			if(auto proj = *it) {
				renderMovement(*proj, window);

				if(proj->get_body()->GetUserData().pointer == 1) {
					it->reset();
					it = cartridge.erase(it);
				}
				else
					it++;
			}
			else {
				it = cartridge.erase(it);
			}
		}			
		for(auto& enemy: enemies) 
			renderMovement(*enemy, window);	
		window.display();
	}

	

	return 0;
}