#ifndef _Rocket_H
#define _Rocket_H

class Rocket{
    private:
        //Laser Image
        ALLEGRO_BITMAP* rocketImg;             //Image of the rocket

        //Variables
        float xCoord;                          //X-Coordinate of the rocket on screen
        float yCoord;                          //Y-Coordinate of the rocket on screen
        float angle;                           //Angle (Degrees) the rocket is traveling
        int   damage;                          //Damage the rocket will cause
        int   speed;                           //Speed the rocket travels
    public:
        //Constructor
        Rocket(float, float, float);            //Constructor
        //Computational
        void  moveForward();                    //Propels laser forward at it's angle
        //Getters
        float           getRadiansAngle();      //Returns angle in radians
        float           getDegreeAngle();       //Returns angle in degrees
        int             getXCoord();            //Returns current xCoord
        int             getYCoord();            //Returns current yCoord
        ALLEGRO_BITMAP* getBitmap();            //Returns Image
        //Deconstructor
        ~Rocket();                              //Destructor
};

//Constructor
Rocket::Rocket(float x, float y, float a) {
    rocketImg = al_load_bitmap("Images/Rocket.png");                   //Sets image
    al_convert_mask_to_alpha(rocketImg, al_map_rgb(255,255,255));     //Mask the white background of the rocketImg
    xCoord = x;                                                      //Sets xCoord from x variable passed in
    yCoord = y;                                                      //Sets yCoord from y variable passed in
    angle = a;                                                       //Sets angle from a variable passed in
    damage = 1;                                                      //Default damage to 1
    speed = 5;                                                       //Default speed to 5
}

//Moves rocket forward based on the angle of the image
void Rocket::moveForward() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    xCoord += sin(radians) * speed;         //Compute the next x coordinate based on the angle
    yCoord += -cos(radians) * speed;        //Compute the next y coordinate based on the angle
}

//Convert the angle from degrees to radians and returns the radians
float Rocket::getRadiansAngle() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    return radians;                         //Return radians
}

//Return the angle as degrees
float Rocket::getDegreeAngle() {
    return angle;
}

//Return the X-Coordinate
int Rocket::getXCoord() {
    return xCoord;
}

//Return the Y-Coordinate
int Rocket::getYCoord() {
    return yCoord;
}

//Return the rocketImg (for drawing)
ALLEGRO_BITMAP* Rocket::getBitmap() {
    return rocketImg;
}

//Deconstructor
Rocket::~Rocket() {
    al_destroy_bitmap(rocketImg);     //Destroy the rocketImg bitmap
}

#endif

