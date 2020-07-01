#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <vector>

//needed forward declaration
class Room;

class Player
{
public:
    Player();
    //setters and getters
    void setPlayerRoom(Room *aRoom);
    void setStartRoom(Room *aRoom);
    void setCandle(bool candle);
    void setMonocle(bool monocle);
    void setSpectacles(bool spectacles);
    void setEnchanted(bool enchanted);
    void setEscaped(bool escaped);
    bool getCandle();
    bool getMonocle();
    bool getSpectacles();
    bool getEnchanted();
    bool getEscaped();
    Room* getPlayerRoom();
    Room* getStartRoom();
    //possessions manipulation
    void addItem(std::string aItem);
    void removeItem(std::string aItem);

protected:
    std::vector<std::string> possessions;
    bool hasCandle;
    bool hasMonocle;
    bool hasSpectacles;
    bool isEnchanted;
    bool hasEscaped;
    Room *playerRoom;
    Room *startRoom;
};

#endif // PLAYER_H

