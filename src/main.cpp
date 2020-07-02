#include <iostream>

#include <SFML/Graphics.hpp>

#include "car.h"
#include "car_actions.h"

class CarSprite {
private:
	CarBody *car_body;
	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Transform getTransform() {
		Vec2d<float> position = car_body->getPos();
		Vec2d<float> size = car_body->getSize();
		float rotation = car_body->getRotation();

		sf::Transform t = sf::Transform::Identity;
		t.translate(size.x / 2.0f, size.y / 2.0f);
		t.rotate(360.0f / (2 * M_PI) * rotation);
		t.translate(position.x, position.y);
		return t;
	}

public:
	CarSprite(CarBody *car_body) {
		this->car_body = car_body;

		// load car image asset
		std::string asset_path = "../assets/car.png";
		if (!texture.loadFromFile(asset_path)) {
			std::cerr << asset_path << std::endl;
			exit(1);
		}

		sprite.setTexture(texture);
	}	

	void draw(sf::RenderWindow &window) {
		sf::Transform transform = this->getTransform();
		window.draw(sprite, transform);
	}
};

int main() {
	CarBody car{400, 300};
	CarAction::CarActionController car_action{&car};
	CarSprite car_sprite{&car};

	sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(800, 600), "Traffic Simulation");

    while (window.isOpen()) {

		// handle input
        sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
			}

			if (event.key.code == sf::Keyboard::W) {
				car_action.doAction(CarAction::GAS_LIGHT);
			}

			if (event.key.code == sf::Keyboard::Escape) {
				std::cout << "the escape key was pressed" << std::endl;
				std::cout << "control:" << event.key.control << std::endl;
				std::cout << "alt:" << event.key.alt << std::endl;
				std::cout << "shift:" << event.key.shift << std::endl;
				std::cout << "system:" << event.key.system << std::endl;
			}
		}

		// update state
		sf::Time dt = clock.restart();
		float dt_sec = dt.asSeconds();
		car.update(dt_sec);

		// render screen
        window.clear();
		car_sprite.draw(window);
        window.display();
    }

    return 0;
}
