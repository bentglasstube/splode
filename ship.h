#pragma once

#include "audio.h"
#include "graphics.h"

class Ship {
  public:
    Ship(float x, float y);

    bool update(Audio& audio, unsigned int elapsed);
    void draw(Graphics& graphics) const;

    void set_engines(bool main, bool left, bool right);

  private:

    const float GRAVITY = 0.0001f;
    const float ROTATION_SPEED = 0.1f;
    const int SHIP_SIZE = 50;

    float x_, y_, vx_, vy_, angle_;
    bool engine_;
    int thrust_;

};
