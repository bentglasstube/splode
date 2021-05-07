#pragma once

#include <memory>
#include <vector>

#include "audio.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "sprite.h"
#include "text.h"

class TitleScreen : public Screen {
  public:

    void init() override;
    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;
    std::string get_music_track() const override;

    Screen* next_screen() const override;

  private:

    std::unique_ptr<Text> text_;
    std::unique_ptr<Sprite> logo_;

    std::vector<std::string> choices_;
    size_t choice_;
};
