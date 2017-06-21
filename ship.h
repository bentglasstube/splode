#pragma once

#include "audio.h"
#include "graphics.h"

class Ship {
  public:
    Ship(double x, double y);

    bool update(Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    void set_engines(bool main, bool left, bool right);

  private:

    const double kPi = 3.14159265358979;
    const double kGravity = 0.0001f;
    const double kEngineFactor = 2.0f;
    const double kRotationSpeed = kGravity * 500;
    const double kShipSize = 10;

    double x_, y_, vx_, vy_, angle_;
    bool engine_;
    int thrust_;

    struct Point { double x, y; };

    Point coords(double angle, double radius) const;
};
