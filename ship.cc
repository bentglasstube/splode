#include "ship.h"

#include <cmath>

Ship::Ship(float x, float y)  : x_(x), y_(y), vx_(0), vy_(0), angle_(0), engine_(false), thrust_(0) {}

bool Ship::update(Audio& audio, unsigned int elapsed) {
  float ax = 0;
  float ay = GRAVITY;

  if (engine_) {
    ax -= 2 * GRAVITY * cos(angle_);
    ay -= 2 * GRAVITY * sin(angle_);

    audio.play_sample("thrust.wav");
    // TODO reduce fuel levels
  }

  // TODO set cap on accelleration

  angle_ += ROTATION_SPEED * thrust_;

  // TODO check for flips

  vx_ += ax * elapsed;
  vy_ += ay * elapsed;

  x_ += vx_ * elapsed;
  y_ += vy_ * elapsed;

  return true;
}

#define POLAR(a, r) (x + cos(angle_ + a) * r), (y + sin(angle_ + a) * r)
#define PI    3.14159265359

void Ship::draw(Graphics& graphics) const {
  // TODO account for camera

  const float x = x_ + cos(angle_) * SHIP_SIZE;
  const float y = y_ - sin(angle_) * SHIP_SIZE;

  graphics.draw_line(POLAR(PI / -2, SHIP_SIZE), POLAR(0, 2 * SHIP_SIZE), 0x00ffffff);
  graphics.draw_line(POLAR(PI /  2, SHIP_SIZE), POLAR(0, 2 * SHIP_SIZE), 0x00ffffff);
  graphics.draw_line(POLAR(PI / -2, SHIP_SIZE), POLAR(PI / 2, SHIP_SIZE), 0x00ffffff);

  graphics.draw_circle(x_, y_, SHIP_SIZE, 0xffffff44, false);

  if (engine_) {
    graphics.draw_line(POLAR(PI /  2, SHIP_SIZE / 2), POLAR(PI, SHIP_SIZE), 0xff0000ff);
    graphics.draw_line(POLAR(PI / -2, SHIP_SIZE / 2), POLAR(PI, SHIP_SIZE), 0xff0000ff);
  }
}

void Ship::set_engines(bool main, bool left, bool right) {
  engine_ = main;
  thrust_ = 0 + (left ? -1 : 0) + (right ? 1 : 0);
}
