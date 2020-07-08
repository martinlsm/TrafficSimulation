#include <iostream>

#include <SFML/Graphics.hpp>
#include <vector>

#include "car.h"
#include "car_actions.h"
#include "constants.h"
#include "roadway.h"
#include "rendering.h"

using namespace traffic;

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
