#ifndef _SHIP_H
#define _SHIP_H

class Ship{
    private:
        //Ship Image
        ALLEGRO_BITMAP* shipImg;               //Image of the ship

        //Variables
        float xCoord;                          //X-Coordinate of the laser on screen
        float yCoord;                          //Y-Coordinate of the laser on screen
        float angle;                           //Angle (Degrees) the laser is traveling
        float energy;
        int   health;                          //Health the ship has before being destroyed
        int   speed;                           //Speed the laser travels
        int   rocketCount;

        //Animation
        int frameX;
        int frameY;
        int frameTimer;

        //Computational
        void screenEdgeCollision();            //Check if the ship touches edge of screen
    public:
        //Constructor
        Ship(float, float);                    //Constructor
        //Computational
        void update(bool, bool, bool);
        void idle();                           //Set frames to idle ship state
        void turnLeft();                       //Turns the ship left by decreasing the angle
        void turnRight();                      //Turns the ship right by increasing the angle
        void moveForward(bool, bool);          //Propels ship forward at it's angle
        void refillEnergy();
        Weapon* fireLaser();
        Weapon* fireRocket();
        void draw(ALLEGRO_DISPLAY*);
        //Getters
        float           getRadiansAngle();     //Returns angle in radians
        float           getDegreeAngle();      //Returns angle in degrees
        int             getXCoord();           //Returns current xCoord
        int             getYCoord();           //Returns current yCoord
        float           getEnergy();           //Returns current energy count
        int             getRocketCount();      //Returns current rocketCount
        ALLEGRO_BITMAP* getBitmap();           //Returns Image
        //Deconstructor
        ~Ship();                               //Deconstructor
};

//Constructor
Ship::Ship(float x, float y) {
    shipImg = al_load_bitmap("Images/spaceship_1_sheet.png");       //Sets image
    al_convert_mask_to_alpha(shipImg, al_map_rgb(255,255,255));     //Mask the white background of the shipImg
    xCoord = x;                                                     //Sets xCoord from x variable passed in
    yCoord = y;                                                     //Sets xCoord from x variable passed in
    energy = 100;
    angle = 0;                                                      //Default angle to 5
    health = 1;                                                     //Default health points to 5
    speed = 4;                                                      //Default speed to 5
    rocketCount = 3;


    frameX = 0;
    frameY = 0;
    frameTimer = 15;
}

void Ship::update(bool left, bool right, bool forwrd) {
    //Movement
    if(left == true)
        turnLeft();
    if(right == true)
        turnRight();
    if(forwrd == true)
        moveForward(left, right);
    if(left != true && right != true && forwrd != true)
        idle();

    //Refill Energy
    refillEnergy();
}

//Return ship to non-movement state
void Ship::idle() {
    frameX = 0;
    frameY = 0;
}

//Turns ship left by decreasing the angle by the ship's speed
void Ship::turnLeft() {
    frameY = 96;
    if(frameTimer == 0) {
        frameX += 32;
        if(frameX > 32)
            frameX = 0;
        frameTimer = 15;
    }
    else
        frameTimer--;

    angle -= speed;       //Decrease angle by speed
    if(angle < 0)         //Make sure angle isn't below 0 (to stay within a 0-360 number)
        angle += 360;     //Add 360 if angle goes below 0
}

//Turns ship right by increasing the angle by the ship's speed
void Ship::turnRight() {
    frameY = 48;
    if(frameTimer == 0) {
        frameX += 32;
        if(frameX > 32)
            frameX = 0;
        frameTimer = 15;
    }
    else
        frameTimer--;

    angle += speed;       //Increase angle by speed
    if(angle >= 360)      //Make sure angle isn't over 360 (to stay within a 0-360 number)
        angle -= 360;     //Subtract 360 if angle goes over 360
}

//Moves ship forward based on the angle of the image
void Ship::moveForward(bool left, bool right) {
    frameY = 144;
    if(left != true && right != true) {
        if(frameTimer == 0) {
            frameX += 32;
            if(frameX > 32)
                frameX = 0;
            frameTimer = 15;
        }
        else
            frameTimer--;
    }

    float radians = angle * (3.14/180);     //Change degrees to radians
    xCoord += sin(radians) * speed;         //Compute the next x coordinate based on the angle
    yCoord += -cos(radians) * speed;        //Compute the next y coordinate based on the angle

    screenEdgeCollision();                  //Check collision if the ship collides with a screen edge
}

void Ship::refillEnergy() {
    if(energy < 100)
        energy += 1;
    if(energy > 100)
        energy = 100;
}

Weapon* Ship::fireLaser() {
    energy -= 50;
    return new Weapon(xCoord, yCoord, angle, 1, 5, "Images/Laser.png");
}
Weapon* Ship::fireRocket() {
    rocketCount--;
    return new Weapon(xCoord, yCoord, angle, 3, 7, "Images/Rocket.png");
}

//Check to make sure the xCoord is not hitting the screen edge
//If it does, send the ship to the opposite side of the screen
void Ship::screenEdgeCollision() {
    if(xCoord < 0)         //Check if hitting top of screen
        xCoord += 800;     //Send ship to bottom of screen
    if(xCoord > 800)       //Check if hitting bottom of screen
        xCoord -= 800;     //Send ship to top of screen
    if(yCoord < 0)         //Check if hitting left side of screen
        yCoord += 600;     //Send ship to right side of screen
    if(yCoord > 600)       //Check if hitting right side of screen
        yCoord -= 600;     //Send ship to left side of screen
}

//Draw the ship on the screen
void Ship::draw(ALLEGRO_DISPLAY* display) {
    ALLEGRO_BITMAP* rotated = NULL;
    rotated = al_create_bitmap(32, 48);
    al_set_target_bitmap(rotated);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_bitmap_region(getBitmap(),
        frameX, frameY, 32, 48, 0, 0, 0);
    al_convert_mask_to_alpha(rotated, al_map_rgb(0,0,0));
    al_set_target_bitmap(al_get_backbuffer(display));
    al_draw_rotated_bitmap(rotated,
                   al_get_bitmap_width(rotated)/2,
                   al_get_bitmap_height(rotated)/2,
                   getXCoord(),
                   getYCoord(),
                   getRadiansAngle(),
                   0);
    al_destroy_bitmap(rotated);
}

//Convert the angle from degrees to radians and returns the radians
float Ship::getRadiansAngle() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    return radians;                         //Return radians
}

//Return the angle as degrees
float Ship::getDegreeAngle() {
    return angle;
}

//Return the X-Coordinate
int Ship::getXCoord() {
    return xCoord;
}

//Return the Y-Coordinate
int Ship::getYCoord() {
    return yCoord;
}

//Return energy
float Ship::getEnergy() {
    return energy;
}

//Return rocketCount
int Ship::getRocketCount() {
    return rocketCount;
}

//Return the shipImg (for drawing)
ALLEGRO_BITMAP* Ship::getBitmap() {
    return shipImg;
}

//Deconstructor
Ship::~Ship() {
    al_destroy_bitmap(shipImg);     //Destroy the laserImg bitmap
}

#endif
