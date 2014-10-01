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
        int   health;                          //Health the ship has before being destroyed
        int   speed;                           //Speed the laser travels

        //Computational
        void screenEdgeCollision();            //Check if the ship touches edge of screen
    public:
        //Constructor
        Ship(float, float);                    //Constructor
        //Computational
        void turnLeft();                       //Turns the ship left by decreasing the angle
        void turnRight();                      //Turns the ship right by increasing the angle
        void moveForward();                    //Propels ship forward at it's angle
        //Getters
        float           getRadiansAngle();     //Returns angle in radians
        float           getDegreeAngle();      //Returns angle in degrees
        int             getXCoord();           //Returns current xCoord
        int             getYCoord();           //Returns current yCoord
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
    angle = 0;                                                      //Default angle to 5
    health = 1;                                                     //Default health points to 5
    speed = 3;                                                      //Default speed to 5
}

//Turns ship left by decreasing the angle by the ship's speed
void Ship::turnLeft() {
    angle -= speed;       //Decrease angle by speed
    if(angle < 0)         //Make sure angle isn't below 0 (to stay within a 0-360 number)
        angle += 360;     //Add 360 if angle goes below 0
}

//Turns ship right by increasing the angle by the ship's speed
void Ship::turnRight() {
    angle += speed;       //Increase angle by speed
    if(angle >= 360)      //Make sure angle isn't over 360 (to stay within a 0-360 number)
        angle -= 360;     //Subtract 360 if angle goes over 360
}

//Moves ship forward based on the angle of the image
void Ship::moveForward() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    xCoord += sin(radians) * speed;         //Compute the next x coordinate based on the angle
    yCoord += -cos(radians) * speed;        //Compute the next y coordinate based on the angle

    screenEdgeCollision();                  //Check collision if the ship collides with a screen edge
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

//Return the shipImg (for drawing)
ALLEGRO_BITMAP* Ship::getBitmap() {
    return shipImg;
}

//Deconstructor
Ship::~Ship() {
    al_destroy_bitmap(shipImg);     //Destroy the laserImg bitmap
}

#endif
