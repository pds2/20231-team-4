#include "../include/GameUtilities.hpp"

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
