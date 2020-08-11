#pragma once

#include <SFML/Graphics.hpp>

namespace texture_manager {
	extern sf::Texture* car_texture;

	void load_textures();
	void delete_textures();
} // namespace texture_manager
