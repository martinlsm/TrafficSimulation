#include <stdexcept>

#include "game_world.h"
#include "road_system.h"

namespace traffic {

Destination::Destination(Vec2d<float> position, float direction)
        : position(position), direction(direction) {}


CarBody Destination::placeCar() {
	return CarBody{position.x, position.y, direction};
}

bool Destination::inside(const Vec2d<float> p, const float margin) const {
	Vec2d<float> diff = p;
    diff -= this->position;
    return diff.abs() <= margin;
}

/*
 *    A  B
 *    |  |
 * C--D--E--F
 *    |  |
 * G--H--I--J
 *    |  |
 *    K  L
 */
void load_world_1(vector<RoadPiece*> &v, vector<Destination> &dests) {
	Vec2d<float> A {WORLD_WIDTH / 4.0f, 0.0f};
	Vec2d<float> B {3.0f * WORLD_WIDTH / 4.0f, 0.0f};
	Vec2d<float> C {0.0f, WORLD_HEIGHT / 4.0f};
	Vec2d<float> D {WORLD_WIDTH / 4.0f, WORLD_HEIGHT / 4.0f};
	Vec2d<float> E {3.0f * WORLD_WIDTH / 4.0f,  WORLD_HEIGHT / 4.0f};
	Vec2d<float> F {WORLD_WIDTH, WORLD_HEIGHT / 4.0f};
	Vec2d<float> G {0.0f, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> H {WORLD_WIDTH / 4.0f, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> I {3.0f * WORLD_WIDTH / 4.0f, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> J {WORLD_WIDTH, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> K {WORLD_WIDTH / 4.0f, WORLD_HEIGHT};
	Vec2d<float> L {3.0f * WORLD_WIDTH / 4.0f, WORLD_HEIGHT};
	float road_width = 75.0f;

	v.push_back(new StraightRoad(A, D, road_width));
	v.push_back(new StraightRoad(B, E, road_width));
	v.push_back(new StraightRoad(C, D, road_width));
	v.push_back(new StraightRoad(D, E, road_width));
	v.push_back(new StraightRoad(E, F, road_width));
	v.push_back(new StraightRoad(D, H, road_width));
	v.push_back(new StraightRoad(E, I, road_width));
	v.push_back(new StraightRoad(G, H, road_width));
	v.push_back(new StraightRoad(H, I, road_width));
	v.push_back(new StraightRoad(I, J, road_width));
	v.push_back(new StraightRoad(H, K, road_width));
	v.push_back(new StraightRoad(I, L, road_width));

	dests.push_back({A, M_PI / 2.0f});
	dests.push_back({B, M_PI / 2.0f});
	dests.push_back({C, 0.0f});
	dests.push_back({F, M_PI});
	dests.push_back({G, 0.0f});
	dests.push_back({J, M_PI});
	dests.push_back({K, 3.0f * M_PI / 2.0f});
	dests.push_back({L, 3.0f * M_PI / 2.0f});
}

TrafficEnvironment load_environment(unsigned int id) {
	vector<RoadPiece*> v;
	vector<Destination> dests;
	switch (id) {
		case 1:
			load_world_1(v, dests);
			break;
	}

	if (v.empty()) {
		throw std::invalid_argument("invalid world id");
	}
	return {RoadSystem{v}, dests, {}};
}

} // namespace traffic
