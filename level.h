#pragma once

#include <string>
#include <vector>

#include "graphics.h"
#include "geometry.h"

class Level {
  public:
    Level(const std::string& data);
    void draw(Graphics& graphics) const;

    Point get_start() const;
    Point get_pad() const;
    int get_fuel() const;
    bool intersect(const PolyLine& poly) const;

    static const std::string kLevelData[21];

  private:
    std::string name_;
    int fuel_;
    Point start_, pad_;
    std::vector<PolyLine> terrain_;
};
