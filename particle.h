#pragma once

#include "graphics.h"

#include "geometry.h"

class Particle {
  public:
    Particle(Point pos, int color);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics, const Rect& viewport) const;

    bool dead() const;

  private:
    Point pos_, vel_;
    double length_, angle_, angle_vel_;
    int color_, lifespan_, ttl_;
};

class ParticleEmitter {
  public:
    ParticleEmitter(int color);

    void update(unsigned int elapsed);
    void draw(Graphics& graphics, const Rect& viewport) const;

    void emit(const Point& pos);

    // TODO remove
    int count() const;

  private:
    std::vector<Particle> particles_;
    int color_;
};
