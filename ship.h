#pragma once

#include "audio.h"
#include "graphics.h"

#include "geometry.h"

class Ship {
  public:
    Ship(const Point& pos, double fuel);

    bool update(Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    void set_engines(bool main, bool left, bool right);

    double get_fuel() const;
    PolyLine hull() const;

  private:

    const double kPi = 3.14159265358979;
    const double kGravity = 0.0001;
    const double kEngineFactor = 2.0;
    const double kRotationSpeed = kGravity * 500;
    const double kShipSize = 2.5;

    double x_, y_, vx_, vy_, angle_, fuel_;
    bool engine_;
    int thrust_;

    Point coords(double angle, double radius) const;
    PolyLine engine() const;
};
