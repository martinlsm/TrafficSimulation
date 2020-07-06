#include <iostream>

#include <SFML/Graphics.hpp>

#include "car.h"
#include "car_actions.h"
#include "constants.h"
#include "roadway.h"

using namespace traffic;

class StraightRoadSprite {
private:
	StraightRoad *road;
	sf::RectangleShape rect;

	sf::Transform getTransform() {
		Vec2d<float> crossing_a = road->getCrossingA();
		Vec2d<float> crossing_b = road->getCrossingB();
		float x = crossing_b.x - crossing_a.x;
		float y = crossing_b.y - crossing_a.y;
		float rotation = 180.0f / M_PI * std::atan(y / x);
		sf::Transform t = sf::Transform::Identity;
		t.translate(crossing_a.x, crossing_a.y);
		t.rotate(rotation);
		t.translate(0, -road->getWidth() / 2.0f);
		return t;
	}
public:
	StraightRoadSprite(StraightRoad *road)
			: road(road),
			rect(sf::Vector2f(road->getLength(), road->getWidth())) {}

	void draw(sf::RenderWindow &window) {
		sf::Transform transform = this->getTransform();
		window.draw(rect, transform);
	}
};

class CarSprite {
private:
	CarBody *car_body;
	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Transform getTransform() {
		Vec2d<float> position = car_body->getPos();
		Vec2d<float> size = car_body->getSize();
		float rotation = 180.0f / M_PI * car_body->getRotation();

		sf::Transform t = sf::Transform::Identity;
		t.translate(position.x, position.y);
		t.rotate(rotation);
		t.rotate(90.0f);
		t.translate(-size.x / 2.0f, -size.y / 2.0f);
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
	CarBody car{100, 100};
	CarAction::CarActionController car_action{&car};
	CarSprite car_sprite{&car};

	Vec2d<float> crossing_a {100, 100};
	Vec2d<float> crossing_b {700, 700};
	StraightRoad road {crossing_a, crossing_b, 75};
	StraightRoadSprite road_sprite {&road};

	sf::Clock clock;
    sf::RenderWindow window(sf::VideoMode(WORLD_WIDTH, WORLD_HEIGHT), "Traffic Simulation");
	window.setFramerateLimit(FRAME_RATE);

    while (window.isOpen()) {
		// handle input
        sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed
					|| event.key.code == sf::Keyboard::Q) {
				window.close();
			}

			if (event.key.code == sf::Keyboard::W) {
				car_action.doAction(CarAction::GAS_MEDIUM);
			} else if (event.key.code == sf::Keyboard::S) {
				car_action.doAction(CarAction::BRAKE_MEDIUM);
			} else if (event.key.code == sf::Keyboard::A) {
				car_action.doAction(CarAction::TURN_LEFT_HARD);
			} else if (event.key.code == sf::Keyboard::D) {
				car_action.doAction(CarAction::TURN_RIGHT_HARD);
			}
		}

		// update state
		sf::Time dt = clock.restart();
		float dt_sec = dt.asSeconds();
		car.update(dt_sec);

		std::cout << road.inside(car) << std::endl;

		// render screen
        window.clear();
		road_sprite.draw(window);
		car_sprite.draw(window);
        window.display();
    }

    return 0;
}
