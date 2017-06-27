#include "game_screen.h"

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

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void GameScreen::draw(Graphics& graphics) const {
  const SDL_Rect r = { 0, 0, 256, 256 };
  graphics.draw_rect(&r, 0x00ff00ff, false);

  // TODO camera
  const double viewport_padding = 5;

  const double top = MIN(ship_->position().y, level_->get_pad().y) - viewport_padding;
  const double bottom = MAX(ship_->position().y, level_->get_pad().y) + viewport_padding;
  const double left = MIN(ship_->position().x, level_->get_pad().x) - viewport_padding;
  const double right = MAX(ship_->position().x, level_->get_pad().x) + viewport_padding;

  const SDL_Rect viewport = {
    MAX(left, 0), MAX(top, 0),
    MIN(right, 256), MIN(bottom, 256),
  };

  // TODO use viewport for scaling instead of drawing it
  graphics.draw_rect(&viewport, 0xffffffff, false);

  level_->draw(graphics);
  ship_->draw(graphics);

  if (ship_->get_fuel() > 0) {
    const SDL_Rect fuel = { 0, 0, 5 * ship_->get_fuel(), 5 };
    graphics.draw_rect(&fuel, 0xffff00ff, false);
  }

  if (level_->intersect(ship_->hull())) {
    graphics.draw_rect(&r, 0xff000044, true);
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
  ship_.reset(new Ship(level_->get_start(), level_->get_fuel()));
}
