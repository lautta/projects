#include <iostream>
#include <cstdlib>
#include "Room.h"
#include "Player.h"

//constructor for base class that takes description and available directions to print
Room::Room(std::string description, std::string directions)
{
    this->roomDesc = description;
    this->availableDirs = directions;
    this->north = NULL;
    this->south = NULL;
    this->east = NULL;
    this->west = NULL;
}

/****************************************************************************************
**Function: linkRoom
**Description: Inherited by subclasses. Takes room pointers and creates a link for the
**four possible directions.
**Parameters: Pointers for north, south, east, and west.
**Pre-conditions: Rooms have been instantiated.
**Post-conditions: Direction pointers are correctly assigned for the room.
****************************************************************************************/
void Room::linkRoom(Room *n, Room *s, Room *e, Room *w)
{
    north = n;
    south = s;
    east = e;
    west = w;
}

/****************************************************************************************
**Function: describeRoom
**Description: Inherited by subclasses. Prints room description and available directions
**from player's position. Will be used in special methods.
**Parameters: None.
**Pre-conditions: Rooms have been instantiated.
**Post-conditions: Description and directions are displayed.
****************************************************************************************/
void Room::describeRoom()
{
    std::cout << "*****************************************************" << std::endl
              << roomDesc << std::endl << "From here, you can move " << availableDirs
              << std::endl << std::endl;
}

/****************************************************************************************
**Function: validate
**Description: Inherited by subclasses. Validates the int room commands with a provided
**range.
**Parameters: Int input variable, the lowest range and the highest range.
**Pre-conditions: None.
**Post-conditions: Int input variable has been assigned a valid int.
****************************************************************************************/
void Room::validate(int &input, int lowest, int highest)
{
    while(!(std::cin >> input) || input < lowest || input > highest)
    {
        std::cout << "Try again and enter a valid option: ";
        std::cin.clear();
        std::cin.ignore(100,'\n');
    }
    std::cin.clear();
    std::cin.ignore(100, '\n');
}

/****************************************************************************************
**Function: changeRooms
**Description: Inherited by subclasses. Displays the command for the player's movement.
**Checks room links to see if room is valid and if it is set's player location to the
**next room. These will be placed in the special methods.
**Parameters: Pointer to player object to change position.
**Pre-conditions: Rooms have been correctly instantiated and the player is in a room.
**Post-conditions: Player's position has been updated.
****************************************************************************************/
void Room::changeRooms(Player *aPlayer)
{
    Room* nextRoom;

    do
    {
        std::cout << "Which direction do you want to move? "
                     "(1=North 2=South 3=East 4=West) ";

        int choice = 0;
        validate(choice, 1, 4);

        //set nextRoom to the requested room link
        if (choice == 1)
        {
            nextRoom = north;
        }
        else if (choice == 2)
        {
            nextRoom = south;
        }
        else if (choice == 3)
        {
            nextRoom = east;
        }
        else if (choice == 4)
        {
            nextRoom = west;
        }
        else
        {
            nextRoom = NULL;
        }

        //check if the room link is valid and set player to next room if so
        if (nextRoom)
        {
            aPlayer->setPlayerRoom(nextRoom);
        }
        else
        {
            std::cout << "There is no exit that way." << std::endl;
        }
    }
    while (!nextRoom);

    std::cout << std::endl;
}

/****************************************************************************************
**Function: Entry special
**Description: Defined for Entry subclass. Basic room that gives a description and then
**runs the player movement command options.
**Parameters: Pointer to player object to change position and update player members.
**Pre-conditions: Room and player have been correctly instantiated.
**Post-conditions: Player's position has been updated.
****************************************************************************************/
void Entry::special(Player *aPlayer)
{
    describeRoom();
    changeRooms(aPlayer);
}

/****************************************************************************************
**Function: DarkHall special
**Description: Defined for DarkHall subclass. If player is in this room with a candle the
**room gives a description and then runs the player movement command options. If player
**doesn't have candle, the player can't advance and his position is sent back to start.
**Parameters: Pointer to player object to change position and update player members.
**Pre-conditions: Room and player have been correctly instantiated.
**Post-conditions: Player's position has been updated.
****************************************************************************************/
void DarkHall::special(Player *aPlayer)
{
    if (aPlayer->getCandle())
    {
        describeRoom();
        changeRooms(aPlayer);
    }
    //player doesn't have candle and is sent back to start room
    else
    {
        std::cout << "You attempt to enter a dark and dank hallway. " << std::endl
                  << "You frantically scramble around in the dark without any sense of "
                     "direction. Perhaps you need some light." << std::endl
                  << "Eventually you arrive at a familiar place..." << std::endl
                  << std::endl;
        aPlayer->setPlayerRoom(aPlayer->getStartRoom());
    }
}

