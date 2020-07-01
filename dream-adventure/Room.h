#ifndef ROOM_H
#define ROOM_H

#include <string>

//needed forward declaration
class Player;

class Room
{
protected:
    Room* north;
    Room* south;
    Room* east;
    Room* west;
    std::string availableDirs;
    std::string roomDesc;

public:
    Room(){};
    Room(std::string description, std::string directions);
    virtual ~Room(){};

    void linkRoom(Room *n, Room *s, Room *e, Room *w);
    void describeRoom();
    void validate(int &input, int lowest, int highest);
    void changeRooms(Player *aPlayer);
    virtual void special(Player *aPlayer) = 0;
};

//normal room, starting location
class Entry : public Room
{
public:
    Entry(std::string description, std::string directions) : Room(description, directions){}
    virtual void special(Player *aPlayer);
};

//need candle to proceed
class DarkHall : public Room
{
public:
    DarkHall(std::string description, std::string directions) : Room(description, directions){}
    virtual void special(Player *aPlayer);
};

//pick up candle
class CandleRoom : public Room
{
public:
    CandleRoom(std::string description, std::string directions) : Room(description, directions){}
    virtual void special(Player *aPlayer);
};

//glasses and monocle
class Study : public Room
{
public:
    Study(std::string description, std::string directions) : Room(description, directions){}
    virtual void special(Player *aPlayer);
};

//weird spirit
class ConcertHall : public Room
{
public:
    ConcertHall(std::string description, std::string directions) : Room(description, directions){}
    virtual void special(Player *aPlayer);
};

//exit
class Gallery : public Room
{
public:
    Gallery(std::string description, std::string directions) : Room(description, directions){}
    virtual void special(Player *aPlayer);
};

#endif // ROOM_H

