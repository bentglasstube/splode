#include "particle.h"

#include <cmath>

Particle::Particle(Point pos, int color, double angle) :
  pos_(pos), vel_(0, 0), length_(0), angle_(angle), angle_vel_(0),
  color_(color), lifespan_(500), ttl_(lifespan_) {

  std::uniform_real_distribution<double> r(0, 1);

  angle_ += (r(rand_) - 0.5) / 2;
  angle_vel_ = (r(rand_) - 0.5) / 50;

  const double vel = (r(rand_) + 1) / 50;
  vel_.x = vel * std::cos(angle_);
  vel_.y = vel * std::sin(angle_);

  length_ = r(rand_) + 1;
}

void Particle::update(unsigned int elapsed) {
  ttl_ -= elapsed;

  pos_.x += elapsed * vel_.x;
  pos_.y += elapsed * vel_.y;
  angle_ += elapsed * angle_vel_;
}

void Particle::draw(Graphics& graphics, const Rect& viewport) const {
  const double dx = length_ / 2 * std::cos(angle_);
  const double dy = length_ / 2 * std::sin(angle_);

  const double x1 = (pos_.x - dx - viewport.x) / (double) viewport.w * graphics.width();
  const double y1 = (pos_.y - dy - viewport.y) / (double) viewport.h * graphics.height();
  const double x2 = (pos_.x + dx - viewport.x) / (double) viewport.w * graphics.width();
  const double y2 = (pos_.y + dy - viewport.y) / (double) viewport.h * graphics.height();

  const int alpha = 255 * ttl_ / lifespan_;

  graphics.draw_line({(int)x1, (int)y1}, {(int)x2, (int)y2}, (color_ & 0xffffff00) | alpha);
}

bool Particle::dead() const {
  return ttl_ <= 0;
}

ParticleEmitter::ParticleEmitter(int color) : color_(color) {}

void ParticleEmitter::update(unsigned int elapsed) {
  auto i = particles_.begin();
  while (i != particles_.end()) {
    (*i).update(elapsed);
    if ((*i).dead()) {
      i = particles_.erase(i);
    } else {
      ++i;
    }
  }
}

void ParticleEmitter::draw(Graphics& graphics, const Rect& viewport) const {
  for (const auto& p : particles_) {
    if (!p.dead()) p.draw(graphics, viewport);
  }
}

void ParticleEmitter::emit(const Point& pos, double angle) {
  particles_.emplace_back(pos, color_, angle);
}

std::default_random_engine Particle::rand_;
