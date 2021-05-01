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

    void init() override;
    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    Screen* next_screen() const override;
    std::string get_music_track() const override;

    void set_score(int score);

  private:

    struct HighScore {
      HighScore(const std::string& name, int score) : name(name), score(score) {}
      std::string name;
      int score;
    };

    std::unique_ptr<Sprite> logo_;
    std::unique_ptr<Text> text_;
    std::vector<HighScore> top_scores_;

    int score_;
    size_t place_;

    bool entering_name() const;
    void save_scores() const;

};
