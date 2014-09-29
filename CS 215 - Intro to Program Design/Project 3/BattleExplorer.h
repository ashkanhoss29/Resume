// Ashkan Hosseini
// Section - 006
// 3/30/10


#ifndef BATTLEEXPLORER_H
#define BATTLEEXPLORER_H

#include "Explorer.h" //Explorer specifications needed becuase we will be expanding the class with BattleExplorer class.
#include "Weapons.h" //Weapons objects will be added/used with BattleExplorer (this #include also includes libraries such as <string> that are required here)

//BattleExplorer is a subclass of Explorer class.  We need this so we can add Weapons objects:
class BattleExplorer: public Explorer
{
public:
	BattleExplorer(); //Constructor
	~BattleExplorer(); //Deconstructor
	void addWeapon(Weapons w); //will add variable w (Weapons type) in parameter to weapons array
	string battleDump(); //will return a string in valid JSON form representing the settings for the battle explorer

private:
	Weapons * weapons; //used to create an array of weapons.  It is pointer so we can delete the old array in order to increase index size.
	int arr_size; //size of the Weapon array
	int weaponCount; //counter used to keep track of the number of weapons in the weapons array (if any).
};

#endif