#ifndef _GAME_H
#define _GAME_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_native_dialog.h>
#include <iostream>
#include <cstring>
#include <cmath>
#include <sstream>
#include <list>
#include "Weapon.h"
#include "Ship.h"
#include "Asteroid.h"

class Game {
    private:
        //Bitmaps
        ALLEGRO_BITMAP* background;
        ALLEGRO_BITMAP* hud;

        //Allegro Variables
        ALLEGRO_EVENT_QUEUE* event_queue;
        ALLEGRO_DISPLAY*     display;
        ALLEGRO_TIMER*       timer;
        ALLEGRO_FONT*        font;;

        //Variables
        int   score;        //Tracks players score
        int   level;        //Tracks current level
        int   lives;        //Tracks number of player lives
        int   countdown;    //Countdown for a level to start
        bool  doneLevel;    //Loop for the individual level
        bool  doneGame;     //Loop for the entire game
        bool  setTimer;     //Allows timer to happen
        bool  initGame;

        //Objects
        std::list<Weapon*>   weapons;
        std::list<Asteroid*> asteroids;
        Ship* player;


        //Private Functions
        void initLevel();
        void draw();
        void drawGameOver();

        //Private singleton design
        static Game* _instance;
        Game() {};
    public:
        //For singleton design
        static Game* Instance();

        //Functions
        int Init();
        void MainLoop();
};

//-------------------------------------Singleton Design------------------------------------
Game* Game::_instance = NULL;

Game* Game::Instance() {
    if(_instance == NULL)
        _instance = new Game;
    return _instance;
}
//-----------------------------------End Singleton Design-----------------------------------

