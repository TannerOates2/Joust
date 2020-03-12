/***************************************
Joust
Author: Tanner
Description: Asks for information about two knights then they joust and which ever one runs out of stamina or gets hit first loses.
***************************************/


#include <iostream>
#include <random>
#include <chrono>
#include<string>
using namespace std;

namespace Random
{   
	static std::default_random_engine rng;
	static bool seeded = false;

	// Random class definition
	class Random
	{
		private:
		std::uniform_int_distribution<int> _dist;

		public:
		Random(int low, int high) : _dist(low, high)
		{ 
			seed();     
		}

		static void seed()
		{
			//we only want to seed once
			if (seeded)
			{
				return; 
			} 
			seeded = true;

			// Deriving seed from current system time
			auto generatedSeed = std::chrono::system_clock::now().time_since_epoch().count();
			rng.seed(generatedSeed);
		}

		int get()
		{
			return _dist(rng);
		}
	};
}

class Weapon
{
private:
	int _staminaCost;
	string _type;
	int _hitChance;

	public:
	Weapon(int sc, string type, int hc)
		: _staminaCost(sc), _type(type), _hitChance(hc)
	{}

	bool wield()
	{
		// consult sequence diagram 
		Random::Random r(1, 100 - _hitChance);
		return (r.get ()< this->_hitChance) ? true : false;
	}

	int getStaminaCost()
	{
		return this->_staminaCost;
	}

	int getHitChance()
	{
		return this->_hitChance;
	}

	string  getWepaonType()
	{
		return this->_type;
	}

	void displayWeaponInfo()
	{
		cout<< this->_type<< " that requires "<< this->_staminaCost
			<< " stamina and has a "<<5<<"% chance to hit\n";
	}
};



class Knight
{
private:
	int _stamina;
	string _name;
	Weapon _weapon;
	bool _onHorse;

	public:
	Knight(string name, int stamina, Weapon weapon)
		: _name(name), _stamina(stamina), _weapon(weapon), _onHorse(true)
	{}

	// gets status of on horse
	bool getOnHorse()
	{
		return _onHorse;
	}

	// sets on horse
	void setOnHorse(bool b)
	{
		this->_onHorse = b;
	}

	// returns Name of Knight
	string getName()
	{
		return this->_name;
	}

	// Joust Function
	bool Joust()
	{
		// do jousting stuff! 
		// review sequence diagram for how
		if (this->_weapon.wield())
		{
			this->_stamina -= this->_weapon.getStaminaCost();
			return true;
		}
		else
			return false;
	}

	// gets Stamina
	int getStamina()
	{
		return _stamina;
	}

	// if knight is knocked out
	void setExhausted()
	{
		this->_stamina = 0;
	}

	// display stats of Knight
	void displayStats()
	{
		// display knight stats
		// on horse
		if (this->_onHorse)
		{
			//not exhausted and on horse
			if (this->_stamina > 0)
			{
				cout<<this->_name<<" is not exhausted (stamina="<<this->_stamina
					<< ") and is still on the horse.\n";
				cout<<this->_name << " is using: ";
				this->_weapon.displayWeaponInfo();
			}
			// exhausted and on horse
			else
			{
				cout<<this->_name<<" is exhausted (stamina="<< this->_stamina
					<< ") and is still on the horse.\n";
				cout<< this->_name<< " is using: ";
				this->_weapon.displayWeaponInfo();
			}
		}
		// not on horse
		else
		{
			// not on horse and not exhausted
			if (this->_stamina > 0)
			{
				cout<<this->_name<<" is not exhausted (stamina="<<this->_stamina
					<<") and has been knocked off of the horse\n";
				cout<<this->_name<<" is using: ";
				this->_weapon.displayWeaponInfo();
			}
			// not on horse and exhausted
			else
			{
				cout<<this->_name<<" is exhausted (stamina=" << this->_stamina
					<< ") and has been knocked off of the horse\n";
				cout<<this->_name<<" is using: ";
				this->_weapon.displayWeaponInfo();
			}
		}
	}

};

// function inputs Knight's Info and 
// and returns a Knight Object
Knight getKnightInfo()
{
	string knight_name = "";
	string weapon_type = "";
	int stamina = 0;
	int stamina_cost = 0;
	int hit_chance = 0;

	// Get user info and build knight objects
	cout<<"Enter Info for Knight\n";
	cout<<"Knight Name: ";
	cin>>knight_name;
	cout<<"Stamina: ";
	cin>>stamina;

	// info about Knight's Weapon
	cout<<"Enter a weapon for "<<knight_name<<"\n";
	cout<<"Weapon Type: ";
	cin>>weapon_type;
	cout<<"Stamina Cost: ";
	cin>>stamina_cost;
	cout<<"Hit Chance: ";
	cin>>hit_chance;

	Weapon w(stamina_cost, weapon_type, hit_chance);
	Knight k(knight_name,stamina,w);
	return k;
}



int main()
{
	// gets Knight-1's info
	Knight k1 = getKnightInfo();
	cout<< "\n\n";
	// gets Knight-2's info
	Knight k2 = getKnightInfo();

	bool knight2KNockedoff= false;
	bool knight1KnockedOff = false;

	// no has knocked out
	// no is exhausted
	// no one has knocked of the horse
	while (k1.getStamina() > 0 && k2.getStamina() > 0 && k1.getOnHorse() && k2.getOnHorse()
		&& !knight1KnockedOff && !knight2KNockedoff)
	{
		k1.displayStats();
		cout<< "\n";
		k2.displayStats();
		cout<< "\n";
		bool kn1 = k1.Joust();
		bool kn2 = k2.Joust();

		knight1KnockedOff = kn2;
		knight2KNockedoff = kn1;

		k2.setOnHorse(!kn1);
		k1.setOnHorse(!kn2);

		if (!k1.getOnHorse() || knight1KnockedOff)
			k1.setExhausted();
		if (!k2.getOnHorse() || knight2KNockedoff)
			k2.setExhausted();
	}
	cout<< "----------------------------------\n";
	k1.displayStats();
	cout<< "\n";
	k2.displayStats();
	cout<< "\n";

	// displays winner
	if (k1.getStamina() <= 0 && k2.getStamina() > 0)
		cout << k2.getName()<< " has knocked out "<< k1.getName()<< "\n";
	else if (k2.getStamina() <= 0 && k1.getStamina() > 0)
		cout<<k1.getName()<<" has knocked out "<< k2.getName()<<"\n";
	else
		cout<<"Match draw\n";
	return 0;
}