/****************************************************************************************
**Function: CandleRoom special
**Description: Defined for CandleRoom subclass. The player is given the option to take
**a candle unless he already has one. Then the player is prompted for movement.
**Parameters: Pointer to player object to change position and update player members.
**Pre-conditions: Room and player have been correctly instantiated.
**Post-conditions: Player's position has been updated and player possessions and bools
**may have been updated.
****************************************************************************************/
void CandleRoom::special(Player *aPlayer)
{
    describeRoom();

    if (aPlayer->getCandle())
    {
        std::cout << "You are already holding a candle." << std::endl << std::endl;
        changeRooms(aPlayer);
    }
    else
    {
        std::cout << "Would you like to grab a candle? (1=Yes 2=No) ";

        int choice = 0;
        validate(choice, 1, 2);

        if (choice == 1)
        {
            //add to player possessions and update the candle bool
            std::cout << "You grab one of the many candles." << std::endl << std::endl;
            aPlayer->addItem("a candle");
            aPlayer->setCandle(true);
        }
        else
        {
            std::cout << "You decide not to grab a candle." << std::endl << std::endl;
            aPlayer->setCandle(false);
        }

        changeRooms(aPlayer);
    }
}

/****************************************************************************************
**Function: Study special
**Description: Defined for Study subclass. The player is given the option to take a
**monocle or spectacles but cannot have both at once. If the player's specs are
**enchanted they cannot exchange.
**Parameters: Pointer to player object to change position and update player members.
**Pre-conditions: Room and player have been correctly instantiated.
**Post-conditions: Player's position has been updated and player possessions and bools
**may have been updated.
****************************************************************************************/
void Study::special(Player *aPlayer)
{
    describeRoom();

    //player has the spectacles and has been enchanted
    if (aPlayer->getEnchanted())
    {
        std::cout << "You are currently wearing the old spectacles." << std::endl
                  << "The strange aura around you prevents you from removing them."
                  << std::endl << std::endl;
        changeRooms(aPlayer);
        return;
    }

    //player is wearing the monocle
    if (aPlayer->getMonocle())
    {
        std::cout << "You are currently wearing a monocle." << std::endl
                  << "Would you like to exchange them for the old spectacles? (1=Yes 2=No) ";

        int choice = 0;
        validate(choice, 1, 2);

        if (choice == 1)
        {
            std::cout << "You exchanged the monocle with the spectacles." << std::endl
                      << std::endl;
            aPlayer->removeItem("the monocle");
            aPlayer->addItem("the spectacles");
            aPlayer->setMonocle(false);
            aPlayer->setSpectacles(true);
        }
        else
        {
            std::cout << "You kept the monocle on." << std::endl << std::endl;
        }

        changeRooms(aPlayer);
        return;
    }

    //player is wearing the spectacles
    if (aPlayer->getSpectacles())
    {
        std::cout << "You are currently wearing the old spectacles." << std::endl
                  << "Would you like to exchange them for the monocle? (1=Yes 2=No) ";

        int choice = 0;
        validate(choice, 1, 2);

        if (choice == 1)
        {
            std::cout << "You exchanged the monocle with the spectacles." << std::endl
                      << std::endl;
            aPlayer->removeItem("the spectacles");
            aPlayer->addItem("the monocle");
            aPlayer->setMonocle(true);
            aPlayer->setSpectacles(false);
        }
        else
        {
            std::cout << "You kept the spectacles on." << std::endl << std::endl;
        }

        changeRooms(aPlayer);
        return;
    }

    //if the player is not currently wearing either
    std::cout << "You are drawn to two objects on the desk, a well-made and "
                 "distinguished monocle and some rusty old spectacles." << std::endl
              << "Will you wear one? (1=Monocle 2=Spectacles 3=Neither)";

    int choice = 0;
    validate(choice, 1, 3);

    //add the appropriate item and update bools, or don't do either
    if (choice == 1)
    {
        std::cout << "You put on the monocle. Looking good!" << std::endl << std::endl;
        aPlayer->addItem("the monocle");
        aPlayer->setMonocle(true);
        aPlayer->setSpectacles(false);
    }
    if (choice == 2)
    {
        std::cout << "You put on the spectacles. You feel strange..."
                  << std::endl << std::endl;
        aPlayer->addItem("the spectacles");
        aPlayer->setMonocle(false);
        aPlayer->setSpectacles(true);
    }
    if (choice == 3)
    {
        std::cout << "You decide not to put on either. That may be for the best."
                  << std::endl << std::endl;
        aPlayer->setMonocle(false);
        aPlayer->setSpectacles(false);
    }

    changeRooms(aPlayer);
}

