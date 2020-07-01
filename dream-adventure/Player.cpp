#include <iostream>
#include <algorithm>
#include "Player.h"
#include "Room.h"

//set all bools to false
Player::Player()
{
    hasCandle = false;
    hasMonocle = false;
    hasSpectacles = false;
    isEnchanted = false;
    hasEscaped = false;
}

//getters and setters required for mostly room class access but some main access
void Player::setPlayerRoom(Room *aRoom)
{
    playerRoom = aRoom;
}

void Player::setStartRoom(Room*aRoom)
{
    startRoom = aRoom;
}

void Player::setCandle(bool candle)
{
    hasCandle = candle;
}

void Player::setMonocle(bool monocle)
{
    hasMonocle = monocle;
}

void Player::setSpectacles(bool spectacles)
{
    hasSpectacles = spectacles;
}

void Player::setEnchanted(bool enchanted)
{
    isEnchanted = enchanted;
}

void Player::setEscaped(bool escaped)
{
    hasEscaped = escaped;
}

Room* Player::getPlayerRoom()
{
    return playerRoom;
}

Room* Player::getStartRoom()
{
    return startRoom;
}

bool Player::getCandle()
{
    return hasCandle;
}

bool Player::getMonocle()
{
    return hasMonocle;
}

bool Player::getSpectacles()
{
    return hasSpectacles;
}

bool Player::getEnchanted()
{
    return isEnchanted;
}

bool Player::getEscaped()
{
    return hasEscaped;
}

/****************************************************************************************
**Function: addItem
**Description: adds item to player's possessions vector if the size is less than 2.
**Parameters: String of item name to be added.
**Pre-conditions: Player has been correctly instantiated.
**Post-conditions: Item has been added or possessions is full.
****************************************************************************************/
void Player::addItem(std::string aItem)
{
    if (possessions.size() < 2)
    {
        possessions.push_back(aItem);
    }
    else
    {
        std::cout << "You can't hold more." << std::endl;
    }
}

/****************************************************************************************
**Function: removeItem
**Description: Removes item from player's possessions vector if it exists.
**Parameters: String of item name to be removed.
**Pre-conditions: Player has been correctly instantiated.
**Post-conditions: Item has been removed or it doesn't exist in possessions.
****************************************************************************************/
void Player::removeItem(std::string aItem)
{
    //search through vector to find matching string
    std::vector<std::string>::iterator result = std::find(possessions.begin(),
                                                          possessions.end(), aItem);

    //find reached the end
    if (result == possessions.end())
    {
        return;
    }
    //the string was found, so delete it
    else
    {
        possessions.erase(result);
    }
}

