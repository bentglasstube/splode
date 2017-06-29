#include "game_screen.h"

#include <cmath>
#include <iostream>
#include <sstream>

#include "geometry.h"

void GameScreen::init() {
  text_.reset(new Text("text.png"));

  score_ = 0;
  lives_ = 0;
  level_number_ = 1;

  load_level();
}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  switch (state_) {
    case GameState::INTRO:
      if (input.any_pressed()) state_ = GameState::PLAYING;
      break;

    case GameState::PLAYING:
      {
        ship_->set_engines(
            input.key_held(SDL_SCANCODE_W),
            input.key_held(SDL_SCANCODE_A),
            input.key_held(SDL_SCANCODE_D));
        ship_->update(audio, elapsed * (difficulty_ + 1) / 2);

        const Point p = ship_->position();
        if (p.x < 0 || p.x > 255 || p.y < 0) {
          reason_ = DeathReason::LEAVE;
          death(audio);
        }
        // TODO handle bottom exit

        if (level_->intersect(ship_->hull())) {
          handle_crash();
          if (reason_ == DeathReason::NONE) {
            state_ = GameState::OUTRO;
            audio.play_sample("land.wav");
            score_ += score_info_.total();
          } else {
            death(audio);
          }
        }
      }

      break;

    case GameState::DEATH:
      if (input.any_pressed()) load_level();
      // TODO handle game over
      break;

    case GameState::OUTRO:
      if (input.any_pressed()) {
        ++level_number_;
        load_level();

        // TODO handle last level
      }
      break;
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const Rect v = viewport();

  level_->draw(graphics, v);
  ship_->draw(graphics, v);

  const SDL_Rect fuel_outer = { 5, 5, 10 * level_->fuel(), 10 };
  const SDL_Rect fuel_inner = { 5, 5, 10 * ship_->fuel(), 10 };
  graphics.draw_rect(&fuel_outer, 0xffff00ff, false);
  graphics.draw_rect(&fuel_inner, 0xffff00ff, true);

  const SDL_Rect border = { 0, 0, graphics.width(), graphics.height() };
  graphics.draw_rect(&border, 0x00ff00ff, false);

  text_->draw(graphics, std::to_string(score_), graphics.width() - 8, 8, Text::Alignment::RIGHT);

  switch (state_) {
    case GameState::INTRO:
      info_box(graphics, 200, 32, level_->name());
      break;

    case GameState::DEATH:

      info_box(graphics, 250, 64, "Your head a splode!");
      text_->draw(graphics, death_reason(), graphics.width() / 2, graphics.height() / 2 + 8, Text::Alignment::CENTER);
      break;

    case GameState::OUTRO:
      info_box(graphics, 200, 128, "Good landing!");
      draw_score_info(graphics);
      break;

    default:
      // do nothing
      break;
  }
}

std::string GameScreen::get_music_track() const {
  return "level.ogg";
}

void GameScreen::set_difficulty(int difficulty) {
  difficulty_ = difficulty;
}

Screen* GameScreen::next_screen() {
  return nullptr;
}

void GameScreen::load_level() {
  level_.reset(new Level(Level::kLevelData[level_number_]));
  ship_.reset(new Ship(level_->start(), level_->fuel()));

  state_ = GameState::INTRO;
  reason_ = DeathReason::NONE;
}

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

Rect GameScreen::viewport() const {
  double top = MAX(MIN(ship_->position().y, level_->pad().y) - kViewportPadding, 0);
  double left = MAX(MIN(ship_->position().x, level_->pad().x) - kViewportPadding, 0);
  const double right = MIN(MAX(ship_->position().x, level_->pad().x) + kViewportPadding, 255);
  const double bottom = MIN(MAX(ship_->position().y, level_->pad().y) + kViewportPadding, 255);

  double height = bottom - top;
  double width = right - left;

  if (width < height) {
    width = height;
  } else {
    height = width;
  }

  if (top < 0) top = 0;
  if (left < 0) left = 0;
  if (top + height > 255) top = 255 - height;
  if (left + width > 255) left = 255 - width;

  return { left, top, width, height};
}

void GameScreen::handle_crash() {
  score_info_.position = level_->pad_score(ship_->position());
  score_info_.velocity = ship_->velocity_score();
  score_info_.angle = ship_->angle_score();
  score_info_.flips = ship_->flips() * 2500;
  score_info_.multiplier = 1 + difficulty_;

  if (score_info_.angle <= 0) reason_ = DeathReason::TIP;
  if (score_info_.velocity <= 0) reason_ = DeathReason::CRASH;
  if (score_info_.position <= 0) reason_ = DeathReason::MISS;
}

void GameScreen::death(Audio& audio) {
  state_ = GameState::DEATH;
  audio.play_sample("crash.wav");
  --lives_;
}

std::string GameScreen::death_reason() const {
  switch (reason_) {
    case DeathReason::NONE: return "You found a bug in the game :(";
    case DeathReason::MISS: return "You missed the landing pad.";
    case DeathReason::CRASH: return "You landed too hard.";
    case DeathReason::TIP: return "You landed too crooked.";
    case DeathReason::LEAVE: return "You flew out of bounds.";
  }
}

void GameScreen::info_box(Graphics& graphics, int w, int h, const std::string& title) const {
  const SDL_Rect r = { (graphics.width() - w) / 2, (graphics.height() - h) / 2, w, h };
  graphics.draw_rect(&r, 0x000000ff, true);
  graphics.draw_rect(&r, 0xffffffff, false);

  text_->draw(graphics, title, graphics.width() / 2, r.y + 8, Text::Alignment::CENTER);
}

void GameScreen::draw_score_info(Graphics& graphics) const {
  const int l = graphics.width() / 2 - 84;
  const int r = graphics.width() / 2 + 84;
  const int y = graphics.height() / 2 - 32;

  text_->draw(graphics, "Position", l, y);
  text_->draw(graphics, "Speed", l, y + 16);
  text_->draw(graphics, "Angle", l, y + 32);
  text_->draw(graphics, "Flips", l, y + 48);
  text_->draw(graphics, "Difficulty", l, y + 64);

  text_->draw(graphics, std::to_string(score_info_.position), r, y, Text::Alignment::RIGHT);
  text_->draw(graphics, std::to_string(score_info_.velocity), r, y + 16, Text::Alignment::RIGHT);
  text_->draw(graphics, std::to_string(score_info_.angle), r, y + 32, Text::Alignment::RIGHT);
  text_->draw(graphics, std::to_string(score_info_.flips), r, y + 48, Text::Alignment::RIGHT);
  text_->draw(graphics, "x" + std::to_string(score_info_.multiplier), r, y + 64, Text::Alignment::RIGHT);
}

int GameScreen::ScoreInfo::total() const {
  return multiplier * (position + angle + velocity + flips);
}