/****************************************************************************************
**Function: Gallery special
**Description: Defined for Gallery subclass. If the player is enchanted they are given
**the option to touch the painting, which is the escape. If the player is not enchanted
**the room acts like a basic room and the player can move.
**Parameters: Pointer to player object to change position and update player members.
**Pre-conditions: Room and player have been correctly instantiated.
**Post-conditions: Player's position has been updated and player possessions and bools
**may have been updated and the game may have ended.
****************************************************************************************/
void Gallery::special(Player *aPlayer)
{
    describeRoom();

    //player is enchanted and can end the game
    if (aPlayer->getEnchanted())
    {
        std::cout << "With the spectacles on and the strange aura, you look towards the "
                     "familiar painting of the homestead once more. " << std::endl
                  << "This time you see what appears to be yourself sleeping "
                     "in your own bed! " << std::endl;

        std::cout << "Will you touch the painting? (1=Yes 2=No) ";

        int choice = 0;
        validate(choice, 1, 2);

        if (choice == 1)
        {
            std::cout << "You touch the painting and a light flashes. "
                         "It seems you have escaped from that odd castle!" << std::endl
                      << "You awaken in your bed however you have no recollection of "
                         "what has just transpired..." << std::endl << std::endl;
            aPlayer->setEscaped(true);
        }
        else
        {
            std::cout << "You turn away from the painting." << std::endl << std::endl;
            changeRooms(aPlayer);
        }
    }
    //player is not enchanted, move on
    else
    {
        changeRooms(aPlayer);
    }
}

/****************************************************************************************
**Function: ConcertHall special
**Description: Defined for ConcertHall subclass. If the player has specs, he becomes
**enchanted and can finish the game. If the player has monocle, he is thrown to the
**start room and his possessions and bools are cleared. If he has neither or is already
**enchanted the room operates as a normal room.
**Parameters: Pointer to player object to change position and update player members.
**Pre-conditions: Room and player have been correctly instantiated.
**Post-conditions: Player's position has been updated and player possessions and bools
**may have been updated and the game may have ended.
****************************************************************************************/
void ConcertHall::special(Player *aPlayer)
{
    //player is already enchanted, display unique flavor text and player moves as normal
    if (aPlayer->getEnchanted())
    {
        std::cout << "You are in the concert hall." << std::endl
                  << "From here, you can move North or East." << std::endl << std::endl
                  << "The strange figure is still there. He is obviously growing annoyed "
                     "with you and says:" << std::endl
                  << " \"Well, what are you waiting for... begone!\""
                  << std::endl << std::endl;
        changeRooms(aPlayer);
        return;
    }

    //player has specs and they now become enchanted, he can now finish the game
    if (aPlayer->getSpectacles())
    {
        std::cout << "You are in the concert hall." << std::endl
                  << "From here, you can move North or East." << std::endl << std::endl
                  << "With the spectacles on, you see that the manikin is now a full "
                     "spectral figure! " << std::endl
                  << "He moves around the room and says:" << std::endl
                  << " \"Well done... you have returned my favorite spectacles to me.\""
                  << std::endl
                  << "The spectacles suddenly appear on his face but are still on yours "
                     "as well. You feel a strange sensation in your eyes." << std::endl
                  << "The figure says:" << std::endl
                  << "\"You are now free to leave.\"" << std::endl << std::endl;
        aPlayer->setEnchanted(true);
        changeRooms(aPlayer);
        return;
    }

    //player has monocle and is stripped of his possessions and returned to the start
    if (aPlayer->getMonocle())
    {
        std::cout << "You are in the concert hall." << std::endl
                  << "From here, you can move North or East." << std::endl << std::endl
                  << "With the monocle on, you see that the manikin is now a motionless "
                     "spectral figure! " << std::endl
                  << "He yells without opening his mouth:" << std::endl
                  << " \"I do not wish to see that blasted monocle ever again! "
                     "You have annoyed me greatly!\"" << std::endl
                  << "There is a large flash and your vision fades. " << std::endl
                  << "You awaken in a familiar place and realize you are no longer "
                     "carrying anything..." << std::endl << std::endl;
        aPlayer->setCandle(false);
        aPlayer->setMonocle(false);
        aPlayer->setSpectacles(false);
        aPlayer->setEnchanted(false);
        aPlayer->removeItem("a candle");
        aPlayer->removeItem("the monocle");
        aPlayer->removeItem("the spectacles");
        aPlayer->setPlayerRoom(aPlayer->getStartRoom());
        return;
    }

    //player has nothing, use default description and allow player to change rooms
    describeRoom();
    changeRooms(aPlayer);
}
