#pragma once

#include <vector>

#include "graphics.h"

class Point {
  public:
    Point(double x, double y);
    double x, y;
};

class Rect {
  public:
    Rect(double x, double y, double w, double h);
    double x, y, w, h;

    void draw(Graphics& graphics, int color, const Rect& viewport) const;
};

class PolyLine {
  public:

    void add(double x, double y);
    void add(Point p);
    void close();

    size_t size() const;
    Point point(size_t i) const;

    void draw(Graphics& graphics, int color, const Rect& viewport) const;
    bool intersect(const PolyLine& other) const;

  private:
    std::vector<Point> points_;
};
