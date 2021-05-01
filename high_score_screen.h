#pragma once

#include <memory>
#include <vector>

#include "audio.h"
#include "graphics.h"
#include "screen.h"
#include "sprite.h"
#include "text.h"

class HighScoreScreen : public Screen {
  public:

    HighScoreScreen(int score);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    Screen* next_screen() const override;
    std::string get_music_track() const override;

  private:

    struct HighScore {
      HighScore(const std::string& name, int score) : name(name), score(score) {}
      std::string name;
      int score;
    };

    Text text_;
    Sprite logo_;
    size_t place_;
    int score_;

    std::vector<HighScore> top_scores_;

    bool entering_name() const;
    void save_scores() const;

};
