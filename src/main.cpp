#include <iostream>

#include <SFML/Graphics.hpp>
#include <vector>

#include "car.h"
#include "car_actions.h"
#include "constants.h"
#include "roadway.h"

using namespace traffic;

class RoadPieceSprite {
public:
	virtual void draw(sf::RenderWindow &window) = 0;
};

class StraightRoadSprite : public RoadPieceSprite {
private:
	sf::Transform transform;
	sf::RectangleShape rect;
public:
	StraightRoadSprite(StraightRoad *road)
			: RoadPieceSprite(),
			rect(sf::Vector2f(road->getLength(), road->getWidth())),
			transform(sf::Transform::Identity) {
		Vec2d<float> c_a = road->getEndpointA();
		Vec2d<float> c_b = road->getEndpointB();
		float x = c_b.x - c_a.x;
		float y = c_b.y - c_a.y;
		float rotation = 180.0f / M_PI * std::atan2(y, x);
		transform.translate(c_a.x, c_a.y);
		transform.rotate(rotation);
		transform.translate(0, -road->getWidth() / 2.0f);

		rect.setFillColor(sf::Color(67, 67, 67));
	}

	void draw(sf::RenderWindow &window) {
		window.draw(rect, transform);
	}
};

class CrossingSprite : public RoadPieceSprite {
private:
	sf::CircleShape circ;
public:
	CrossingSprite(Crossing *crossing) : RoadPieceSprite() {
		float radius = crossing->getRadius();
		circ.setRadius(radius);
		Vec2d<float> pos = crossing->getPos();
		circ.setPosition(pos.x - radius, pos.y - radius);
		circ.setFillColor(sf::Color(67, 67, 67));
	}

	void draw(sf::RenderWindow &window) {
		window.draw(circ);
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

	Roadway roadway;
	Vec2d<float> p_a {100, 100};
	Vec2d<float> p_b {400, 400};
	Vec2d<float> p_c {700, 100};
	StraightRoad road_a {p_a, p_b, 75};
	StraightRoad road_b {p_b, p_c, 75};
	Crossing c_a {p_a, 75.0f / 2};
	Crossing c_b {p_b, 75.0f / 2};
	roadway.addRoadPiece(&road_a);
	roadway.addRoadPiece(&road_b);
	roadway.addRoadPiece(&c_a);
	roadway.addRoadPiece(&c_b);

	StraightRoadSprite road_sprite_a {&road_a};
	StraightRoadSprite road_sprite_b {&road_b};
	CrossingSprite crossing_sprite_a {&c_a};
	CrossingSprite crossing_sprite_b {&c_b};

	std::vector<RoadPieceSprite*> road_piece_sprites = {&road_sprite_a, &road_sprite_b,
			&crossing_sprite_a, &crossing_sprite_b};

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

		// render screen
        window.clear();
		for (RoadPieceSprite* rps : road_piece_sprites) {
			rps->draw(window);
		}
		car_sprite.draw(window);
        window.display();
    }

    return 0;
}
