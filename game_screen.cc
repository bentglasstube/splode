#include "game_screen.h"

#include <cmath>

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
  ship_->update(audio, elapsed);

  if (input.key_pressed(SDL_SCANCODE_LEFTBRACKET)) {
    level_number_ = (level_number_ + 20) % 21;
    load_level();
  }

  if (input.key_pressed(SDL_SCANCODE_RIGHTBRACKET)) {
    level_number_ = (level_number_  + 1) % 21;
    load_level();
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const Rect v = viewport();

  level_->draw(graphics, v);
  ship_->draw(graphics, v);

  if (ship_->fuel() > 0) {
    const SDL_Rect fuel = { 0, 0, 5 * ship_->fuel(), 5 };
    graphics.draw_rect(&fuel, 0xffff00ff, false);
  }
}

std::string GameScreen::get_music_track() const {
  return "level.ogg";
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
  const double right = MIN(MAX(ship_->position().x, level_->pad().x) + kViewportPadding, 256);
  const double bottom = MIN(MAX(ship_->position().y, level_->pad().y) + kViewportPadding, 256);

  double height = bottom - top;
  double width = right - left;

  // consider passing in graphics here to get real screen size
  const double screen_ratio = 16 / (double) 9;
  const double view_ratio = width / height;

  if (view_ratio < screen_ratio) {
    // view too narrow, expand width
    const double target_width = screen_ratio * height;
    left = left - (target_width - width) / 2;
    width = target_width;
  } else {
    // view too wide, expand height
    const double target_height = width / screen_ratio;
    top -= (target_height - height) / 2;
    height = target_height;
  }

  if (top < 0) top = 0;
  if (left < 0) left = 0;

  if (top + height > 256) top = height - 256;
  if (left + width > 256) left = width - 256;

  return { left, top, width, height};
}
