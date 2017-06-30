#include "high_score_screen.h"

#include <iostream>
#include <fstream>

#include "title_screen.h"

void HighScoreScreen::init() {
  text_.reset(new Text("text.png"));
  logo_.reset(new Sprite("title.png", 0, 0, 480, 183));

  std::ifstream reader("content/scores.txt");
  std::string n;
  int s, place = 99;

  while (reader) {
    reader >> n;
    reader >> s;
    if (reader.eof()) break;

    if (score_ > s && place > 10) {
      std::cout << "High score: " << place << " place\n";
      place = top_scores_.size();
      top_scores_.emplace_back("NEW PLAYER", score_);
    }

    top_scores_.emplace_back(n, s);
  }
}

bool HighScoreScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.any_pressed()) {
    return false;
  }

  return true;
}

void HighScoreScreen::draw(Graphics& graphics) const {
  logo_->draw(graphics, (graphics.width() - 480) / 2, 64);

  const int x1 = graphics.width() / 2 - 168;
  const int x2 = graphics.width() / 2 + 200;
  for (int i = 0; i < 10; ++i) {
    const int y = graphics.height() / 2 + 16 * i;
    text_->draw(graphics, std::to_string(i + 1) + ". ", x1, y, Text::Alignment::RIGHT);
    text_->draw(graphics, top_scores_[i].name, x1, y, Text::Alignment::LEFT);
    text_->draw(graphics, std::to_string(top_scores_[i].score), x2, y, Text::Alignment::RIGHT);
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
