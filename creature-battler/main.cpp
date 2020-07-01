/****************************************************************************************
**Description: This program allows the user to select two teams made of 1 of 5 fighters
**and the two teams then do battle. The user chooses the name and character type and
**then the results of each round are printed as well as the entire tournament. At the
**end, the top 3 finishers are displayed.
**
**Input: User enters team size, fighter names and creature type.
**
**Output: Prints the results of each combat round, the results of the whole tournament,
**and the top 3 fighters.
****************************************************************************************/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Creature.h"
#include "Stack.h"
#include "Queue.h"

void rumble(Queue &team1, Queue &team2, Stack &losers);
void selectTeam(Queue &team, int teamNum, int count);
void topThree(Stack &losers);
void autoSelect(Queue &team, int teamNum, int choice);

int main()
{
    std::srand(std::time(0)); //the only time rand is seeded

    Queue team1;
    Queue team2;
    Stack losers;

    int fighterNum = 0;
    char cont;

    std::cout << "THIS IS KUMITE!!!!!!!!!!!!!!" << std::endl;
    std::cout << "How many fighters per team? ";
    while(!(std::cin >> fighterNum) || fighterNum < 1)
    {
        std::cout << "Try again and enter a valid positive number: ";
        std::cin.clear();
        std::cin.ignore(100,'\n');
    }
    std::cin.clear();
    std::cin.ignore(100, '\n');
    std::cout << std::endl;


    std::cout << "**TEAM 1: SELECT " << fighterNum << " FIGHTER(S)!!!**" << std::endl;

    for (int i = 0; i < fighterNum; i++)
    {
        selectTeam(team1, 1, i);
    }

    std::cout << "**TEAM 2: SELECT " << fighterNum << " FIGHTER(S)!!!**" << std::endl;

    for (int i = 0; i < fighterNum; i++)
    {
        selectTeam(team2, 2, i);
    }

    std::cout << "LET'S RUMBLE!!!!!!!!!!!!!!!" << std::endl;

    rumble(team1, team2, losers);

    topThree(losers);

    std::cout << std::endl << "Do you want to see the rest of the lineups (y/n)? ";
    std::cin >> cont;
    std::cin.clear();
    std::cin.ignore(100, '\n');

    if (cont == 'y')
    {
        //delete the individual creature data and then remove the stack node
        while(!losers.isEmpty())
        {
            std::cout << "**" << losers.getTop()->getName() << " the "
            << losers.getTop()->getCType() << " on TEAM " << losers.getTop()->getTeam()
            << " with " << losers.getTop()->getKnockouts() << " KOs." << std::endl;

            //delete creature and creature pointer
            delete losers.getTop();
            losers.removeTop();
        }
    }
    else
    {
        while(!losers.isEmpty())
        {
            //delete creature and creature pointer
            delete losers.getTop();
            losers.removeTop();
        }
    }

    return 0;
}

