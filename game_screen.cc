#include "game_screen.h"

void GameScreen::init() {
  ship_.reset(new Ship(240, 240));
  text_.reset(new Text("text.png"));

  score_ = 0;
  lives_ = 0;

  // TODO load appropriate level
}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  ship_->set_engines(input.key_held(SDL_SCANCODE_W), input.key_held(SDL_SCANCODE_A), input.key_held(SDL_SCANCODE_D));
  ship_->update(audio, elapsed);

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  ship_->draw(graphics);
}

std::string GameScreen::get_music_track() const {
  return "level.ogg";
}

Screen* GameScreen::next_screen() {
  return nullptr;
}
