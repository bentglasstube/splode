#pragma once

#include "audio.h"
#include "graphics.h"

#include "geometry.h"

class Ship {
  public:
    static constexpr double kSize = 2.5;

    Ship(const Point& pos, double fuel);

    bool update(Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics, const Rect& viewport) const;

    void set_engines(bool main, bool left, bool right);

    double fuel() const;
    PolyLine hull() const;
    Point position() const;

    int velocity_score() const;
    int angle_score() const;
    int flips() const;

  private:

    static constexpr double kEngineFactor = 2.5;
    static constexpr double kThrustFactor = 700;
    static constexpr double kGravity = 0.00005;
    static constexpr double kPi = 3.14159265358979;

    static constexpr double kUp = -kPi / 2;

    static constexpr double kMaxAngle = 3 * kPi / 32;
    static constexpr double kMaxVelocity = 0.02;

    double x_, y_, vx_, vy_, angle_, fuel_;
    bool engine_;
    int thrust_, flips_;

    Point coords(double angle, double radius) const;
    PolyLine engine() const;
};
