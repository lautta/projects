/****************************************************************************************
**Description: In this game, the player is trapped in a dream and must escape. They must
**gather clues, collect items to advance, and avoid pitfalls to reach the exit. In order
**to avoid traps and complete the game in the shortest time the user must: 1) Grab a
**candle to advance through dark rooms 2) Correctly choose the spectacles in the study
**3) Go to the manikin in the concert hall and have them enchanted 4) Go to the gallery
**and touch the painting. There are clues along the way but that is the shortest route.
**
**Input: User inputs simple commands for movement and decision making.
**
**Output: Prints the game text and menu options.
****************************************************************************************/

#include <iostream>
#include "Player.h"
#include "Room.h"

int main()
{
    //create room instances with default descriptions and their available directions
    Room* entryRoom = new Entry("You are in the large foyer of the grandiose castle.",
                                "North, South, East, or West.");

    Room* galleryRoom = new Gallery("You are in a large gallery with many paintings. "
                                    "You are drawn to a massive painting of a "
                                    "pastoral homestead.",
                                    "South.");

    Room* darkHallW1 = new DarkHall("You are in a dark eerie hall with your candle "
                                    "light as your guide.",
                                    "East or West.");

    Room* darkHallW2 = new DarkHall("You are in a dark eerie hall with your candle "
                                    "light as your guide.",
                                    "North or East.");

    Room* studyRoom = new Study("You are in a messy study with all sorts of knick-knacks"
                                " and old books.",
                                "South.");

    Room* darkHallE = new DarkHall("You are in a dark dead end with your candle "
                                   "light as your guide. You notice a small painting "
                                   "of a distinguished gentleman with spectacles on.",
                                   "West.");

    Room* concertRoom = new ConcertHall("You are in a concert hall with a lifeless "
                                        "manikin posed on stage.",
                                        "North or East.");

    Room* candleRoom = new CandleRoom("You are in a storage room surrounded by "
                                      "candles that are strangely all lit.", "West.");

    //create the links for the 8 rooms
    entryRoom->linkRoom(galleryRoom, concertRoom, darkHallE, darkHallW1);
    galleryRoom->linkRoom(NULL, entryRoom, NULL, NULL);
    darkHallW1->linkRoom(NULL, NULL, entryRoom, darkHallW2);
    darkHallW2->linkRoom(studyRoom, NULL, darkHallW1, NULL);
    studyRoom->linkRoom(NULL, darkHallW2, NULL, NULL);
    darkHallE->linkRoom(NULL, NULL, NULL, entryRoom);
    concertRoom->linkRoom(entryRoom, NULL, candleRoom, NULL);
    candleRoom->linkRoom(NULL, NULL, NULL, concertRoom);

    //create the player instance and set the current room and starting room to entry room
    Player* dreamer = new Player();
    dreamer->setPlayerRoom(entryRoom);
    //this is to reset players position
    dreamer->setStartRoom(entryRoom);

    std::cout << "Welcome to the Game of Your Dreams!!!" << std::endl;
    std::cout << std::endl << "************************************" << std::endl;
    std::cout << "Menu Options:" << std::endl;
    std::cout << "1. Play the game" << std::endl;
    std::cout << "2. Get the solution route" << std::endl;
    std::cout << "3. Quit" << std::endl;
    std::cout << "************************************" << std::endl;
    std::cout << "Enter a menu number: ";

    int choice = 0;
    while(!(std::cin >> choice) || choice < 1 || choice > 3)
    {
        std::cout << "Try again and enter a valid option: ";
        std::cin.clear();
        std::cin.ignore(100,'\n');
    }
    std::cin.clear();
    std::cin.ignore(100, '\n');

    //play the game
    if (choice == 1) {
        std::cout << std::endl
                  << "*****************************************************"
                  << std::endl
                  << "You awaken in a strange dark castle. You have no idea how you got "
                     "here but for some reason you feel like you are running out of "
                     "time and you have the urge to escape. None of this feels "
                     "real..." << std::endl << std::endl;

        //tracks time limit
        int ticks = 0;

        do
        {
            //run the player's room's special function until time limit or the player escapes
            dreamer->getPlayerRoom()->special(dreamer);
            ticks++;

            //reminds player that time is getting low
            if (ticks == 40) {
                std::cout << "Your vision is starting to fade... "
                        "You must be running out of time to escape." << std::endl;
            }

        } while (ticks < 60 && (!dreamer->getEscaped()));

        //if time ran out, print the failure text
        if (ticks == 60) {
            std::cout << "Your vision fades to black. It looks like you have run out of time "
                    "and are stuck in the castle forever!!" << std::endl;
        }
    }
    if (choice == 2)
    {
        std::cout << "Grab a candle to advance through dark rooms. " << std::endl
                  << "Correctly choose the spectacles in the study. " << std::endl
                  << "Go to the manikin in the concert hall and have them enchanted. "
                  << std::endl
                  << "Go to the gallery and touch the painting." << std::endl;
    }
    if (choice == 3)
    {
        std::cout << "Exiting program" << std::endl;
    }

    //delete memory
    delete entryRoom;
    delete galleryRoom;
    delete darkHallW1;
    delete darkHallW2;
    delete studyRoom;
    delete darkHallE;
    delete concertRoom;
    delete candleRoom;
    delete dreamer;

    return 0;
}