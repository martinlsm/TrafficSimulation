#include <iostream>

#include <SFML/Graphics.hpp>
#include <vector>

#include "car.h"
#include "car_actions.h"
#include "constants.h"
#include "road_system.h"
#include "rendering.h"
#include "traffic_env.h"

using namespace traffic;

int main() {
	TrafficEnvironment* environment = load_environment(1);
	WorldRenderer world_renderer {environment->road_system->road_pieces};

	unsigned long car_id = environment->spawn_car(0, 4);
	CarMechanics* car = environment->getCarMechanics(car_id);
	CarAction::CarActionController car_action{car};
	CarSprite car_sprite{car};

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
		environment->update(dt_sec);

		std::cout << (int)environment->getCarState(car_id) << std::endl;

		// render screen
        window.clear({0x43,0x8a, 0x5e});
		world_renderer.draw(window);
		car_sprite.draw(window);
        window.display();
    }

	delete environment->road_system;
	delete environment;

    return 0;
}
