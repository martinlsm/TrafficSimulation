#include <iostream>
#include <SFML/Graphics.hpp>
#include <sys/stat.h>
#include <sys/types.h>

#include "rendering.h"
#include "traffic_env.h"

using namespace traffic;

void export_environment_image(unsigned int env_id) {
	TrafficEnvironment* environment = load_environment(env_id);
	WorldRenderer world_renderer {environment->road_system->road_pieces};

    sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH, WORLD_HEIGHT), "Environment");
	window.clear({0x43,0x8a, 0x5e});
	world_renderer.draw(window);

	sf::Vector2u window_size = window.getSize();
	sf::Texture texture;
	texture.create(window_size.x, window_size.y);
	texture.update(window);
	sf::Image screenshot = texture.copyToImage();

	std::string file_name = "../env_images/env_" + std::to_string(env_id) + ".jpg";
	screenshot.saveToFile(file_name);

	delete environment->road_system;
	delete environment;
}

int main() {
	mkdir("../env_images", 777);
	export_environment_image(1);
}
