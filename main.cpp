#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include "Game.h"

int main() {
    Game *game = Game::Instance();

    game->Init();
    game->MainLoop();

    return 0;
}

