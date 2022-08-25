#ifndef CIRCUITPET_FIRMWARE_POLYGONCC_H
#define CIRCUITPET_FIRMWARE_POLYGONCC_H


#include <glm/vec2.hpp>
#include <vector>
#include "CollisionComponent.h"

class PolygonCC : public CollisionComponent {
public:
	PolygonCC(std::initializer_list<glm::vec2> points);
	/**
	 * @param points Points that define a polygon
	 * @param pivot Pivot point when rotation is applied, relative to the polygon
	 */
	PolygonCC(std::initializer_list<glm::vec2> points, glm::vec2 pivot);
	~PolygonCC() override = default;

	const std::vector<glm::vec2>& getPoints() const;
	bool isConvex() const;
	const glm::vec2& getCenter() const;

private:
	const std::vector<glm::vec2> points;
	static bool checkConvexity(const std::vector<glm::vec2>& polygon);
	static glm::vec2 checkPolyCenter(const std::vector<glm::vec2>& points);


	const bool convex;
	glm::vec2 center;
};


#endif //CIRCUITPET_FIRMWARE_POLYGONCC_H
