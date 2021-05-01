#include "high_score_screen.h"

#include <iostream>
#include <fstream>

#include "title_screen.h"

HighScoreScreen::HighScoreScreen(int score) :
  text_("text.png"),
  logo_("title.png", 0, 0, 480, 183),
  place_(99), score_(score)
{

  std::ifstream reader("content/scores.txt");
  std::string n;
  int s = 0;

  while (reader) {
    reader >> n;
    reader >> s;
    if (reader.eof()) break;

    if (score_ > s && place_ == 99) {
      place_ = top_scores_.size();
      top_scores_.emplace_back("", score_);
    }

    top_scores_.emplace_back(n, s);
  }

  if (place_ == 99 && score_ > 0) {
    place_ = top_scores_.size();
    top_scores_.emplace_back("", score_);
  }
}

bool HighScoreScreen::update(const Input& input, Audio&, unsigned int) {
  if (entering_name()) {
    std::string& name = top_scores_[place_].name;

    // TODO text entry
    // if (name.length() > 0) {
      if (input.key_pressed(Input::Button::Start)) {
        save_scores();
        place_ = 99;
      }
      if (input.key_pressed(Input::Button::B)) name.pop_back();
    // }

  } else {
    if (input.key_pressed(Input::Button::A)) return false;
    if (input.key_pressed(Input::Button::Start)) return false;
  }

  return true;
}

void HighScoreScreen::draw(Graphics& graphics) const {
  logo_.draw(graphics, (graphics.width() - 480) / 2, 64);

  if (entering_name()) {
    text_.draw(graphics, "Congratulations!  You got enough points to be", graphics.width() / 2, graphics.height() / 2 - 64, Text::Alignment::Center);
    text_.draw(graphics, "on the leaderboard.  Enter your name below.", graphics.width() / 2, graphics.height() / 2 - 48, Text::Alignment::Center);

    text_.draw(graphics, "Shit, I broke this part, sorry :(", graphics.width() / 2, graphics.height() / 2 + 128, Text::Alignment::Center);
  }

  const int x1 = graphics.width() / 2 - 168;
  const int x2 = graphics.width() / 2 + 200;
  for (size_t i = 0; i < top_scores_.size(); ++i) {
    if (i == 10) break;

    const int y = graphics.height() / 2 + 16 * i;
    const std::string num = place_ == i ? "> " : std::to_string(i + 1) + ". ";

    text_.draw(graphics, top_scores_[i].name, x1, y, Text::Alignment::Left);
    text_.draw(graphics, std::to_string(top_scores_[i].score), x2, y, Text::Alignment::Right);
    text_.draw(graphics, num, x1, y, Text::Alignment::Right);
  }
}

Screen* HighScoreScreen::next_screen() const {
  return new TitleScreen();
}

std::string HighScoreScreen::get_music_track() const {
  return "scores.ogg";
}

bool HighScoreScreen::entering_name() const {
  return place_ != 99;
}

void HighScoreScreen::save_scores() const {
  std::ofstream writer("content/scores.txt");
  if (!writer) return;

  for (const auto& score : top_scores_) {
    writer << score.name << " " << score.score << "\n";
  }
}
