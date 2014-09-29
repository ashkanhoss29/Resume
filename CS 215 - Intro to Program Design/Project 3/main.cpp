// Ashkan Hosseini
// Section - 006
// 3/30/10


#include <iostream>
#include "Propulsion.h"
#include "Sensor.h"
#include "Weapons.h"
#include "BattleExplorer.h"

int main()
{
	//Propulsion objects:
	Propulsion p;
	Propulsion p2;

	//Propulsion values:
	p.setType(SOLAR_SAIL);
	p.setConsumption(50);
	p.setMaxSpeed(4455);
	p.setPowerFactor(4);
	p2.setType(FUSION);


	//Sensor objects:
	Sensor s;
	s.setType(LIFEFORM);
	s.setConsumption(24);
	s.setMaxRange(1340);


	//Weapon objects:
	Weapons weapon1;
	Weapons weapon2;
	Weapons weapon3;

	//Weapon values:
	weapon1.setDamage(40);
	weapon1.setAmmo(7);
	weapon1.setRange(5000);
	weapon1.setType(Phase_Cannon);
	weapon1.setWeaponFired(true);
	weapon2.setDamage(100);
	weapon2.setAmmo(10);
	weapon2.setRange(450);
	weapon2.setType(Nuke);
	weapon2.setWeaponFired(false);
	//weapon3 used to test the get functions of Weapons class.  The values of weapon3 are set to those of weapon2:
	weapon3.setDamage(weapon2.getDamage());
	weapon3.setAmmo(weapon2.getAmmo());
	weapon3.setRange(weapon2.getType());
	weapon3.setWeaponFired(weapon2.getWeaponFired());


	//BattleExplorer object initialization
	BattleExplorer battleShip; 

	//BattleExplorer values set:
	battleShip.setName("Ashkan");
	battleShip.setValue(300);
	battleShip.setFuel(1340);
	battleShip.setCrew(16);
	battleShip.setSpeed(67000);
	battleShip.setXpos(1);
	battleShip.setYpos(2);
	battleShip.setZpos(3);
	battleShip.addPropulsion(p);
	battleShip.addPropulsion(p2);
	battleShip.addSensor(s);
	battleShip.addWeapon(weapon1);
	battleShip.addWeapon(weapon2);
	battleShip.addWeapon(weapon3);

	cout << battleShip.battleDump(); //output in valid JSON form battleShips settings including engines, weapons, etc.
}