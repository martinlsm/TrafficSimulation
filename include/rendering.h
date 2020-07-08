#pragma once

#include <SFML/Graphics.hpp>

#include "roadway.h"
#include "car.h"
#include "constants.h"

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
	StraightRoadSprite(StraightRoad* road);
	void draw(sf::RenderWindow &window);
};

class CrossingSprite : public RoadPieceSprite {
private:
	sf::CircleShape circ;
public:
	CrossingSprite(Crossing *crossing);
	void draw(sf::RenderWindow &window);
};

class CarSprite {
private:
	CarBody *car_body;
	sf::Texture texture;
	sf::Sprite sprite;
	
	sf::Transform getTransform();
public:
	CarSprite(CarBody *car_body);
	void draw(sf::RenderWindow &window);
};
