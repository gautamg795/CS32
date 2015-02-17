#include "StudentWorld.h"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
GameWorld* createStudentWorld()
{
	return new StudentWorld();
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

//---------------------------------------------------------
// Gameplay Functions
//---------------------------------------------------------

int StudentWorld::init()
{
    m_level = new Level;
    m_zumiKilled = 0;
    totalZumi = 0;
    std::ostringstream oss;
    oss.fill('0');
    oss << "level" << std::setw(2) << getLevel() << ".dat";
    Level::LoadResult result = m_level->loadLevel(oss.str());
    if (result == Level::load_fail_bad_format)
        return GWSTATUS_LEVEL_ERROR;
    
    else if (result == Level::load_fail_file_not_found)
    {
        if (getLevel() == 0)
            return GWSTATUS_NO_FIRST_LEVEL;
        else
            return GWSTATUS_PLAYER_WON;
    }
    m_currentBonus = m_level->getOptionValue(optionLevelBonus);
    m_actors.clear(); // Ensure that m_actors is empty before filling
    for (int x = 0; x < VIEW_WIDTH; x++)
        for (int y = 0; y < VIEW_HEIGHT; y++)
        {
            Level::MazeEntry me = m_level->getContentsOf(x, y);
            
            switch (me)
            {
                case Level::simple_zumi:
                    m_actors.push_back(new sZumi(x, y, this));
                    totalZumi++;
                    break;
                case Level::complex_zumi:
                    m_actors.push_back(new cZumi(x, y, this));
                    totalZumi++;
                    break;
                case Level::player:
                    m_player = new Player(x, y, this);
                    break;
                case Level::perma_brick:
                    m_actors.push_back(new pBrick(x, y, this));
                    break;
                case Level::destroyable_brick:
                    m_actors.push_back(new dBrick(x, y, this));
                    break;
                case Level::exit:
                    m_actors.push_back(new class exit(x, y, this));
                default:
                    break;
                    
            }
            
        }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText(formatText()); // Update the Game Status Line
    
    for (auto it = m_actors.begin(); it != m_actors.end(); it++) // Give each actor a chance to do something
        if (*it != nullptr && !((*it)->isDead()))
        {
            (**it).doSomething();
        }
    
    if (shouldPlayerDie())
        m_player->setDead(); // Did other actors do something causing player death?
    if (!m_player->isDead())
        m_player->doSomething();
    if (shouldPlayerDie())
        m_player->setDead(); // Did player do something causing player death?
    
    if (m_currentBonus > 0)
        m_currentBonus--;
    
    removeDeadActors(); // Will not remove Player yet
    
    
    if (m_player->isDead())
    {
        decLives();
        playSound(SOUND_PLAYER_DIE);
        return GWSTATUS_PLAYER_DIED;
    }
    else if (didPlayerWin())
    {
        increaseScore(m_currentBonus);
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    else
        return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    for (auto it = m_actors.begin(); it != m_actors.end(); it++)
        delete *it;
    delete m_level;
    delete m_player;
}


//---------------------------------------------------------
// All other functions
//---------------------------------------------------------

int StudentWorld::whatObjectIsThis(Actor* ap) const
{
    if (ap == nullptr)
        return -1;
    pBrick* pB = dynamic_cast<pBrick*>(ap);
    if (pB != nullptr)
        return IID_PERMA_BRICK;
    dBrick* dB = dynamic_cast<dBrick*>(ap);
    if (dB != nullptr)
        return IID_DESTROYABLE_BRICK;
    class exit* eP = dynamic_cast<class exit*>(ap);
    if (eP != nullptr)
        return IID_EXIT;
    sZumi* sZ = dynamic_cast<sZumi*>(ap);
    if (sZ != nullptr)
        return IID_SIMPLE_ZUMI;
    cZumi* cZ = dynamic_cast<cZumi*>(ap);
    if (cZ != nullptr)
        return IID_COMPLEX_ZUMI;
    ExtraLifeGoodie* ELG = dynamic_cast<ExtraLifeGoodie*>(ap);
    if (ELG != nullptr)
        return IID_EXTRA_LIFE_GOODIE;
    WalkThroughWallsGoodie* WTWG = dynamic_cast<WalkThroughWallsGoodie*>(ap);
    if (WTWG != nullptr)
        return IID_WALK_THRU_GOODIE;
    IncreaseSimultSprayerGoodie* ISSG = dynamic_cast<IncreaseSimultSprayerGoodie*>(ap);
    if (ISSG != nullptr)
        return IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE;
    bugSpray* bS = dynamic_cast<bugSpray*>(ap);
    if (bS != nullptr)
        return IID_BUGSPRAY;
    bugSprayer* bSer = dynamic_cast<bugSprayer*>(ap);
    if (bSer != nullptr)
        return IID_BUGSPRAYER;
    
    return -1; // If we reach this point, something is wrong.
}

void StudentWorld::applyBugspray(int x, int y)
{
    for (auto it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if ((**it).getX() == x && (**it).getY() == y)
        {
            if (whatObjectIsThis(*it) == IID_DESTROYABLE_BRICK || whatObjectIsThis(*it) == IID_PLAYER || whatObjectIsThis(*it) == IID_COMPLEX_ZUMI || whatObjectIsThis(*it) == IID_SIMPLE_ZUMI)
                (**it).setDead();
            else if (whatObjectIsThis(*it) == IID_BUGSPRAYER)
            {
                bugSprayer* bs = dynamic_cast<bugSprayer*>(*it);
                bs->noTime();
            }
        }
    }
}

Actor* StudentWorld::spotContains(int ID, int x, int y) const
{
    if (ID == IID_PLAYER)
    {
        if (m_player->getX() == x && m_player->getY() == y)
            return m_player;
        else return nullptr;
    }
    for (auto it = m_actors.begin(); it != m_actors.end(); it++)
    {
        if (*it != nullptr && whatObjectIsThis(*it) == ID && (**it).getX() == x && (**it).getY() == y)
            return *it;
    }
    return nullptr;
    
}
std::string StudentWorld::formatText()
{
    std::ostringstream oss;
    oss.fill('0');
    oss << "Score: " << std::setw(8) << getScore() << "  Level: " << std::setw(2) << getLevel() << "  Lives: " << std::setw(3) << getLives();
    oss.fill(' ');
    oss << "  Bonus:" << std::setw(6) << m_currentBonus;
    return oss.str();
}

bool StudentWorld::didPlayerWin() const
{
    return (killedAll() && spotContains(IID_EXIT, m_player->getX(), m_player->getY()));
}

void StudentWorld::removeDeadActors()
{
    auto it = m_actors.begin();
    while (it != m_actors.end())
    {
        if ( (**it).isDead() )
        {
            delete *it;
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}

bool StudentWorld::shouldPlayerDie() const
{
    int x = m_player->getX();
    int y = m_player->getY();
    if (spotContains(IID_SIMPLE_ZUMI, x, y) || spotContains(IID_COMPLEX_ZUMI, x, y) || spotContains(IID_BUGSPRAY, x, y) || (!m_player->getWalkThroughWalls() && spotContains(IID_DESTROYABLE_BRICK, x, y)))
        return true;
    return false;
}

int StudentWorld::whichGoodie() const
{
    unsigned int ExtraLife = getProbOfExtraLifeGoodie();
    unsigned int WalkThru = getProbOfWalkThruGoodie() + ExtraLife;
    //unsigned int MoreSprayers = getProbOfMoreSprayersGoodie() + WalkThru;
    unsigned int r = randInt(1, 100);
    if (r < ExtraLife)
        return IID_EXTRA_LIFE_GOODIE;
    else if ( r < WalkThru )
        return IID_WALK_THRU_GOODIE;
    else
        return IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE;
}