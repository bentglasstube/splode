#include "game_screen.h"

void GameScreen::init() {
  text_.reset(new Text("text.png"));

  score_ = 0;
  lives_ = 0;
  level_number_ = 1;

  // TODO load appropriate level
  const std::string data("\x08Training\x10\xc0\xd0`\x80\x01\x09\x00 0@P\x80p\x80\x80p\xa0\xa0\xd0\x90\xf0`\xffP\x0a", 35);
  level_.reset(new Level(data));
  ship_.reset(new Ship(level_->get_start(), level_->get_fuel()));
}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  ship_->set_engines(input.key_held(SDL_SCANCODE_W), input.key_held(SDL_SCANCODE_A), input.key_held(SDL_SCANCODE_D));
  ship_->update(audio, elapsed);

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const SDL_Rect r = { 0, 0, 256, 256 };
  graphics.draw_rect(&r, 0x00ff00ff, false);

  // TODO camera
  level_->draw(graphics);
  ship_->draw(graphics);

  if (ship_->get_fuel() > 0) {
    const SDL_Rect fuel = { 0, 0, 5 * ship_->get_fuel(), 5 };
    graphics.draw_rect(&fuel, 0xffff00ff, false);
  }
}

std::string GameScreen::get_music_track() const {
  return "level.ogg";
}

Screen* GameScreen::next_screen() {
  return nullptr;
}
