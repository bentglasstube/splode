#include "title_screen.h"

#include "game_screen.h"

void TitleScreen::init() {
  text_.reset(new Text("text.png"));
  logo_.reset(new Sprite("title.png", 0, 0, 480, 183));

  choices_.push_back("Easy");
  choices_.push_back("Medium");
  choices_.push_back("Hard");

  choices_.push_back("High Scores");
  choices_.push_back("Quit");

  choice_ = 0;
}

bool TitleScreen::update(const Input& input, Audio&, unsigned int) {
  if (input.key_pressed(SDL_SCANCODE_W)) --choice_;
  if (input.key_pressed(SDL_SCANCODE_S)) ++choice_;

  if (choice_ < 0) choice_ = 0;
  if (choice_ >= choices_.size()) choice_ = choices_.size() - 1;

  if (input.key_pressed(SDL_SCANCODE_RETURN)) return false;

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  logo_->draw(graphics, 16, 0);

  for (int i = 0; i < choices_.size(); ++i) {
    const int y = 240 + 16 * (i > 2 ? i + 1 : i);
    text_->draw(graphics, choices_[i], 256, y, Text::Alignment::CENTER);

    if (i == choice_) {
      text_->draw(graphics, ">", 192, y, Text::Alignment::LEFT);
      text_->draw(graphics, "<", 312, y, Text::Alignment::LEFT);
    }
  }
}

Screen* TitleScreen::next_screen() {
  if (choice_ < 3) {
    return new GameScreen();
  } else {
    return nullptr;
  }
}

std::string TitleScreen::get_music_track() const {
  return "menu.ogg";
}
