#ifndef TEXTTAGSYSTEM_H
#define TEXTTAGSYSTEM_H
#include <SFML/Graphics.hpp> 
#include "assets.hpp"

enum TAGTYPES {DEFAULT_TAG, PLAYERDAMAGE_TAG, ENEMYDAMAGE_TAG, EXPERIENCE_TAG};

class TextTagSystem {
private:
    class TextTag { 
    private:
        sf::Text text;
        float dirY;
        float dirX;
        float lifetime;
        float speed;

    public:
        TextTag(sf::Font& font, std::string text, 
        float pos_x, float pos_y, 
        float dir_x, float dir_y,
        sf::Color color, unsigned char_size, 
        float lifetime, float speed)
        {
            this->text.setFont(font);
            this->text.setPosition(pos_x, pos_y);
            this->text.setFillColor(color);
            this->text.setCharacterSize(char_size);
            this->text.setString(text);

            this->dirX = dir_x;
            this->dirY = dir_y; 
            this->lifetime = lifetime;
            this->speed = speed;
        }

        TextTag(TextTag* tag, float pos_x, float pos_y, std::string str)
        {
            this->text = tag->text;
            this->text.setPosition(pos_x, pos_y);
            this->text.setString(str);

            this->dirX = tag->dirX;
            this->dirY = tag->dirY; 
            this->lifetime = tag->lifetime;
            this->speed = tag->speed;
        }
        
        ~TextTag()
        {

        }

        const bool isExpired() const {return lifetime <= 0.f;}

        void update(const float& dt)
        {
            if(lifetime > 0.f) {
                this->lifetime -= 10.f * dt;

                this->text.move(this->dirX * this->speed * dt, this->dirY * this->speed * dt);
            }
        }
    
        void render(sf::RenderTarget& target)
        {
            target.draw(text);
        }   

    };

    sf::Font font;
    std::map<unsigned, TextTag*> tagTemplates;
    std::vector<TextTag*> tags;

    void initTagTemplates();

public:
    TextTagSystem();
    virtual ~TextTagSystem();

    void addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const std::string str);
    void addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const std::string pefix, const int val, const std::string postfix);
    void addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const std::string prefix, const double val, const std::string postfix);

    void update(const float &dt);
    void render(sf::RenderTarget& target);
};

#endif