#include "Creature.h"
#include <iostream>
#include <cstdlib>

//constructor for base class with all the necessary variables
Creature::Creature(int aAtkNum, int aAtkSides, int aDefNum, int aDefSides,
                   int aArmor, int aStrength, std::string aCType, std::string aName, int aTeam)
{
    this->atkNum = aAtkNum;
    this->atkSides = aAtkSides;
    this->defNum = aDefNum;
    this->defSides = aDefSides;
    this->armor = aArmor;
    this->strength = aStrength;
    this->cType = aCType;
    this->name = aName;
    this->team = aTeam;
    this->maxStrength = aStrength;

    this->knockouts = 0;
    this->achillesSliced = false;
}

/****************************************************************************************
**Function: rollDice
**Description: Rolls dice depending on number of die and number of sides provided.
**Parameters: Number of die and number of sides to the die.
**Pre-conditions: Rand has been seeded and die number and die sides have been set in
**subclass object.
**Post-conditions: Roll value has been determined and returned.
****************************************************************************************/
int Creature::rollDice(int dieNumber, int dieSides)
{
    int roll = 0;

    for (int i = 0; i < dieNumber; i++)
    {
        roll += ((std::rand() % dieSides) + 1);
    }

    return roll;
}

/****************************************************************************************
**Function: defend
**Description: Inherited function for all creature subclasses that calculates the defense
**roll, takes the attacker's passed attack value and calculates and determines the damage
**done to the character. Updates defenders strength.
**Parameters: Attack value of attacker's attack.
**Pre-conditions: Correct base variables have been set and attack value has been 
**calculated.
**Post-conditions: Subclass creature's strength points have decreased or there has been
**no change.
****************************************************************************************/
void Creature::defend(int attackValue)
{
    int defense = rollDice(defNum, defSides);
    /*std::cout << cType << " defense roll: " << defense
    << " + " << armor << " armor" << std::endl;*/
    //get damage number accounting for defense roll and armor stat
    int damage = attackValue - defense - armor;

    //determine if attack was effective
    if (damage > 0)
    {
        //std::cout << "The attack does " << damage << " damage!" << std::endl;;
        strength -= damage;
    }
    else
    {
        //std::cout << "The attack has no effect!" << std::endl;
    }
}

/****************************************************************************************
**Function: regen
**Description: Inherited function for all creature subclasses that randomly regenerates
**strength for the winning creature only if they have lost strength.
**Parameters: None.
**Pre-conditions: Correct base variables have been set and this creature has won the
**round.
**Post-conditions: Subclass creature's strength points have increased or there has been
**no change.
****************************************************************************************/
void Creature::regen()
{
    int depletedLife = maxStrength - strength;

    //max strength already
    if (depletedLife < 1)
    {
        std::cout << name << " did not regenerate. Already at full strength ("
        << strength << ")." << std::endl;
    }
    else
    {
        //randomly add strength in correct range
        int regen = rollDice(1, depletedLife);
        std::cout << name << " regenerated " << regen << " strength. Before: "
        << strength << " out of " << maxStrength;
        strength += regen;
        std::cout << ". Now: " << strength << " out of " << maxStrength << std::endl;
    }
}

//add to knockout count
void Creature::addKnockout()
{
    knockouts++;
}

//accessor for strength stat
int Creature::getStrength()
{
    return strength;
}

//accessor for creature type
std::string Creature::getCType()
{
    return cType;
}

//accessor for team number
int Creature::getTeam()
{
    return team;
}

//accessor for name provided by user
std::string Creature::getName()
{
    return name;
}

//accessor for knockout count
int Creature::getKnockouts()
{
    return knockouts;
}

/****************************************************************************************
**Function: attack
**Description: Standard attack. Barbarian attack that calculates the defense roll and
**passes that value to a target creature, which then calculates its own defense with that
**number and updates the defender's strength.
**Parameters: Passed by reference target creature(the defender).
**Pre-conditions: Correct base variables have been set.
**Post-conditions: Targeted subclass creature's strength points have decreased or there 
**has been no change.
****************************************************************************************/
void Barbarian::attack(Creature &target)
{
    int attack = rollDice(atkNum, atkSides);
    //std::cout << cType << " attack roll: " << attack << std::endl;
    target.defend(attack);
}

