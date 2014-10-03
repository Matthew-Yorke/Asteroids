#ifndef _WEAPON_H
#define _WEAPON_H

class Weapon{
    private:
        //Laser Image
        ALLEGRO_BITMAP* weaponImg;             //Image of the weapon

        //Variables
        float xCoord;                          //X-Coordinate of the weapon on screen
        float yCoord;                          //Y-Coordinate of the weapon on screen
        float angle;                           //Angle (Degrees) the weapon is traveling
        int   damage;                          //Damage the weapon will cause
        int   speed;                           //Speed the weapon travels
    public:
        //Constructor
        Weapon(float, float, float, int, int, std::string);            //Constructor
        //Computational
        void  moveForward();                    //Propels weapon forward at it's angle
        //Getters
        float           getRadiansAngle();      //Returns angle in radians
        float           getDegreeAngle();       //Returns angle in degrees
        int             getXCoord();            //Returns current xCoord
        int             getYCoord();            //Returns current yCoord
        ALLEGRO_BITMAP* getBitmap();            //Returns Image
        //Deconstructor
        ~Weapon();                              //Destructor
};

//Constructor
Weapon::Weapon(float x, float y, float a, int dmg, int spd, std::string path) {
    const char* p = path.c_str();
    weaponImg = al_load_bitmap(p);                                   //Sets image
    al_convert_mask_to_alpha(weaponImg, al_map_rgb(255,255,255));    //Mask the white background of the rocketImg
    xCoord = x;                                                      //Sets xCoord from x variable passed in
    yCoord = y;                                                      //Sets yCoord from y variable passed in
    angle = a;                                                       //Sets angle from a variable passed in
    damage = dmg;                                                    //Default damage to 1
    speed = spd;                                                     //Default speed to 5
}

//Moves weapon forward based on the angle of the image
void Weapon::moveForward() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    xCoord += sin(radians) * speed;         //Compute the next x coordinate based on the angle
    yCoord += -cos(radians) * speed;        //Compute the next y coordinate based on the angle
}

//Convert the angle from degrees to radians and returns the radians
float Weapon::getRadiansAngle() {
    float radians = angle * (3.14/180);     //Change degrees to radians
    return radians;                         //Return radians
}

//Return the angle as degrees
float Weapon::getDegreeAngle() {
    return angle;
}

//Return the X-Coordinate
int Weapon::getXCoord() {
    return xCoord;
}

//Return the Y-Coordinate
int Weapon::getYCoord() {
    return yCoord;
}

//Return the weaponImg (for drawing)
ALLEGRO_BITMAP* Weapon::getBitmap() {
    return weaponImg;
}

//Deconstructor
Weapon::~Weapon() {
    al_destroy_bitmap(weaponImg);     //Destroy the weaponImg bitmap
}

#endif


