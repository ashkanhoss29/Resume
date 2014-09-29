// Ashkan Hosseini
// Section - 006
// 3/30/10


#ifndef WEAPONS_H
#define WEAPONS_H

#include <sstream> //used in other files where "Weapons.h" is #included
#include <string> //needed for member function stringRep()

using namespace std;

//The type of weapon that a weapon can be is signified by one of these enum
enum WEAPON_TYPES {
	BB, Missile, Laser, Nuke, Phase_Cannon, Anti_Matter_Blast, Singularity_Grenade
};

//Weapons class is used to represent a weapon of a "Battle Explorer":
class Weapons
{
public:
	Weapons(); //Constructor
	~Weapons(); //Deconstructor
	int getDamage(); //returns the value of damage
	void setDamage(int d); //sets the value for damage to value of parameter d
	int getAmmo(); //returns the value of ammo
	void setAmmo(int a); //sets the value of ammo to the value of parameter a
	bool getWeaponFired(); //returns the value of weapon_fired
	void setWeaponFired(bool wf); //sets the value of weapon_fired to the value of parameter wf
	int getRange(); //returns the value of range
	void setRange(int r); //sets the value of range to the value of parameter r
	WEAPON_TYPES getType(); //returns the value of type
	void setType(WEAPON_TYPES wt); //sets the value of type to the value of parameter wt
	int powerFactor(); //determines power factor, sets it to variable factor, then returns the value
	string stringRep(); //returns the settings of the weapon in valid JSON form {Attribute:value}

private:
	WEAPON_TYPES type; //variable to store the value of the type of weapon
	int ammo; //stores value for amount of ammo
	int damage; //stores value for damage of weapon
	int range; //stores value for range of weapon
	bool weapon_fired; //stores value as true if weapon has been fired and false if not
};

#endif