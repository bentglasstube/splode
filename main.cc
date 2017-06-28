#include "game.h"

#include "title_screen.h"

int main(int, char**) {
  Game::Config config;

  config.graphics.title = "Splode";
  config.graphics.width = 800;
  config.graphics.height = 800;
  config.graphics.fullscreen = false;

  TitleScreen *s = new TitleScreen();

  Game game(config);
  game.loop(s);

  return 0;
}
