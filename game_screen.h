#pragma once

#include <memory>
#include <string>

#include "audio.h"
#include "graphics.h"
#include "screen.h"
#include "text.h"

#include "level.h"
#include "ship.h"

class GameScreen : public Screen {
  public:

    void init() override;
    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    std::string get_music_track() const override;

    Screen* next_screen() override;

  private:

    std::unique_ptr<Ship> ship_;
    std::unique_ptr<Text> text_;
    std::unique_ptr<Level> level_;
    int score_, lives_, level_number_;

};
