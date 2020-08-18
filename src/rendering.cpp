#include "rendering.h"
#include "texture_manager.h"

StraightRoadSprite::StraightRoadSprite(StraightRoad* road)
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

StraightRoadSprite::~StraightRoadSprite() {}

void StraightRoadSprite::draw(sf::RenderWindow &window) {
	window.draw(rect, transform);
}

FilledCircularPieceSprite::FilledCircularPieceSprite(
		FilledCircularPiece *road_piece) : RoadPieceSprite() {
	float radius = road_piece->getRadius();
	circ.setRadius(radius);
	Vec2d<float> pos = road_piece->getPos();
	circ.setPosition(pos.x - radius, pos.y - radius);
	circ.setFillColor(sf::Color(67, 67, 67));
}

FilledCircularPieceSprite::~FilledCircularPieceSprite() {}

void FilledCircularPieceSprite::draw(sf::RenderWindow &window) {
	window.draw(circ);
}

WorldRenderer::WorldRenderer(vector<RoadPiece*>& road_pieces) {
	for (RoadPiece* road_piece : road_pieces) {
		if (typeid(*road_piece) == typeid(FilledCircularPiece)) {
			FilledCircularPiece* fcp = dynamic_cast<FilledCircularPiece*>(road_piece);
			sprites.push_back(new FilledCircularPieceSprite(fcp));
		} else if (typeid(*road_piece) == typeid(StraightRoad)) {
			StraightRoad* straight_road = dynamic_cast<StraightRoad*>(road_piece);
			sprites.push_back(new StraightRoadSprite(straight_road));
		}
	}
}

WorldRenderer::~WorldRenderer() {
	for (RoadPieceSprite* rps : sprites) {
		delete rps;
	}
}

void WorldRenderer::draw(sf::RenderWindow &window) {
	for (RoadPieceSprite* sprite : sprites) {
		sprite->draw(window);
	}
}

sf::Transform CarSprite::getTransform() {
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

CarSprite::CarSprite(CarMechanics *car_body) {
	this->car_body = car_body;
	sprite.setTexture(*texture_manager::car_texture);
}

void CarSprite::draw(sf::RenderWindow &window) {
	sf::Transform transform = this->getTransform();
	window.draw(sprite, transform);
}

SensorsRenderer::SensorsRenderer(
		RoadSystem* road_system, CarMechanics* car, vector<float>& sensor_angles)
			: road_system(road_system),
			car(car),
			sensor_angles(sensor_angles),
			lines(sf::LinesStrip, sensor_angles.size()) {}

void SensorsRenderer::draw(sf::RenderWindow& window) {
	vector<float> readings = road_system->sensor_readings(*car, sensor_angles);
	Vec2d<float> car_pos = car->getPos();
	for (size_t i = 0; i < sensor_angles.size(); i++) {
		float actual_angle = car->getRotation() + sensor_angles[i];
		float endpoint_x = car_pos.x + (readings[i] * std::cos(actual_angle));
		float endpoint_y = car_pos.y + (readings[i] * std::sin(actual_angle)); 
		lines[i].position = sf::Vector2f(endpoint_x, endpoint_y);
	}
	window.draw(lines);
}
