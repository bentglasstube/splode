#include "ship.h"

#include <cmath>

Ship::Ship(double x, double y)  : x_(x), y_(y), vx_(0), vy_(0), angle_(-kPi / 2), engine_(false), thrust_(0) {}

bool Ship::update(Audio& audio, unsigned int elapsed) {
  double ax = 0;
  double ay = kGravity;

  if (engine_) {
    ax += kEngineFactor * kGravity * cos(angle_);
    ay += kEngineFactor * kGravity * sin(angle_);

    audio.play_sample("thrust.wav");
    // TODO reduce fuel levels
  }

  // TODO set cap on accelleration

  angle_ += kRotationSpeed * thrust_;

  // TODO check for flips

  vx_ += ax * elapsed;
  vy_ += ay * elapsed;

  x_ += vx_ * elapsed;
  y_ += vy_ * elapsed;

  return true;
}

void Ship::draw(Graphics& graphics) const {
  // TODO account for camera

  if (engine_) {
    const Point a = coords(kPi / 2, kShipSize / 2);
    const Point b = coords(kPi, kShipSize);
    const Point c = coords(-kPi / 2, kShipSize / 2);

    graphics.draw_line(a.x, a.y, b.x, b.y, 0xff0000ff);
    graphics.draw_line(c.x, c.y, b.x, b.y, 0xff0000ff);
  }

  const Point d = coords(0, kShipSize * 2);
  const Point e = coords(kPi / 2, kShipSize);
  const Point f = coords(-kPi / 2, kShipSize);

  graphics.draw_line(d.x, d.y, e.x, e.y, 0x00ffffff);
  graphics.draw_line(e.x, e.y, f.x, f.y, 0x00ffffff);
  graphics.draw_line(f.x, f.y, d.x, d.y, 0x00ffffff);
}

void Ship::set_engines(bool main, bool left, bool right) {
  engine_ = main;
  thrust_ = 0 + (left ? -1 : 0) + (right ? 1 : 0);
}

Ship::Point Ship::coords(double angle, double radius) const {
  return { x_ + cos(angle_ + angle) * radius, y_ + sin(angle_ + angle) * radius };
}
