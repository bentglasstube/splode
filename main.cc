#include "game.h"

#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Splode";
  config.graphics.width = 1920;
  config.graphics.height = 1080;
  config.graphics.fullscreen = true;

  TitleScreen *s = new TitleScreen();

  Game game(config);
  game.loop(s);

  return 0;
}
