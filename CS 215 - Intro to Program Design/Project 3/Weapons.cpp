// Ashkan Hosseini
// Section - 006
// 3/30/10


#include "Weapons.h" //Weapons class specification

Weapons::Weapons()
{
	damage=ammo=range=weapon_fired=0; //variables damage, ammo, range, and weapon_fired are set to 0
	type = BB; //variable type is set to BB
}

Weapons::~Weapons()
{
}

string Weapons::stringRep() 
{
	string stringtype; //used to stored string representation of weapon type
	//This if statement will determine the string representation of the enum value of variable type:
	if(type == BB)
		stringtype = "BB";
	else if(type == Missile)
		stringtype = "Missile"; 
	else if(type == Laser)
		stringtype = "Laser";
	else if(type == Nuke)
		stringtype = "Nuke";
	else if(type == Phase_Cannon)
		stringtype = "Phase Cannon";
	else if(type == Anti_Matter_Blast)
		stringtype = "Anti-Matter Blast";
	else
		stringtype = "Singularity Grenade";


	//The following lines of code with variable addString will create a string in valid JSON representing the values of the weapon:
	stringstream addString; //variable of type stringstream is used to add multiple strings and other variable types into one string

	//The strings up until weapons_fired ared added to variable addString in correct JSON format:
	addString << "{ \"type\":\"" << stringtype << "\", \"ammo\":" << ammo << ", \"damage\":" << damage << ", \"maxRange\":"
		<< range << ", \"fired\":";

	//This if statement will add to addString a string representing the boolean value of variable weapon_fired:
	if(weapon_fired)
		addString << "true";
	else
		addString << "false";

	//Power factor and the end of the string are added:
	addString << ", \"powerFactor\":" << powerFactor() << " }";


	return addString.str(); //value of addString returned as type string
}

int Weapons::getDamage()
{
	return damage; //returns value of variable damamge
}

void Weapons::setDamage(int d)
{
	damage = d; //sets variable damage to the value of parameter d
}

int Weapons::getAmmo()
{
	return ammo; //returns the value of variable ammo
}

void Weapons::setAmmo(int a)
{
	ammo = a; //sets the value of variable ammo to parameter d
}

int Weapons::getRange()
{
	return range; //returns the value of variable range
}

void Weapons::setRange(int r)
{
	range = r; //sets the value of variable range to parameter r
}

bool Weapons::getWeaponFired()
{
	return weapon_fired; //returns the value of variable weapon_fired
}

void Weapons::setWeaponFired(bool wf)
{
	weapon_fired = wf; //sets the value of weapon_fired to parameter wf
}

WEAPON_TYPES Weapons::getType()
{
	return type; //returns the value of type
}

void Weapons::setType(WEAPON_TYPES wt)
{
	type = wt; //sets value of variable type to parameter wt
}

int Weapons::powerFactor()
{
	return damage * ammo; //returns the product of variables damage and ammo
}