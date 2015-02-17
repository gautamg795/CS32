#ifndef GAMEWORLD_H_
#define GAMEWORLD_H_

#include "GameConstants.h"
#include <string>

const int START_PLAYER_LIVES = 3;

class GameController;

class GameWorld
{
public:

	GameWorld()
     : m_lives(START_PLAYER_LIVES), m_score(0), m_level(0), m_controller(NULL)
	{
	}

	virtual ~GameWorld()
	{
	}
	
	virtual int init() = 0;
	virtual int move() = 0;
	virtual void cleanUp() = 0;

	void setGameStatText(std::string text);

	bool getKey(int& value);
	void playSound(int soundID);

	unsigned int getLevel() const
	{
		return m_level;
	}

	unsigned int getLives() const
    {
        return m_lives;
    }
    
	void decLives()
    {
        m_lives--;
    }
    
	void incLives() {
        m_lives++;
    }
    
	unsigned int getScore() const
    {
        return m_score;
    }
	
	void increaseScore(unsigned int howMuch)
	{
		m_score += howMuch;
	}
    
    bool testParamsProvided() const
    {
        //for (int i = 0; i < NUM_TEST_PARAMS; i++)
        //{
        //    if (m_testParams[i] != 0)
        //        return true;
        //}
        return false;
    }

    int getTestParam(int paramID) const
    {
        //return paramID < NUM_TEST_PARAMS ? m_testParams[paramID] : 0;
		return -1;
    }
    
      // The following should be used by only the framework, not the student

	bool isGameOver() const
    {
        return m_lives == 0;
    }

	void advanceToNextLevel() 
	{
		++m_level;
	}
   
	void setController(GameController* controller)
	{
		m_controller = controller;
	}
    
	void setTestParams(int testParams[])
	{
		//for (int i = 0; i < NUM_TEST_PARAMS; i++)
		//	m_testParams[i] = testParams[i];
	}
    
private:
	unsigned int	m_level;
	unsigned int	m_lives;
	unsigned int	m_score;
	GameController* m_controller;
	//int				m_testParams[NUM_TEST_PARAMS];
};

#endif // GAMEWORLD_H_
