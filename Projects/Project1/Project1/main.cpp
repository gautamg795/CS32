// main.cpp
#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////

int main()
{
    // Initialize the random number generator
    srand(static_cast<unsigned int>(time(0)));
    // Create a game
    // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(16,20, 50);
    // Play the game
    g.play();
    
}