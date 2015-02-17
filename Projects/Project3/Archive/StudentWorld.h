#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include <list>
#include "Actor.h"
#include <string>
// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
    //---------------------------------------------------------
    // Housekeeping Functions
    //---------------------------------------------------------
    StudentWorld() { m_level = nullptr; m_actors.clear(); m_player = nullptr; }
	~StudentWorld()
    {
        for (auto it = m_actors.begin(); it != m_actors.end(); it++)
            delete *it;
        delete m_level;
        delete m_player;
    }
    
    //---------------------------------------------------------
    // Gameplay Functions
    //---------------------------------------------------------
    virtual int init();
	virtual int move();
	virtual void cleanUp();
    
    //---------------------------------------------------------
    // Actor Management Functions
    //---------------------------------------------------------
    void addActor(Actor* ap) { m_actors.push_back(ap); }
    int whatObjectIsThis(Actor* ap) const; // Returns ID of the object pointed to by ap
    void removeDeadActors();
    void killPlayer() { m_player->setDead(); }
    void applyBugspray(int x, int y);
    
    //---------------------------------------------------------
    // Gamestate Management Functions
    //---------------------------------------------------------
    std::string formatText();
    void sprayerSprayed() { m_player->sprayerDied(); }
    void zumiDied() { m_zumiKilled++; }

    //---------------------------------------------------------
    // Information Getters
    //---------------------------------------------------------
    Actor* spotContains(int ID, int x, int y) const;
    bool didPlayerWin() const; // Checks if Player is standing on Exit (preconditions met)
    bool killedAll() const { return m_zumiKilled == totalZumi; } // Returns if all Zumis killed
    bool shouldPlayerDie() const; // Returns if Player location and colocation of any Actors should kill the player
    int whichGoodie() const; // If a Goodie is to be dropped, returns the ID of which one
    int getPlayerX() const { return m_player->getX(); }
    int getPlayerY() const { return m_player->getY(); }
    
    //---------------------------------------------------------
    // Level Info Getters
    //---------------------------------------------------------
    unsigned int getProbOfGoodieOverall() const { return m_level->getOptionValue(optionProbOfGoodieOverall); }
    unsigned int getProbOfExtraLifeGoodie() const { return m_level->getOptionValue(optionProbOfExtraLifeGoodie); }
    unsigned int getProbOfWalkThruGoodie() const { return m_level->getOptionValue(optionProbOfWalkThruGoodie); }
    unsigned int getProbOfMoreSprayersGoodie() const { return m_level->getOptionValue(optionProbOfMoreSprayersGoodie); }
    unsigned int getTicksPerSimpleZumiMove() const { return m_level->getOptionValue(optionTicksPerSimpleZumiMove)-1; } // -1 because we move when this many ticks have passed
    unsigned int getTicksPerComplexZumiMove() const { return m_level->getOptionValue(optionTicksPerComplexZumiMove)-1; }
    unsigned int getGoodieLifetimeInTicks() const { return m_level->getOptionValue(optionGoodieLifetimeInTicks); }
    unsigned int getComplexZumiSearchDistance() const { return m_level->getOptionValue(optionComplexZumiSearchDistance); }
    unsigned int getWalkThroughLifetimeTicks() const { return m_level->getOptionValue(optionWalkThruLifetimeTicks); }
    unsigned int getBoostedSprayerLifetimeTicks() const { return m_level->getOptionValue(optionBoostedSprayerLifetimeTicks); }
    unsigned int getMaxBoostedSprayers() const { return m_level->getOptionValue(optionMaxBoostedSprayers); }
    
private:
    std::list<Actor*> m_actors;
    Player* m_player;
    Level* m_level;
    int m_currentBonus;
    int m_zumiKilled = 0;
    int totalZumi = 0;
};

#endif // STUDENTWORLD_H_
