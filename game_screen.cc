#include "game_screen.h"

#include <cmath>
#include <iostream>
#include <sstream>

#include "geometry.h"

void GameScreen::init() {
  text_.reset(new Text("text.png"));

  score_ = 0;
  lives_ = 0;
  level_number_ = 1;

  load_level();
}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  ship_->set_engines(input.key_held(SDL_SCANCODE_W), input.key_held(SDL_SCANCODE_A), input.key_held(SDL_SCANCODE_D));
  ship_->update(audio, elapsed * (difficulty_ + 1) / 2);

  const Point p = ship_->position();
  if (p.x < 0 || p.x > 255 || p.y < 0) {
    audio.play_sample("crash.wav");
    --lives_;
    load_level();
  }

  if (level_->intersect(ship_->hull())) {
    if (calculate_score()) {
      audio.play_sample("land.wav");
      ++level_number_;
    } else {
      audio.play_sample("crash.wav");
      --lives_;
    }
    load_level();
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const Rect v = viewport();

  level_->draw(graphics, v);
  ship_->draw(graphics, v);

  PolyLine p;
  p.add(0, 0);
  p.add(0, 255);
  p.add(255, 255);
  p.add(255, 0);
  p.close();

  p.draw(graphics, 0xffffff80, v);

  const SDL_Rect fuel_outer = { 5, 5, 10 * level_->fuel(), 10 };
  const SDL_Rect fuel_inner = { 5, 5, 10 * ship_->fuel(), 10 };
  graphics.draw_rect(&fuel_outer, 0xffff00ff, false);
  graphics.draw_rect(&fuel_inner, 0xffff00ff, true);

  const SDL_Rect border = { 0, 0, graphics.width(), graphics.height() };
  graphics.draw_rect(&border, 0x00ff00ff, false);
}

std::string GameScreen::get_music_track() const {
  return "level.ogg";
}

void GameScreen::set_difficulty(int difficulty) {
  difficulty_ = difficulty;
}

Screen* GameScreen::next_screen() {
  return nullptr;
}

void GameScreen::load_level() {
  level_.reset(new Level(Level::kLevelData[level_number_]));
  ship_.reset(new Ship(level_->start(), level_->fuel()));
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Rect GameScreen::viewport() const {
  double top = MAX(MIN(ship_->position().y, level_->pad().y) - kViewportPadding, 0);
  double left = MAX(MIN(ship_->position().x, level_->pad().x) - kViewportPadding, 0);
  const double right = MIN(MAX(ship_->position().x, level_->pad().x) + kViewportPadding, 255);
  const double bottom = MIN(MAX(ship_->position().y, level_->pad().y) + kViewportPadding, 255);

  double height = bottom - top;
  double width = right - left;

  if (width < height) {
    width = height;
  } else {
    height = width;
  }

  if (top < 0) top = 0;
  if (left < 0) left = 0;
  if (top + height > 255) top = 255 - height;
  if (left + width > 255) left = 255 - width;

  return { left, top, width, height};
}

bool GameScreen::calculate_score() {
  // TODO save scores in score object
  const int p = level_->pad_score(ship_->position());
  if (p < 0) {
    std::cout << "Missed the pad\n";
    return false;
  }

  const int v = ship_->velocity_score();
  if (v < 0) {
    std::cout << "Landing too hard\n";
    return false;
  }

  const int a = ship_->angle_score();
  if (a < 0) {
    std::cout << "Tipped over\n";
    return false;
  }

  const int f = ship_->flips() * 2500;

  // TODO time bonus?
  // TODO fuel bonus?

  return true;
}
