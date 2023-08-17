#pragma once
#include "SFML/Graphics.hpp"
#include <filesystem>
#include <unordered_map>

class textures {
private:
	static std::unordered_map<std::string, sf::Texture*> textures_map;

public:
	static void initializeTexture();
	static sf::Texture* getTexture(std::string filename);
	static void displayImage(sf::RenderWindow& window, sf::Texture* texture, float position_x, float position_y, sf::Vector2f size);
};