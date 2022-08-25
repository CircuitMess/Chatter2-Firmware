#include "PolygonCC.h"
#include <Arduino.h>

PolygonCC::PolygonCC(std::initializer_list<glm::vec2> points) : CollisionComponent(CollisionType::Polygon), points(points),
																convex(checkConvexity(this->points)), center(checkPolyCenter(points)){
}

PolygonCC::PolygonCC(std::initializer_list<glm::vec2> points, glm::vec2 pivot) : CollisionComponent(CollisionType::Polygon), points(points),
																				 convex(checkConvexity(this->points)), center(pivot){

}

bool PolygonCC::checkConvexity(const std::vector<glm::vec2>& polygon){
	typedef glm::vec2 Point;
	//Code from Sunshine2K, licensed under MIT
	//source: http://www.sunshine2k.de/coding/java/Polygon/Convex/polygon.htm
	//license: http://www.sunshine2k.de/license.html

	if(polygon.size() < 3) return false;

	Point p;
	Point v;
	Point u;
	int res = 0;
	for(int i = 0; i < polygon.size(); i++){
		p = polygon[i];
		Point tmp = polygon[(i + 1) % polygon.size()];
		v.x = tmp.x - p.x;
		v.y = tmp.y - p.y;
		u = polygon[(i + 2) % polygon.size()];

		if(i == 0) // in first loop direction is unknown, so save it in res
			res = (int)(u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x);
		else{
			int newres = (int)(u.x * v.y - u.y * v.x + v.x * p.y - v.y * p.x);
			if((newres > 0 && res < 0) || (newres < 0 && res > 0))
				return false;
		}
	}
	return true;
}

const std::vector<glm::vec2>& PolygonCC::getPoints() const{
	return points;
}

bool PolygonCC::isConvex() const{
	return convex;
}

glm::vec2 PolygonCC::checkPolyCenter(const std::vector<glm::vec2>& points) {
	float left = 0, right = 0, top = 0, bot = 0;

	//get leftmost, rightmost, topmost, bottommost points
	for(const auto& point : points){
		if(point.x < left) left = point.x;
		if(point.x > right) right = point.x;
		if(point.y < top) top = point.y;
		if(point.y > bot) bot = point.y;
	}

	return {(right - left) / 2, (bot - top) / 2};
}

const glm::vec2& PolygonCC::getCenter() const{
	return center;
}
