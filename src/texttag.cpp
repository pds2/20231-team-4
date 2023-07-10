#include "texttags.hpp"
#include <sstream>

void TextTagSystem::initTagTemplates() {
    this->tagTemplates[DEFAULT_TAG] = new TextTag(this->font, "", 0.f, 0.f, 0.f, -1.f, sf::Color::White, 12, 10.f, 10.f);
    this->tagTemplates[EXPERIENCE_TAG] = new TextTag(this->font, "", 0.f, 0.f, 0.f, -1.f, sf::Color::Green, 12, 10.f, 10.f);
    this->tagTemplates[ENEMYDAMAGE_TAG] = new TextTag(this->font, "", 0.f, 0.f, 0.f, -1.f, sf::Color::Red, 12, 10.f, 10.f);
    this->tagTemplates[PLAYERDAMAGE_TAG] = new TextTag(this->font, "", 0.f, 0.f, 0.f, 1.f, sf::Color::Red, 12, 10.f, 10.f);
}

TextTagSystem::TextTagSystem() : font(Assets::font) {
    this->initTagTemplates();
}

TextTagSystem::~TextTagSystem() {
    for(auto* tag: this->tags) {
        delete tag;
    }

    for(auto& tag: this->tagTemplates) {
        delete tag.second;
    }
}

void TextTagSystem::addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const std::string str) {
    this->tags.push_back(new TextTag(this->tagTemplates[tag_type], pos_x, pos_y, str));
}

void TextTagSystem::addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const std::string prefix, const int val, const std::string postfix) {

    std::stringstream ss;
    ss << prefix << " " << val << " " << postfix;

    this->tags.push_back(new TextTag(this->tagTemplates[tag_type], pos_x, pos_y, ss.str()));
}

void TextTagSystem::addTextTag(const unsigned tag_type, const float pos_x, const float pos_y, const std::string prefix, const double val, const std::string postfix) {

    std::stringstream ss;
    ss << prefix << " " << val << " " << postfix;

    this->tags.push_back(new TextTag(this->tagTemplates[tag_type], pos_x, pos_y, ss.str()));
}

void TextTagSystem::update(const float &dt) {
    for(size_t i = 0; i < this->tags.size(); i++) {
        this->tags[i]->update(dt);

        if(this->tags[i]->isExpired()) {
            delete this->tags[i];
            this->tags.erase(this->tags.begin() + i);
        }
    }
}

void TextTagSystem::render(sf::RenderTarget& target) {
    for(auto& tag: this->tags) {
        tag->render(target);
    }
} 