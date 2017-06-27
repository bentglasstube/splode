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
  const double top = MAX(MIN(ship_->position().y, level_->pad().y) - kViewportPadding, 0);
  const double left = MAX(MIN(ship_->position().x, level_->pad().x) - kViewportPadding, 0);
  const double right = MIN(MAX(ship_->position().x, level_->pad().x) + kViewportPadding, 256);
  const double bottom = MIN(MAX(ship_->position().y, level_->pad().y) + kViewportPadding, 256);

  const double height = bottom - top;
  const double width = right - left;

  const double screen_ratio = 16 / (double) 9;
  const double view_ratio = width / height;

  if (view_ratio < screen_ratio) {
    // view too narrow, expand width
    const double target_width = screen_ratio * height;
    return { left - (target_width - width) / 2, top, target_width, height };
  } else {
    // view too wide, expand height
    const double target_height = width / screen_ratio;
    return { left, top - (target_height - height) / 2, width, target_height };
  }
}
