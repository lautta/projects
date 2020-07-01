#ifndef CREATURE_H
#define CREATURE_H

#include <string>

class Creature
{
protected:
    //inherited values for all creatures and subclasses
    int atkNum;
    int atkSides;
    int defNum;
    int defSides;
    int armor;
    int strength;
    int maxStrength;
    std::string cType;
    std::string name;
    int knockouts;
    int team;
    bool achillesSliced;

public:
    Creature (){};
    //constructor for inherited values
    Creature (int aAtkNum, int aAtkSides, int aDefNum, int aDefSides,
              int aArmor, int aStrength, std::string aCType, std::string aName, int aTeam);
    virtual ~Creature(){};

    virtual void attack (Creature &target) = 0;
    virtual void defend (int attackValue);
    int rollDice(int dieNumber, int dieSides);
    void regen();
    std::string getCType();
    int getStrength();
    int getTeam();
    std::string getName();
    int getKnockouts();
    void addKnockout();

};

class Barbarian : public Creature
{
public:
    //barbarian constructor with initializer list
    Barbarian(std::string aName, int aTeam) :
            Creature(2, 6, 2, 6, 0, 12, "Barbarian", aName, aTeam){}

    virtual void attack (Creature &target);
};

class ReptilePeople : public Creature
{
public:
    //reptile constructor with initializer list
    ReptilePeople(std::string aName, int aTeam) :
            Creature(3, 6, 1, 6, 7, 18, "Reptile People", aName, aTeam){}

    virtual void attack (Creature &target);
};

class BlueMen : public Creature
{
public:
    //blue men constructor with initializer list
    BlueMen(std::string aName, int aTeam) :
            Creature(2, 10, 3, 6, 3, 12, "Blue Men", aName, aTeam){}

    virtual void attack (Creature &target);
};

class Goblin : public Creature
{
public:
    //goblin constructor with initializer list
    Goblin(std::string aName, int aTeam) :
            Creature(2, 6, 1, 6, 3, 8, "Goblin", aName, aTeam){}

    //redefined attack and defend functions
    virtual void attack (Creature &target);
    virtual void defend (int attackValue);
};

class Shadow : public Creature
{
public:
    //shadow constructor with initializer list
    Shadow(std::string aName, int aTeam) :
            Creature(2, 10, 1, 6, 0, 12, "Shadow", aName, aTeam){}

    //redefined attack and defend functions
    virtual void attack (Creature &target);
    virtual void defend (int attackValue);
};


#endif //CREATURE_H
