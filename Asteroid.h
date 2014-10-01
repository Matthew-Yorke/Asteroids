#ifndef _ASTEROID_H
#define _ASTEROID_H

class Asteroid{
    private:
        //Laser Image
        ALLEGRO_BITMAP* asteroidImg;           //Image of the asteroid

        //Variables
        float xCoord;                          //X-Coordinate of the asteroid on screen
        float yCoord;                          //Y-Coordinate of the asteroid on screen
        float angle;                           //Angle (Degrees) the asteroid is traveling
        float rotation;                        //Angle of current rotation of the asteroid
        int   health;                          //Health the asteroid has before being destroyed
        int   speed;                           //Speed the asteroid travels

        //Computational
        void screenEdgeCollision();            //Check if the ship touches edge of screen
    public:
        //Constructor
        Asteroid(float, float, float);         //Constructor
        //Computational
        void moveForward();                    //Propels asteroid forward at it's angle
        //Getters
        float           getRadiansAngle();     //Returns angle in radians
        float           getDegreeAngle();      //Returns angle in degrees
        float           getRotation();         //Returns angle of rotation
        int             getXCoord();           //Returns current xCoord
        int             getYCoord();           //Returns current yCoord
        ALLEGRO_BITMAP* getBitmap();           //Returns Image
        //Deconstructor
        ~Asteroid();                           //Destructor
};

//Constructor
Asteroid::Asteroid(float x, float y, float a) {
    asteroidImg = al_load_bitmap("Images/Asteroid_Large.png");          //Sets image
    al_convert_mask_to_alpha(asteroidImg, al_map_rgb(255,255,255));     //Mask the white background of the asteroidImg
    xCoord = x;                                                         //Sets xCoord from x variable passed in
    yCoord = y;                                                         //Sets yCoord from y variable passed in
    angle = a;                                                          //Sets angle from a variable passed in
    rotation = 0;                                                       //Default rotation to 0
    health = 1;                                                         //Default health to 1
    speed = 2;                                                          //Default speed to 2
}

//Moves ship forward based on the angle of the image
void Asteroid::moveForward() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    xCoord += sin(radians) * speed;         //Compute the next x coordinate based on the angle
    yCoord += -cos(radians) * speed;        //Compute the next y coordinate based on the angle

    screenEdgeCollision();                  //Check collision if the asteroid collides with a screen edge
}

//Check to make sure the xCoord is not hitting the screen edge
//If it does, send the asteroid to the opposite side of the screen
void Asteroid::screenEdgeCollision() {
    if(xCoord < 0)         //Check if hitting top of screen
        xCoord += 800;     //Send asteroid to bottom of screen
    if(xCoord > 800)       //Check if hitting bottom of screen
        xCoord -= 800;     //Send asteroid to top of screen
    if(yCoord < 0)         //Check if hitting left side of screen
        yCoord += 600;     //Send asteroid to right side of screen
    if(yCoord > 600)       //Check if hitting right side of screen
        yCoord -= 600;     //Send asteroid to left side of screen
}

//Convert the angle from degrees to radians and returns the radians
float Asteroid::getRadiansAngle() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    return radians;                         //Return radians
}

//Return the angle as degrees
float Asteroid::getDegreeAngle() {
    return angle;
}

//Return the angle as degrees
float Asteroid::getRotation() {
    float radians = (angle + rotation) * (3.14/180);     //Change degrees for the asteroids angle plus its rotation to radians
    rotation += speed;                                   //Increment rotation
    if(rotation >= 360)                                  //Check if rotation exceeds 360 (to keep angle between 0-360)
        rotation = 0;                                    //Reset rotation to 0
    return radians;                                      //Return radians
}

//Return the X-Coordinate
int Asteroid::getXCoord() {
    return xCoord;
}

//Return the Y-Coordinate
int Asteroid::getYCoord() {
    return yCoord;
}

//Return the asteroidImg (for drawing)
ALLEGRO_BITMAP* Asteroid::getBitmap() {
    return asteroidImg;
}

//Deconstructor
Asteroid::~Asteroid() {
    al_destroy_bitmap(asteroidImg);     //Destroy the asteroidImg bitmap
}

#endif

