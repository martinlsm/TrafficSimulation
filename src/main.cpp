#include <iostream>

#include <SFML/Graphics.hpp>

#include "car.h"

sf::Transform carTransform(CarBody &car) {
	Vec2d<float> position = car.getPos();
	Vec2d<float> size = car.getSize();
	float rotation = car.getRotation();

	sf::Transform t = sf::Transform::Identity;
	t.translate(size.x / 2.0f, size.y / 2.0f);
	t.rotate(360.0f / (2 * M_PI) * rotation);
	t.translate(position.x, position.y);
	return t;
}

int main() {
	CarBody car {50, 50};
	sf::Texture texture;
	if (!texture.loadFromFile("../assets/car.png")) {
		std::cout << "Could not load asset\n";
		return 1;
	}
	sf::Sprite sprite;
	sprite.setTexture(texture);

    sf::RenderWindow window(sf::VideoMode(800, 600), "Traffic Simulation");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
		window.draw(sprite, carTransform(car));
		texture.update(window);
        window.display();
    }

    return 0;
}