//---------------------------------------Game Functions-------------------------------------
int Game::Init() {
    //Intialize game settings
    score = 0;          //Start score at 0
    level = 2;          //Start at level 1
    lives = 3;          //Start with 3 lives

    //Start random number generator
    srand(time(NULL));

    //Make sure allegro intializes
    if(!al_init()) {
        al_show_native_message_box(display, "Error", "Error: al_init", "Failed to initialize allegro!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    //Add the image addon for Allegro
    if(!al_init_image_addon()) {
      al_show_native_message_box(display, "Error", "Error: al_init_image_addon", "Failed to initialize al_init_image_addon!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return -1;
    }

    //Add the primitives addon for Allegro
    if(!al_init_primitives_addon()) {
      al_show_native_message_box(display, "Error", "Error: al_init_primitives_addon", "Failed to initialize al_init_primitives_addon!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return -1;
    }

    //Setup the display for allegro
    display = al_create_display(800, 700);
    if(!display) {
       al_show_native_message_box(display, "Error", "Error: display", "Failed to initialize display!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
      return -1;
    }

    //Install the keyboard for allegro
    al_install_keyboard();

    //Install the font addon for allegro
    al_init_font_addon();

    //Install the ttf(True Type Font) for allegro
    al_init_ttf_addon();

    //Set font to Arial Black
    font = al_load_ttf_font("Fonts/ariblk.ttf", 24, 0);

    //Setup the event_queue
    event_queue = al_create_event_queue();
    if(!event_queue) {
        al_show_native_message_box(display, "Error", "Error: al_create_event_queue", "Failed to initialize al_create_event_queue!",
                                 NULL, ALLEGRO_MESSAGEBOX_ERROR);
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }

    //Set up player ship
    player = new Ship(400-(32/2), 300-(48/2));

    //Set up the bitmaps(images) to be used
    background = al_load_bitmap("Images/Background.png");
    hud = al_load_bitmap("Images/hud.png");

    //Set FPS to 60
    timer = al_create_timer(1.0/60);

    //Register types of events the event_queue will handle
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    return 0;
}

void Game::MainLoop() {
    //Variables
    ALLEGRO_EVENT ev;

    //Local Variables
    bool left;
    bool right;
    bool forwrd;
    bool drawgfx;

    while(!doneGame){
        left = false;
        right = false;
        forwrd = false;
        initLevel();

        //Main Loop
        //Loop until Win(Destroy All Asteroids) or Lose(Run Out of Lives) Conditions
        //TO DO: Win Level Conditions and Game Over Conditions
        while(!doneLevel) {
            drawgfx = false;

            //Obtain event actions before updating
            do{
                al_wait_for_event(event_queue, &ev);
                //Clicking the close button
                if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                    doneLevel = true;
                    doneGame = true;
                    break;
                }
                //Key presses
                else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
                    if(ev.keyboard.keycode == ALLEGRO_KEY_W)
                        forwrd = true;
                    if(ev.keyboard.keycode == ALLEGRO_KEY_A) {
                        if(right != true)
                            left = true;
                    }
                    if(ev.keyboard.keycode == ALLEGRO_KEY_D) {
                        if(left != true)
                            right = true;
                    }
                    if(ev.keyboard.keycode == ALLEGRO_KEY_R) {
                        if(player->getRocketCount() > 0)
                            weapons.push_back(player->fireRocket());
                    }
                    //Level skip for testing...DELETE AFTER FINISHED
                    if(ev.keyboard.keycode == ALLEGRO_KEY_V) {
                        doneLevel = true;
                        level++;
                        if(level > 5)
                            doneGame = true;
                        weapons.clear();
                        asteroids.clear();
                        break;
                    }
                    if(ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                        if(player->getEnergy() - 50 >= 0) {
                            weapons.push_back(player->fireLaser());
                        }
                    }
                    if(ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                        doneLevel = true;
                        doneGame = true;
                        break;
                    }
                }
                else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
                    if(ev.keyboard.keycode == ALLEGRO_KEY_W)
                        forwrd = false;
                    if(ev.keyboard.keycode == ALLEGRO_KEY_A)
                        left = false;
                    if(ev.keyboard.keycode == ALLEGRO_KEY_D)
                        right = false;
                }
                else if(ev.type == ALLEGRO_EVENT_TIMER) {
                    if(setTimer == true) {
                        drawgfx = true;
                        break;
                    }
                    if(ev.timer.source == timer) {
                        player->update(left, right, forwrd);

                        //Updates Weapon List
                        std::list<Weapon*>::iterator itWeap;
                        for(itWeap = weapons.begin(); itWeap != weapons.end(); ++itWeap) {
                            (*itWeap)->moveForward();
                            //Remove out of bounds laser
                            if((*itWeap)->getXCoord() < 0 || (*itWeap)->getXCoord() > 800 || (*itWeap)->getYCoord() < 0 || (*itWeap)->getYCoord() > 600) {
                                (*itWeap)->~Weapon();
                                weapons.erase(itWeap++);
                            }
                        }

                        //Updates Asteroid List
                        std::list<Asteroid*>::iterator astIt;
                        for(astIt = asteroids.begin(); astIt != asteroids.end(); ++astIt)
                            (*astIt)->moveForward();
                        drawgfx = true;
                    }
                }
            }while(!al_is_event_queue_empty(event_queue));

            //Check for drawing
            if(drawgfx == true)
                draw();
        }
    }
    //Draw Game Over Screen
    //drawGameOver();

    //End of game cleanup
    al_destroy_bitmap(background);
    al_destroy_bitmap(hud);
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
}

void Game::initLevel() {
    doneLevel = false;  //Init Level loop
    doneGame = false;   //Init Game loop
    setTimer = true;    //set the levels timer

    int x, y, ang, spd;
    //TO DO: Create Algorithm for how many asteroids per level
    int lrgAst = level + 3;     //Add 1 more Large asteroid every level (starting at 4)
    int medAst = 0.5 * level;   //Add 2 more asteroid every other level (starting at 0)
    int smlAst = 0.25 * level;  //Add 1 more small asteroid every 4 levels (starting at 0)
    bool add;
    std::list<Asteroid*>::iterator astIt;

    //Place Large Asteroids on the level field
    for(int i = 0; i < lrgAst; i++) {
        //Set asteroid parameters randomly on the field (xCoord, yCoord, angle)
        x = rand() % 800;
        y = rand() % 600;
        ang = rand() % 360;
        spd = (rand() % 3) + 1;
        add = true;

        //If outside the safe zone add
        if((x < 300 || x > 500) || (y < 150 || y > 450)) {
            //Create a new asteroid to add to the list
            Asteroid* ast = new Asteroid(x, y, ang, spd, "Large");
            //Check all asteroids in asteroids list
            for(astIt = asteroids.begin(); astIt != asteroids.end(); ++astIt) {
                //Add new asteroid to the list if it does not overlap the previous
                if(!((ast->getXCoord() < (*astIt)->getXCoord()-128 ||
                   ast->getXCoord() > (*astIt)->getXCoord()+128) ||
                   (ast->getYCoord() < (*astIt)->getYCoord()-128 ||
                   ast->getYCoord() > (*astIt)->getYCoord()+128)))
                {
                    delete ast;     //free memory
                    add = false;    //don't allow adding of the new asteroid
                    break;          //break out of checking if asteroids collide
                }
            }
            //Add new asteroid if it did not overlap the current asteroids
            if(add == true)
                asteroids.push_back(ast);
            //Add another pass if the new asteroid did overlap a current asteroid
            else
                i--;
        }
        //If within the safe zone add another pass to add the asteroid
        else
            i--;
    }


    //Place Medium Asteroids on the level field
    for(int i = 0; i < medAst; i++) {
        //Set asteroid parameters randomly on the field (xCoord, yCoord, angle)
        x = rand() % 800;
        y = rand() % 600;
        ang = rand() % 360;
        spd = (rand() % 3) + 1;
        add = true;

        //If outside the safe zone add
        if((x < 300 || x > 500) || (y < 150 || y > 450)) {
            Asteroid* ast = new Asteroid(x, y, ang, spd, "Medium");
            //Add first asteroid
            if(asteroids.empty()) {
                asteroids.push_back(ast);
                continue;
            }
            //Check all asteroids in asteroids list
            for(astIt = asteroids.begin(); astIt != asteroids.end(); ++astIt) {
                //Add new asteroid to the list if it does not overlap the previous
                if(!((ast->getXCoord() < (*astIt)->getXCoord()-128 ||
                   ast->getXCoord() > (*astIt)->getXCoord()+128) ||
                   (ast->getYCoord() < (*astIt)->getYCoord()-128 ||
                   ast->getYCoord() > (*astIt)->getYCoord()+128)))
                {
                    add = false;
                    break;
                }
            }
            if(add == true)
                asteroids.push_back(ast);
            else
                i--;
        }
        else
            i--;
    }

    //Place Small Asteroids on the level field
    //Place Large Asteroids on the level field
    for(int i = 0; i < smlAst; i++) {
        //Set asteroid parameters randomly on the field (xCoord, yCoord, angle)
        x = rand() % 800;
        y = rand() % 600;
        ang = rand() % 360;
        spd = (rand() % 3) + 1;
        add = true;

        //If outside the safe zone add
        if((x < 300 || x > 500) || (y < 150 || y > 450)) {
            Asteroid* ast = new Asteroid(x, y, ang, spd, "Small");
            //Add first asteroid
            if(asteroids.empty()) {
                asteroids.push_back(ast);
                continue;
            }
            //Check all asteroids in asteroids list
            for(astIt = asteroids.begin(); astIt != asteroids.end(); ++astIt) {
                //Add new asteroid to the list if it does not overlap the previous
                if(!((ast->getXCoord() < (*astIt)->getXCoord()-128 ||
                   ast->getXCoord() > (*astIt)->getXCoord()+128) ||
                   (ast->getYCoord() < (*astIt)->getYCoord()-128 ||
                   ast->getYCoord() > (*astIt)->getYCoord()+128)))
                {
                    add = false;
                    break;
                }
            }
            if(add == true)
                asteroids.push_back(ast);
            else
                i--;
        }
        else
            i--;
    }

    //Set level start conditions
    countdown = 360;
    setTimer = true;
}

void Game::drawGameOver() {
    al_clear_to_color(al_map_rgb(0,0,0));
    font = al_load_ttf_font("Fonts/ariblk.ttf", 72, 0);
    al_draw_text(font, al_map_rgb(255,255,255), 500 , 280, ALLEGRO_ALIGN_CENTER, "GAME OVER");
    al_flip_display();
    al_rest(3);
}

void Game::draw() {
    //Clear the display
    al_clear_to_color(al_map_rgb(0,0,0));

    //Draw background
    al_draw_bitmap(background, 0, 0, 0);

    //Draw the ship
    player->draw(display);

    //Draw Asteroids
    std::list<Asteroid*>::iterator astIt;
    for(astIt = asteroids.begin(); astIt != asteroids.end(); ++astIt) {
        if(setTimer != true)
            al_draw_rotated_bitmap((*astIt)->getBitmap(),
                   al_get_bitmap_width((*astIt)->getBitmap())/2,
                   al_get_bitmap_height((*astIt)->getBitmap())/2,
                   (*astIt)->getXCoord(),
                   (*astIt)->getYCoord(),
                   (*astIt)->getRotation(),
                   0);
        else
            al_draw_rotated_bitmap((*astIt)->getBitmap(),
                   al_get_bitmap_width((*astIt)->getBitmap())/2,
                   al_get_bitmap_height((*astIt)->getBitmap())/2,
                   (*astIt)->getXCoord(),
                   (*astIt)->getYCoord(),
                   (*astIt)->getRadiansAngle(),
                   0);
    }

    std::list<Weapon*>::iterator itWeap;
    for(itWeap = weapons.begin(); itWeap != weapons.end(); ++itWeap) {
        al_draw_rotated_bitmap((*itWeap)->getBitmap(),
                   al_get_bitmap_width((*itWeap)->getBitmap())/2,
                   al_get_bitmap_height((*itWeap)->getBitmap())/2,
                   (*itWeap)->getXCoord(),
                   (*itWeap)->getYCoord(),
                   (*itWeap)->getRadiansAngle(),
                   0);
    }

    //Draw HUD
    al_draw_bitmap(hud, 0, 600, 0);
    al_draw_rectangle(200, 625, 600, 650, al_map_rgb(0,0,0), 2);
    al_draw_filled_rectangle(200, 625, 200+player->getEnergy()*4, 650,al_map_rgb(0,0,150));
    std::stringstream ss;
    std::string str;
    const char* cstrLives;
    ss << lives;
    str = "X " + ss.str();
    cstrLives = str.c_str();
    al_draw_text(font, al_map_rgb(255,255,255), 70, 634, ALLEGRO_ALIGN_LEFT, cstrLives);
    const char* cstrScore;
    ss.str("");
    ss << score;
    str = "Score: " + ss.str();
    cstrScore = str.c_str();
    al_draw_text(font, al_map_rgb(255,255,255), 630, 634, ALLEGRO_ALIGN_LEFT, cstrScore);

    if(setTimer == true) {
        const char* cstrCountdown;
        ss.str("");
        ss << countdown / 60;
        str = ss.str();
        cstrCountdown = str.c_str();
        al_draw_text(font, al_map_rgb(255,255,255), player->getXCoord()-8, player->getYCoord()-16, ALLEGRO_ALIGN_LEFT, cstrCountdown);
        countdown--;
        if(countdown < 0)
            setTimer = false;
        al_flip_display();
    }

    //Flip back buffer(what was just drawn on) to be displayed on screen
    al_flip_display();
}

//-------------------------------------End Game Functions-----------------------------------

#endif
