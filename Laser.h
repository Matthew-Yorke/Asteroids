#ifndef _LASER_H
#define _LASER_H

class Laser{
    private:
        //Laser Image
        ALLEGRO_BITMAP* laserImg;              //Image of the laser

        //Variables
        float xCoord;                          //X-Coordinate of the laser on screen
        float yCoord;                          //Y-Coordinate of the laser on screen
        float angle;                           //Angle (Degrees) the laser is traveling
        int   damage;                          //Damage the laser will cause
        int   speed;                           //Speed the laser travels
    public:
        //Constructor
        Laser(float, float, float);            //Constructor
        //Computational
        void  moveForward();                   //Propels laser forward at it's angle
        //Getters
        float           getRadiansAngle();     //Returns angle in radians
        float           getDegreeAngle();      //Returns angle in degrees
        int             getXCoord();           //Returns current xCoord
        int             getYCoord();           //Returns current yCoord
        ALLEGRO_BITMAP* getBitmap();           //Returns Image
        //Deconstructor
        ~Laser();                              //Destructor
};

//Constructor
Laser::Laser(float x, float y, float a) {
    laserImg = al_load_bitmap("Images/laser.png");                   //Sets image
    al_convert_mask_to_alpha(laserImg, al_map_rgb(255,255,255));     //Mask the white background of the laserImg
    xCoord = x;                                                      //Sets xCoord from x variable passed in
    yCoord = y;                                                      //Sets yCoord from y variable passed in
    angle = a;                                                       //Sets angle from a variable passed in
    damage = 1;                                                      //Default damage to 1
    speed = 5;                                                       //Default speed to 5
}

//Moves laser forward based on the angle of the image
void Laser::moveForward() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    xCoord += sin(radians) * speed;         //Compute the next x coordinate based on the angle
    yCoord += -cos(radians) * speed;        //Compute the next y coordinate based on the angle
}

//Convert the angle from degrees to radians and returns the radians
float Laser::getRadiansAngle() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    return radians;                         //Return radians
}

//Return the angle as degrees
float Laser::getDegreeAngle() {
    return angle;
}

//Return the X-Coordinate
int Laser::getXCoord() {
    return xCoord;
}

//Return the Y-Coordinate
int Laser::getYCoord() {
    return yCoord;
}

//Return the laserImg (for drawing)
ALLEGRO_BITMAP* Laser::getBitmap() {
    return laserImg;
}

//Deconstructor
Laser::~Laser() {
    al_destroy_bitmap(laserImg);     //Destroy the laserImg bitmap
}

#endif
