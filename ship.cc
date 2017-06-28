#include "ship.h"

#include <cmath>

Ship::Ship(const Point& pos, double fuel) :
  x_(pos.x), y_(pos.y), vx_(0), vy_(0), angle_(kUp), fuel_(fuel),
  engine_(false), thrust_(0), flips_(0) {}

bool Ship::update(Audio& audio, unsigned int elapsed) {
  double ax = 0;
  double ay = kGravity;

  if (engine_ && fuel_ > 0) {
    ax += kGravity * kEngineFactor * cos(angle_);
    ay += kGravity * kEngineFactor * sin(angle_);

    // audio.play_sample("thrust.wav");
    fuel_ -= elapsed / 1000.0f;
  }

  angle_ += kGravity * kThrustFactor * thrust_;

  if (angle_ > kPi / 2) {
    angle_ -= 2 * kPi;
    audio.play_sample("flip.wav");
    flips_++;
  } else if (angle_ < -3 * kPi / 2) {
    angle_ += 2 * kPi;
    audio.play_sample("flip.wav");
    flips_++;
  }

  vx_ += ax * elapsed;
  vy_ += ay * elapsed;

  // TODO set cap on velocity

  x_ += vx_ * elapsed;
  y_ += vy_ * elapsed;

  return true;
}

void Ship::draw(Graphics& graphics, const Rect& viewport) const {
  if (engine_ && fuel_ > 0) engine().draw(graphics, 0xff0000ff, viewport);
  hull().draw(graphics, 0x00ffffff, viewport);
}

void Ship::set_engines(bool main, bool left, bool right) {
  engine_ = main;
  thrust_ = 0 + (left ? -1 : 0) + (right ? 1 : 0);
}

PolyLine Ship::hull() const {
  PolyLine p;

  p.add(coords(0, kSize * 3));
  p.add(coords(kPi / 2, kSize));
  p.add(coords(-kPi / 2, kSize));
  p.close();

  return p;
}

double Ship::fuel() const {
  return fuel_;
}

Point Ship::position() const {
  return { x_, y_ };
}

int Ship::velocity_score() const {
  const double v = std::sqrt(vx_ * vx_ + vy_ * vy_);
  return 1000 * (1 - v / kMaxVelocity);
}

int Ship::angle_score() const {
  const double a = std::abs(angle_ - kUp);
  return 1000 * (1 - a / kMaxAngle);
}

int Ship::flips() const {
  return flips_;
}

Point Ship::coords(double angle, double radius) const {
  return { x_ + cos(angle_ + angle) * radius, y_ + sin(angle_ + angle) * radius };
}

PolyLine Ship::engine() const {
  PolyLine p;

  p.add(coords(kPi / 2, kSize / 2));
  p.add(coords(kPi, kSize));
  p.add(coords(-kPi / 2, kSize / 2));

  return p;
}
