#pragma once

#include <memory>
#include <string>

#include "audio.h"
#include "graphics.h"
#include "screen.h"
#include "text.h"

#include "level.h"
#include "particle.h"
#include "ship.h"

class GameScreen : public Screen {
  public:

    GameScreen(int difficulty, int level);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    std::string get_music_track() const override;

    Screen* next_screen() const override;

  private:

    const double kViewportPadding = 25;

    enum class GameState { INTRO, PLAYING, DEATH, OUTRO };
    enum class DeathReason { NONE, MISS, CRASH, TIP, LEAVE };

    struct ScoreInfo {
      int position, angle, velocity, flips, multiplier;
      int total() const;
    };

    int score_, lives_, level_number_, difficulty_;
    Level level_;
    Ship ship_;
    Text text_;
    ParticleEmitter hull_exploder_;

    GameState state_;
    DeathReason reason_;
    ScoreInfo score_info_;

    void load_level();
    Rect viewport() const;
    void handle_crash();
    void death(Audio& audio);
    std::string death_reason() const;
    void info_box(Graphics& graphics, int w, int h, const std::string& title) const;
    void draw_score_info(Graphics& graphics) const;
    void add_points(int points);
};
