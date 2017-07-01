#include "game_screen.h"

#include <cmath>
#include <iostream>
#include <sstream>

#include "geometry.h"
#include "high_score_screen.h"

void GameScreen::init() {
  text_.reset(new Text("text.png"));
  hull_exploder_.reset(new ParticleEmitter(0x00ffffff));

  score_ = 0;
  lives_ = 5;
  level_number_ = 1;

  load_level();
}

bool GameScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  const bool next = input.key_pressed(SDL_SCANCODE_SPACE) ||
    input.key_pressed(SDL_SCANCODE_RETURN) ||
    input.key_pressed(SDL_SCANCODE_ESCAPE);
  const bool up = input.key_held(SDL_SCANCODE_W) || input.key_held(SDL_SCANCODE_UP);
  const bool left = input.key_held(SDL_SCANCODE_A) || input.key_held(SDL_SCANCODE_LEFT);
  const bool right = input.key_held(SDL_SCANCODE_D) || input.key_held(SDL_SCANCODE_RIGHT);

  hull_exploder_->update(elapsed);

  switch (state_) {
    case GameState::INTRO:
      if (next) state_ = GameState::PLAYING;
      break;

    case GameState::PLAYING:
      {
        ship_->set_engines(up, left, right);
        ship_->update(audio, elapsed * (difficulty_ + 1) / 2);

        const Point p = ship_->position();
        if (p.x < 0 || p.x > 255 || p.y < 0) {
          reason_ = DeathReason::LEAVE;
          death(audio);
        }

        if (p.y > 255) {
          level_number_ = 0;
          load_level();
        }

        if (level_->intersect(ship_->hull())) {
          handle_crash();
          if (reason_ == DeathReason::NONE) {
            state_ = GameState::OUTRO;
            audio.stop_samples();
            audio.play_sample("land.wav");
            add_points(score_info_.total());
          } else {
            death(audio);
          }
        }
      }

      break;

    case GameState::DEATH:
      if (next) {
        if (lives_ > 0) {
          load_level();
        } else {
          return false;
        }
      }

      break;

    case GameState::OUTRO:
      if (next) {
        if (level_number_ > 0) ++level_number_;
        if (level_number_ > 20) return false;
        load_level();
      }
      break;
  }

  return true;
}

void GameScreen::draw(Graphics& graphics) const {
  const Rect v = viewport();

  level_->draw(graphics, v);
  if (state_ != GameState::DEATH) ship_->draw(graphics, v);
  hull_exploder_->draw(graphics, v);

  const SDL_Rect fuel = { 8, 24, (int)(8 * ship_->fuel()), 8 };
  graphics.draw_rect(&fuel, 0x000000ff, true);
  graphics.draw_rect(&fuel, 0xffff00ff, false);

  const SDL_Rect border = { 0, 0, graphics.width(), graphics.height() };
  graphics.draw_rect(&border, 0x00ff00ff, false);

  text_->draw(graphics, std::to_string(score_), 8, 8);
  text_->draw(graphics, "Ships: " + std::to_string(lives_), graphics.width() - 8, 8, Text::Alignment::RIGHT);

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
  HighScoreScreen* scores = new HighScoreScreen();
  scores->set_score(score_);
  return scores;
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
  audio.stop_samples();
  audio.play_sample("crash.wav");
  --lives_;

  const PolyLine& hull = ship_->hull();
  for (int i = 0; i < 200; ++i) {
    hull_exploder_->emit(hull.point(i % 3), i / 5.0);
  }
}

std::string GameScreen::death_reason() const {
  switch (reason_) {
    case DeathReason::NONE: return "You found a bug in the game :(";
    case DeathReason::MISS: return "You missed the landing pad.";
    case DeathReason::CRASH: return "You landed too hard.";
    case DeathReason::TIP: return "You landed too crooked.";
    case DeathReason::LEAVE: return "You flew out of bounds.";
  }

  return "";
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

void GameScreen::add_points(int points) {
  const int before = score_ / 20000;
  score_ += points;
  const int after = score_ / 20000;
  if (after > before) ++lives_;
}

int GameScreen::ScoreInfo::total() const {
  return multiplier * (position + angle + velocity + flips);
}
