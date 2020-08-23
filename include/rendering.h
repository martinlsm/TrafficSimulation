#pragma once

#include <SFML/Graphics.hpp>

#include "road_system.h"
#include "car.h"
#include "constants.h"

using namespace traffic;

class RoadPieceSprite {
public:
	virtual ~RoadPieceSprite() {};
	virtual void draw(sf::RenderWindow &window) = 0;
};

class StraightRoadSprite : public RoadPieceSprite {
private:
	sf::RectangleShape rect;
	sf::Transform transform;
public:
	StraightRoadSprite(StraightRoad* road);
	~StraightRoadSprite() override;
	void draw(sf::RenderWindow &window) override;
};

class FilledSquareSprite : public RoadPieceSprite {
private:
	sf::RectangleShape rect;
	sf::Transform transform;
public:
	FilledSquareSprite(FilledSquare* sq);
	~FilledSquareSprite() override;
	void draw(sf::RenderWindow &window) override;
};

class FilledCircularPieceSprite : public RoadPieceSprite {
private:
	sf::CircleShape circ;
public:
	FilledCircularPieceSprite(FilledCircularPiece *road_piece);
	~FilledCircularPieceSprite() override;
	void draw(sf::RenderWindow &window) override;
};

class WorldRenderer {
private:
	vector<RoadPieceSprite*> sprites;
public:
	WorldRenderer(vector<RoadPiece*>& road_pieces);
	~WorldRenderer();
	void draw(sf::RenderWindow &window);
};

class CarSprite {
private:
	CarMechanics* car_body;
	sf::Sprite sprite;
	
	sf::Transform getTransform();
public:
	CarSprite(CarMechanics *car_body);
	void draw(sf::RenderWindow &window);
};

class SensorsRenderer {
private:
	RoadSystem* road_system;
	CarMechanics* car;
	vector<float>& sensor_angles;
	sf::VertexArray lines;
public:
	SensorsRenderer(RoadSystem* road_system,
			CarMechanics* car, vector<float>& sensor_angles);
	SensorsRenderer& operator=(SensorsRenderer&& rhs);
	void draw(sf::RenderWindow& window);
};
