
#ifndef CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
#define CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H

#include <functional>
#include <list>
#include "../GameSystem.h"

class CollisionSystem : public GameSystem {
friend Game;

typedef std::vector<glm::vec2> Polygon;

public:
	CollisionSystem(const Game* game);

	void addPair(const GameObject& first, const GameObject& second, std::function<void()> handler);
	void removePair(const GameObject& first, const GameObject& second);

	void wallTop(const GameObject& obj, std::function<void()> handler);
	void wallBot(const GameObject& obj, std::function<void()> handler);
	void wallLeft(const GameObject& obj, std::function<void()> handler);
	void wallRight(const GameObject& obj, std::function<void()> handler);
	void wallsVertical(const GameObject& obj, std::function<void()> handler);
	void wallsHorizontal(const GameObject& obj, std::function<void()> handler);
	void wallsAll(const GameObject& obj, std::function<void()> handler);

	void drawDebug(Sprite* canvas);

	void update(uint32_t deltaMicros) override;

private:
	struct Pair {
		const GameObject* first;
		const GameObject* second;
		std::function<void()> handler;
		bool colliding = false;

		bool operator==(const Pair& other) const {
			return (first == other.first && second == other.second) || (first == other.second && second == other.first);
		}
	};

	struct {
		GameObject top;
		GameObject bot;
		GameObject left;
		GameObject right;
	} Walls;

	std::list<Pair> pairs;
	std::list<Pair> removedPairs;
	std::list<Pair> addedPairs;
	void removeObject(const GameObject& GO);

	static bool circleCircle(const GameObject& circle1, const GameObject& circle2);
	static bool rectRect(const GameObject& square1, const GameObject& square2);
	static bool rectCircle(const GameObject& rect, const GameObject& circle);
	static bool polyPoly(const GameObject& obj1, const GameObject& obj2);
	static bool polyRect(const GameObject& poly, const GameObject& rect);
	static bool polyCircle(const GameObject& poly, const GameObject& circle);

	static bool polyContainsPoint(const Polygon& polygon, glm::vec2 point);
	static CollisionSystem::Polygon getRotatedTranslatedPoly(const GameObject& poly);
	static void drawPolygon(const GameObject& poly, Sprite* canvas, Color color);
};


#endif //CIRCUITPET_FIRMWARE_COLLISIONSYSTEM_H
