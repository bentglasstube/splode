#include "title_screen.h"

#include "game_screen.h"
#include "high_score_screen.h"

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
  const bool quit = input.key_pressed(SDL_SCANCODE_ESCAPE);
  const bool choose = input.key_pressed(SDL_SCANCODE_SPACE) || input.key_pressed(SDL_SCANCODE_RETURN);
  const bool up = input.key_pressed(SDL_SCANCODE_W) || input.key_pressed(SDL_SCANCODE_UP);
  const bool down = input.key_pressed(SDL_SCANCODE_S) || input.key_pressed(SDL_SCANCODE_DOWN);

  if (up && choice_ > 0) --choice_;
  if (down && choice_ < choices_.size() - 1) ++choice_;

  if (choose) return false;

  if (quit) {
    choice_ = 4;
    return false;
  }

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  logo_->draw(graphics, (graphics.width() - 480) / 2, 64);

  for (size_t i = 0; i < choices_.size(); ++i) {
    const int y = graphics.height() / 2 + 16 * (i > 2 ? i + 1 : i);
    text_->draw(graphics, choices_[i], graphics.width() / 2, y, Text::Alignment::CENTER);

    if (i == choice_) {
      text_->draw(graphics, ">              <", graphics.width() / 2, y, Text::Alignment::CENTER);
    }
  }
}

Screen* TitleScreen::next_screen() {
  if (choice_ < 3) {
    GameScreen* s = new GameScreen();
    s->set_difficulty(choice_);
    return s;
  } else if (choice_ == 3) {
    return new HighScoreScreen();
  } else {
    return nullptr;
  }
}

std::string TitleScreen::get_music_track() const {
  return "menu.ogg";
}
