#include "level.h"

Level::Level(const std::string& data) : name_(""), fuel_(0), start_(0, 0), pad_(0, 0), terrain_() {
  name_ = data.substr(1, (unsigned char) data.at(0));
  size_t offset = (unsigned char) data.at(0) + 1;

  fuel_ = (unsigned char) data.at(offset++);
  start_.x = (unsigned char) data.at(offset++);
  start_.y = 256 - (unsigned char) data.at(offset++);
  pad_.x = (unsigned char) data.at(offset++);
  pad_.y = 256 - (unsigned char) data.at(offset++);

  const size_t segments = (unsigned char) data.at(offset++);
  for (size_t i = 0; i < segments; ++i) {
    const size_t length = (unsigned char) data.at(offset++);
    PolyLine seg;
    for (size_t j = 0; j < length; ++j) {
      const int x = (unsigned char) data.at(offset++);
      const int y = 256 - (unsigned char) data.at(offset++);
      seg.add(x, y);
    }
    terrain_.emplace_back(seg);
  }
}

void Level::draw(Graphics& graphics) const {
  for (const auto& p : terrain_) {
    p.draw(graphics, 0x00ff00ff);
  }
  const SDL_Rect r = { pad_.x - 7.5, pad_.y, 15, 5 };
  graphics.draw_rect(&r, 0xffffffff, false);
}

Point Level::get_start() const {
  return start_;
}

int Level::get_fuel() const {
  return fuel_;
}
