#include "ship.h"

#include <cmath>

Ship::Ship(const Point& pos, double fuel)  : x_(pos.x), y_(pos.y), vx_(0), vy_(0), angle_(-kPi / 2), fuel_(fuel), engine_(false), thrust_(0) {}

bool Ship::update(Audio& audio, unsigned int elapsed) {
  double ax = 0;
  double ay = kGravity;

  if (engine_ && fuel_ > 0) {
    ax += kEngineFactor * kGravity * cos(angle_);
    ay += kEngineFactor * kGravity * sin(angle_);

    audio.play_sample("thrust.wav");
    fuel_ -= elapsed / 1000.0f;
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
  if (engine_ && fuel_ > 0) {
    engine().draw(graphics, 0xff0000ff);
  }
  hull().draw(graphics, 0x00ffffff);
}

void Ship::set_engines(bool main, bool left, bool right) {
  engine_ = main;
  thrust_ = 0 + (left ? -1 : 0) + (right ? 1 : 0);
}

Point Ship::coords(double angle, double radius) const {
  return { x_ + cos(angle_ + angle) * radius, y_ + sin(angle_ + angle) * radius };
}

PolyLine Ship::engine() const {
  PolyLine p;

  p.add(coords(kPi / 2, kShipSize / 2));
  p.add(coords(kPi, kShipSize));
  p.add(coords(-kPi / 2, kShipSize / 2));

  return p;
}

PolyLine Ship::hull() const {
  PolyLine p;

  p.add(coords(0, kShipSize * 3));
  p.add(coords(kPi / 2, kShipSize));
  p.add(coords(-kPi / 2, kShipSize));
  p.close();

  return p;
}

double Ship::get_fuel() const {
  return fuel_;
}

Point Ship::position() const {
  return { x_, y_ };
}
