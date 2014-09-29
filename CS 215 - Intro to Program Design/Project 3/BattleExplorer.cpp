// Ashkan Hosseini
// Section - 006
// 3/30/10


#include "BattleExplorer.h" //specifications of class BattleExplorer (this also effectivaly includes other libraries needed such as "Weapons.h")

BattleExplorer::BattleExplorer()
{
	arr_size = 4; //size of array will start at 4 (doubled whenever array is full in addWeapon function)
	weaponCount = 0; //weaponCount set to 0
}

BattleExplorer::~BattleExplorer()
{
}

void BattleExplorer::addWeapon(Weapons w)
{
	//This if statement will check the state of the array and what needs to be done accordingly.
	//1. If an array has not yet been created, a new one will be made with a size of 4 index.  
	//2. If array is full, array size is doubled and a new array is created with that many index (previous data from the full array is transfered over to new one).
	if(weaponCount == 0)
	{
		weapons = new Weapons[arr_size]; //when weaponCount is zero it indicates that no weapon has been loaded and an array of waeapons has not yet been created, so an array is created/allocated
	}
	else if(weaponCount+1 > arr_size)
	{
		Weapons * tempweapons; //a temp. array which will be the bridge between the old, full array, and the new one
		tempweapons = new Weapons[arr_size];
		for(int x = 0; x < weaponCount; x++)
		{
			tempweapons[x] = weapons[x]; //data of weapons array is stored into tempweapons so we can delete weapons array and create a new one
		}
		delete[] weapons; //weapons array is deleted so we can make a new one with twice the index
		arr_size = arr_size*2; //index size is doubled
		weapons = new Weapons[arr_size]; //a new weapons array is allocated with the newly doubled arr_size
		for(int x = 0; x < weaponCount; x++)
		{
			weapons[x] = tempweapons[x]; //data that was temp. stored in tempweapons is transfered back into weapons array
		}
		delete[] tempweapons; //tempweapons is deleted
	}

	weapons[weaponCount] = w; //with the array check (above if statement) complete, we can load new weapon into weapons array


	int ct = weaponCount; //ct is a variable used to count through the weapons array for the below loop.  weaponCount is not used because we need to subtract from the variable used to count through the array.

	//This loop will go through the array from last index to first and order the weapons stored based on their power factor (largest to smallest):
	while(ct > 0)
	{
		//If the weapon at index ct has a larger power factor than the weapon before it, those two weapons are swapped:
		if(weapons[ct].powerFactor() > weapons[ct-1].powerFactor())
		{
			Weapons tempweapons = weapons[ct-1]; //the weapon stored at index ct-1 is temp. stored into tempweapons
			weapons[ct-1] = weapons[ct]; //weapon at index ct of array is stored into array at index ct-1
			weapons[ct] = tempweapons; //weapon stored in tempweapons is now stored into weapons array at index ct, finishing the swap
		}
		ct--; //counter is subtracted by one
	}

	weaponCount++; //weaponCount is incremented for use with the next weapon that is added
}

string BattleExplorer::battleDump()
{
	stringstream addStrings; //this variable will be used to add multiple strings together (also so we can easily transfere and change int variables into strings (so 5 will become "5")).
	string addDump(dump()); //dump() is called and returns the string representation (in JSON format) of the BattleExplorers settings for everything up till weapon settings.  This string is stored in addStrings

	addStrings << "{" << addDump << ", \"weapons\" : ["; //the beginnings of the weapons settings in proper JSON format are added

	//This loop examines a weapon in weapons array at index x and adds the proper string to addStrings.  The loop will run depending on the size of weaponCount, which is the amount of weapons stored in the array.
	for(int x = 0; x < weaponCount; x++)
	{
		//If a weapon has been previously examined, a comma will be added to sperate the two weapons' string representations:
		if(x > 0)
		{
			addStrings << ",";
		}
		addStrings << weapons[x].stringRep(); //stringRep() method function is called for the weapon at index x of weapons array so the string representation of that weapon can be added to addStrings
	}

	addStrings << "]}}"; //the end of the string representation of BattleExplorer is added to addStrings

	return addStrings.str(); //the value of addStrings is return as one proper string (type string and not stringstream)
}