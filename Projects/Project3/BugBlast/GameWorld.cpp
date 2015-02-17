#include "GameWorld.h"
#include "GameController.h"
#include <string>
#include <cstdlib>
using namespace std;

bool GameWorld::getKey(int& value)
{
	bool result = m_controller->getLastKey(value);

	if (result && (value == 'q' || value == '\x03'))  // CTRL-C
			exit(0);

	return result;
}

void GameWorld::playSound(int soundID)
{
	m_controller->playSound(soundID);
}

void GameWorld::setGameStatText(string text)
{
	m_controller->setGameStatText(text);
}
