#include "high_score_screen.h"

#include <iostream>
#include <fstream>

#include "title_screen.h"

void HighScoreScreen::init() {
  text_.reset(new Text("text.png"));
  logo_.reset(new Sprite("title.png", 0, 0, 480, 183));
  place_ = -1;

  std::ifstream reader("content/scores.txt");
  std::string n;
  int s = 0;

  while (reader) {
    reader >> n;
    reader >> s;
    if (reader.eof()) break;

    if (score_ > s && place_ == -1) {
      std::cout << "High score: " << place_ << " place\n";
      place_ = top_scores_.size();
      top_scores_.emplace_back("", score_);
    }

    top_scores_.emplace_back(n, s);
  }
}

bool HighScoreScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (entering_name()) {
    std::string& name = top_scores_[place_].name;

    if (name.length() > 0) {
      if (input.key_pressed(SDL_SCANCODE_RETURN)) {
        save_scores();
        place_ = -1;
      }
      if (input.key_pressed(SDL_SCANCODE_BACKSPACE)) name.pop_back();
    }

    bool shift = input.key_held(SDL_SCANCODE_LSHIFT) || input.key_held(SDL_SCANCODE_RSHIFT);
    for (const SDL_Scancode scancode : input.all_pressed()) {
      SDL_Keycode k = SDL_GetKeyFromScancode(scancode);
      if (k >= SDLK_a && k <= SDLK_z) {
        name += (shift ? 'A' : 'a') + (k - SDLK_a);
      } else if (k >= SDLK_0 && k <= SDLK_9 && !shift) {
        name += '0' + k - SDLK_0;
      }
      // TODO handle symbols and shit
    }

  } else {
    if (input.key_pressed(SDL_SCANCODE_SPACE)) return false;
  }

  return true;
}

void HighScoreScreen::draw(Graphics& graphics) const {
  logo_->draw(graphics, (graphics.width() - 480) / 2, 64);

  if (entering_name()) {
    text_->draw(graphics, "Congratulations!  You got enough points to be", graphics.width() / 2, graphics.height() / 2 - 64, Text::Alignment::CENTER);
    text_->draw(graphics, "on the leaderboard.  Enter your name below.", graphics.width() / 2, graphics.height() / 2 - 48, Text::Alignment::CENTER);
  }

  const int x1 = graphics.width() / 2 - 168;
  const int x2 = graphics.width() / 2 + 200;
  for (int i = 0; i < 10; ++i) {
    const int y = graphics.height() / 2 + 16 * i;
    const std::string num = place_ == i ? "> " : std::to_string(i + 1) + ". ";

    text_->draw(graphics, top_scores_[i].name, x1, y, Text::Alignment::LEFT);
    text_->draw(graphics, std::to_string(top_scores_[i].score), x2, y, Text::Alignment::RIGHT);
    text_->draw(graphics, num, x1, y, Text::Alignment::RIGHT);
  }
}

Screen* HighScoreScreen::next_screen() {
  return new TitleScreen();
}

std::string HighScoreScreen::get_music_track() const {
  return "scores.ogg";
}

void HighScoreScreen::set_score(int score) {
  score_ = score;
}

bool HighScoreScreen::entering_name() const {
  return place_ != -1;
}

void HighScoreScreen::save_scores() const {
  std::ofstream writer("content/scores.txt");
  if (!writer) return;

  for (const auto& score : top_scores_) {
    writer << score.name << " " << score.score << "\n";
  }
}
