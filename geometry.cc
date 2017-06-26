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

size_t PolyLine::size() const {
  return points_.size();
}

Point PolyLine::get_point(size_t i) const {
  return points_.at(i);
}

void PolyLine::draw(Graphics& graphics, int color) const {
  for (size_t i = 1; i < points_.size(); ++i) {
    graphics.draw_line(points_[i - 1].x, points_[i - 1].y, points_[i].x, points_[i].y, color);
  }
}

int orientation(Point p, Point q, Point r) {
  const int v = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (v == 0) return 0;
  return v > 0 ? 1 : 2;
}

bool lines_intersect(Point p, Point q, Point r, Point s) {
  int o1 = orientation(p, q, r);
  int o2 = orientation(p, q, s);
  int o3 = orientation(r, s, p);
  int o4 = orientation(r, s, q);

  // TODO handle colinearity
  return o1 != o2 && o3 != o4;
}

bool PolyLine::intersect(const PolyLine& other) const {
  for (size_t i = 1; i < size(); ++i) {
    for (size_t j = 1; j < other.size(); ++j) {
      if (lines_intersect(get_point(i - 1), get_point(i), other.get_point(j - 1), other.get_point(j))) return true;
    }
  }

  return false;
}