//same as barbarian
void ReptilePeople::attack(Creature &target)
{
    int attack = rollDice(atkNum, atkSides);
    //std::cout << cType << " attack roll: " << attack << std::endl;
    target.defend(attack);
}

//same as barbarian
void BlueMen::attack(Creature &target)
{
    int attack = rollDice(atkNum, atkSides);
    //std::cout << cType << " attack roll: " << attack << std::endl;
    target.defend(attack);
}

/****************************************************************************************
**Function: attack
**Description: Redefined attack function for Goblin. Same as normal attack but enables
**a bool for the goblin that is trigger by an attack roll of 12, the target not being
**another Goblin, and achilles not already triggered.
**Parameters: Passed by reference target creature(the defender).
**Pre-conditions: Correct base variables have been set.
**Post-conditions: Targeted subclass creature's strength points have decreased or there 
**has been no change. Achilles has been activated for current Goblin or not.
****************************************************************************************/
void Goblin::attack(Creature &target)
{
    //standard attack roll
    int attack = rollDice(atkNum, atkSides);
    //std::cout << cType << " attack roll: " << attack << std::endl;

    //not a Goblin, attack roll a 12, and achilles not already sliced
    if (target.getCType() != "Goblin" && attack == 12 && !achillesSliced)
    {
        achillesSliced = true;
        std::cout << "Goblin sliced an achilles!" << std::endl;
    }

    //standard defend call
    target.defend(attack);
}

/****************************************************************************************
**Function: defend
**Description: Redefined defend function for Goblin. Same as normal defend but checks if
**achilles has been triggered and if so reduces the incoming attack value in half.
**Parameters: Attack value of attacker's attack.
**Pre-conditions: Correct base variables have been set, attack value has been 
**calculated, and achilles has been triggered or not.
**Post-conditions: Goblin's strength points have decreased or there has been
**no change and if achilles was triggered the attack value was halved.
****************************************************************************************/
void Goblin::defend(int attackValue)
{
    //incoming attack halved if achilles was sliced
    if(achillesSliced)
    {
        //std::cout << "The attack is less effective because of the mangled achilles!" << std::endl;
        attackValue = (attackValue / 2);
    }

    //standard defend
    int defense = rollDice(defNum, defSides);
    /*std::cout << cType << " defense roll: " << defense
    << " + " << armor << " armor" << std::endl;*/
    int damage = attackValue - defense - armor;

    if (damage > 0)
    {
        //std::cout << "The attack does " << damage << " damage!" << std::endl;
        strength -= damage;
    }
    else
    {
        //std::cout << "The attack has no effect!" << std::endl;
    }
}

//same as barbarian
void Shadow::attack(Creature &target)
{
    int attack = rollDice(atkNum, atkSides);
    //std::cout << cType << " attack roll: " << attack << std::endl;
    target.defend(attack);
}

/****************************************************************************************
**Function: defend
**Description: Redefined defend function for Shadow. Same as normal defend but 50% of the
**time no damage is calculated
**Parameters: Attack value of attacker's attack.
**Pre-conditions: Correct base variables have been set and attack value has been 
**calculated.
**Post-conditions: Either no damage calculation took place because of dodge or Shadow's
**strength points have decreased or there has been no change because defense rolls.
****************************************************************************************/
void Shadow::defend(int attackValue)
{
    //will return 0 or 1, 50% chance
    if((rand() % 2) != 0)
    {
        std::cout << "The Shadow dodged an attack!" << std::endl;
    }
    //if not dodged, standard defend
    else
    {
        int defense = rollDice(defNum, defSides);
        /*std::cout << cType << " defense roll: " << defense
        << " + " << armor << " armor" << std::endl;*/
        int damage = attackValue - defense - armor;

        if (damage > 0)
        {
            //std::cout << "The attack does " << damage << " damage!" << std::endl;
            strength -= damage;
        }
    }
}