#include "../include/GameUtilities.hpp"

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
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game Test", Style::Default);
    window.setFramerateLimit(60);


    /*
     * Objects declarations
     */
    //Player
    Player player(WINDOW_WIDTH/2, WINDOW_HEIGHT/2, new Box(30, 30, 0.f), b2_dynamicBody, "frog.png", &world, playerProperties(100, 10, 3),weapon::GUN);
    
    //Enemy
    vector<Enemy*> enemies;
    for(int i{0}; i<100; i++) {
        enemies.push_back(new Enemy(250+(rand()%800-300+1), 50+(rand()%600-50+1), new Box(20, 20, 1.f), b2_dynamicBody, "bugol.png", &world, enemyProperties(100, 10, 10, 1)));
    }

    //Start of the window loop
    while(window.isOpen()) {
        Event event;

        while(window.pollEvent(event)) {
            if(event.type == Event::Closed)
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


        window.clear(Color::Black);
        world.Step(1/60.f, 6, 3);


        renderMovement(player, window);
        for(weak_ptr projectile: player.get_weapon()->get_cartridge())
            renderMovement(*projectile.lock(), window);
        for(auto &enemy: enemies)
            renderMovement(*enemy, window);
        
        window.display();
    }

    for(auto &enemy: enemies)
        delete enemy;

    return 0;
}