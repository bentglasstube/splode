#pragma once

#include <vector>

#include "graphics.h"

class Point {
  public:
    Point(double x, double y);
    double x, y;
};

class PolyLine {
  public:

    void add(double x, double y);
    void add(Point p);
    void close();

    void draw(Graphics& graphics, int color) const;
    bool intersect(const PolyLine& other) const;

  private:
    std::vector<Point> points_;
};
