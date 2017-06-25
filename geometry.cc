#include "geometry.h"

Point::Point(double x, double y) : x(x), y(y) {}

void PolyLine::add(double x, double y) {
  points_.emplace_back(x, y);
}

void PolyLine::add(Point p) {
  points_.emplace_back(p);
}

void PolyLine::close() {
  points_.emplace_back(points_[0]);
}

void PolyLine::draw(Graphics& graphics, int color) const {
  for (size_t i = 1; i < points_.size(); ++i) {
    graphics.draw_line(points_[i - 1].x, points_[i - 1].y, points_[i].x, points_[i].y, color);
  }
}

bool PolyLine::intersect(const PolyLine& other) const {
  return false;
}