/****************************************************************************************
**Function: selectTeam
**Description: Creates the fighters for one roster and add them to the appropriate queue.
**Parameters: Team roster queue passed by reference to be updated with new creatures,
**team number to instantiate new creatures with and a count for print purposes.
**Pre-conditions: Team roster queue has been created.
**Post-conditions: Team roster queue has been correctly updated.
****************************************************************************************/
void selectTeam(Queue &team, int teamNum, int count)
{
    int choice;
    std::string fighterName;

    std::cout << "TEAM " << teamNum << " FIGHTER #" << count + 1 << ": " << std::endl
    << "Name your fighter: ";

    getline(std::cin, fighterName);

    std::cout << "1. Goblin" << std::endl
    << "2. Barbarian" << std::endl
    << "3. Reptile People" << std::endl
    << "4. Blue Men" << std::endl
    << "5. Shadow" << std::endl;
    std::cout << "Select a creature type (1-5): ";
    while(!(std::cin >> choice) || choice < 1 || choice > 5)
    {
        std::cout << "Try again and enter a valid menu option: ";
        std::cin.clear();
        std::cin.ignore(100,'\n');
    }
    std::cin.clear();
    std::cin.ignore(100, '\n');

    //instantiate creatures and add to roster queue
    if (choice == 1)
    {
        Creature *fighter = new Goblin(fighterName, teamNum);
        team.addBack(fighter);
    }
    else if (choice == 2)
    {
        Creature *fighter = new Barbarian(fighterName, teamNum);
        team.addBack(fighter);
    }
    else if (choice == 3)
    {
        Creature *fighter = new ReptilePeople(fighterName, teamNum);
        team.addBack(fighter);
    }
    else if (choice == 4)
    {
        Creature *fighter = new BlueMen(fighterName, teamNum);
        team.addBack(fighter);
    }
    else if (choice == 5)
    {
        Creature *fighter = new Shadow(fighterName, teamNum);
        team.addBack(fighter);
    }
    std::cout << std::endl;
}
/****************************************************************************************
**Function: rumble
**Description: Applies full battle logic between two rosters and determines the winner of
**each round and then then the tournament itself. Updates rosters and losers accordingly.
**Parameters: References to the roster queues for team 1 and team 2 and the defeated
**fighter stack.
**Pre-conditions: Rosters have been filled with correctly instantiated pointers to
**creatures.
**Post-conditions: Prints round information and tournament win information and rosters
**and losers are updated.
****************************************************************************************/
void rumble(Queue &team1, Queue &team2, Stack &losers)
{
    //track both teams wins and the number of rounds
    int t1Wins = 0;
    int t2Wins = 0;
    int round = 1;
    //create pointers fighter objects
    Creature *fighter1;
    Creature *fighter2;

    //fight until one roster is empty
    while((!team1.isEmpty()) && (!team2.isEmpty()))
    {
        //assign fronts of rosters to the fighter pointers
        fighter1 = team1.getFront();
        fighter2 = team2.getFront();
        //remove the front of the roster, necessary here for while loop logic.
        team1.removeFront();
        team2.removeFront();

        std::cout << std::endl << "ROUND " << round << ":" << std::endl
        << "**" << fighter1->getName() << " the " << fighter1->getCType()
        << " (TEAM " << fighter1->getTeam() << ")** VERSUS **"
        << fighter2->getName() << " the " << fighter2->getCType()
        << " (TEAM " << fighter2->getTeam() << ")**" << std::endl;

        //while one creature is alive
        while ((fighter1->getStrength() > 0) && (fighter2->getStrength() > 0))
        {
            //fighter 1 attacks fighter 2 with fighter 2 defense called internally
            fighter1->attack(*fighter2);

            //if fighter 2 is alive, attack fighter 1
            if (fighter2->getStrength() > 0)
            {
                fighter2->attack(*fighter1);
            }
            //fighter 1 won
            else
            {
                //add to win, round, and creature KO totals
                round++;
                t1Wins++;
                fighter1->addKnockout();

                std::cout << fighter1->getName() << " the " << fighter1->getCType()
                << " (TEAM " << fighter1->getTeam() << ") won the round! " << std::endl
                << fighter1->getName() << " KO COUNT: " << fighter1->getKnockouts() << std::endl
                << "TEAM 1 WINS: " << t1Wins << std::endl
                << "TEAM 2 WINS: " << t2Wins << std::endl;

                //let the winner regen
                fighter1->regen();
                //re-add winning fighter pointer to the back of team1 roster
                team1.addBack(fighter1);
                //add the loser pointer to loser stack
                losers.addTop(fighter2);
            }

            //fighter 2 won
            if (fighter1->getStrength() <= 0)
            {
                round++;
                t2Wins++;
                fighter2->addKnockout();

                std::cout << fighter2->getName() << " the " << fighter2->getCType()
                << " (TEAM " << fighter2->getTeam() << ") won the round! " << std::endl
                << fighter2->getName() << " KO COUNT: " << fighter2->getKnockouts() << std::endl
                << "TEAM 1 WINS: " << t1Wins << std::endl
                << "TEAM 2 WINS: " << t2Wins << std::endl;

                fighter2->regen();
                team2.addBack(fighter2);
                losers.addTop(fighter1);
            }
        }
    }

    //compare raw wins of the two teams and print results
    std::cout << std::endl << "FINAL RESULTS!!!!!!!" << std::endl;
    if (t1Wins > t2Wins)
    {
        std::cout << "TEAM 1 wins the tournament with " << t1Wins << " wins!!!" << std::endl;
    }
    else if (t2Wins > t1Wins)
    {
        std::cout << "TEAM 2 wins the tournament with " << t2Wins << " wins!!!" << std::endl;
    }
    else
    {
        std::cout << "There was a tie... Both teams had " << t1Wins << " wins." << std::endl;
    }

    //tourney is over, add remaining creature pointers to loser stack and remove from team
    while(!team1.isEmpty())
    {
        losers.addTop(team1.getFront());
        team1.removeFront();
    }
    while(!team2.isEmpty())
    {
        losers.addTop(team2.getFront());
        team2.removeFront();
    }
}

/****************************************************************************************
**Function: topThree
**Description: Gets the top three fighters from the loser pile and prints them in order
**of kills.
**Parameters: Reference to losers stack.
**Pre-conditions: Tournament has concluded and the losers stack has been filled with
**all possible fighters.
**Post-conditions: Top three have been printed and removed from stack and their memory
**has been freed.
****************************************************************************************/
void topThree(Stack &losers)
{
    std::cout << std::endl << "FINAL RANKINGS!!!!!!!" << std::endl;

    //get only the first two in case someone picks a 1v1 battle
    for (int i = 0; i < 2; i++)
    {
        std::cout << (i + 1) << ": " << losers.getTop()->getName() << " the "
        << losers.getTop()->getCType() << " on TEAM " << losers.getTop()->getTeam()
        << " with " << losers.getTop()->getKnockouts() << " KOs." << std::endl;

        //delete creature and creature pointer
        delete losers.getTop();
        losers.removeTop();
    }

    //otherwise print the third ranked fighter
    if (!losers.isEmpty())
    {
        std::cout << "3: " << losers.getTop()->getName() << " the "
        << losers.getTop()->getCType() << " on TEAM " << losers.getTop()->getTeam()
        << " with " << losers.getTop()->getKnockouts() << " KOs." << std::endl;

        delete losers.getTop();
        losers.removeTop();
    }
}
