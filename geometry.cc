#include "geometry.h"

Point::Point(double x, double y) : x(x), y(y) {}

Rect::Rect(double x, double y, double w, double h) : x(x), y(y), w(w), h(h) {}

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

Point PolyLine::point(size_t i) const {
  return points_.at(i);
}

void PolyLine::draw(Graphics& graphics, int color, const Rect& viewport) const {
  for (size_t i = 1; i < points_.size(); ++i) {
    double x1 = points_[i - 1].x;
    double y1 = points_[i - 1].y;
    double x2 = points_[i].x;
    double y2 = points_[i].y;

    x1 = (x1 - viewport.x) / (double) viewport.w * graphics.width();
    y1 = (y1 - viewport.y) / (double) viewport.h * graphics.height();
    x2 = (x2 - viewport.x) / (double) viewport.w * graphics.width();
    y2 = (y2 - viewport.y) / (double) viewport.h * graphics.height();

    graphics.draw_line(x1, y1, x2, y2, color);
  }
}

int orientation(Point p, Point q, Point r) {
  const double v = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
  if (v == 0) return 0;
  return v > 0 ? 1 : 2;
}

bool lines_intersect(Point p, Point q, Point r, Point s) {
  int o1 = orientation(p, q, r);
  int o2 = orientation(p, q, s);
  int o3 = orientation(r, s, p);
  int o4 = orientation(r, s, q);

  return o1 != o2 && o3 != o4;
}

bool PolyLine::intersect(const PolyLine& other) const {
  for (size_t i = 1; i < size(); ++i) {
    for (size_t j = 1; j < other.size(); ++j) {
      if (lines_intersect(point(i - 1), point(i), other.point(j - 1), other.point(j))) return true;
    }
  }

  return false;
}
