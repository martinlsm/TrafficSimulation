#include "texture_manager.h"
#include <exception>

namespace texture_manager {
sf::Texture* car_texture;

void load_textures() {
	car_texture = new sf::Texture();
	if (!car_texture->loadFromFile("../assets/car.png")) {
		throw "car texture could not be found";
	}
}

void delete_textures() {
	delete car_texture;
}

} // namespace texture_manager
