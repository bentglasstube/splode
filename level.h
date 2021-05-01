#pragma once

#include <string>
#include <vector>

#include "graphics.h"
#include "geometry.h"

class Level {
  public:
    Level(const std::string& data);
    void load(const std::string& data);
    void draw(Graphics& graphics, const Rect& viewport) const;

    std::string name() const;
    Point start() const;
    Point pad() const;
    int fuel() const;
    int pad_score(const Point& pos) const;
    bool intersect(const PolyLine& poly) const;

    static const std::string kLevelData[21];

  private:
    static constexpr double kPadSize = 2.5;
    static constexpr double kMaxDistance = 5;

    std::string name_;
    int fuel_;
    Point start_, pad_;
    std::vector<PolyLine> terrain_;

    PolyLine pad_rect() const;
};
