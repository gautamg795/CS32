#include "Actor.h"
#include "StudentWorld.h"
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <queue>
using namespace std;
// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

//---------------------------------------------------------
// !-- ACTOR FUNCTIONS --!
//---------------------------------------------------------
#pragma mark Actor
// No actor can walk on the same place as a block (usually).
// This function determines if an actor can move in
// the specified direction (not impeded)
bool Actor::canMoveHere(int dir, bool d = false) const // 'd' is true when Player can walk through walls.
{
    switch (dir)
    {
        case KEY_PRESS_LEFT:
            if ((!getWorld()->spotContains(IID_PERMA_BRICK, getX()-1, getY()) && !getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX()-1, getY())) || (d &&  !getWorld()->spotContains(IID_PERMA_BRICK, getX()-1, getY())))
                return true;
            return false;
        case KEY_PRESS_DOWN:
            if ((!getWorld()->spotContains(IID_PERMA_BRICK, getX(), getY()-1) && !getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX(), getY()-1)) || (d && !getWorld()->spotContains(IID_PERMA_BRICK, getX(), getY()-1)))
                return true;
            return false;
        case KEY_PRESS_RIGHT:
            if ((!getWorld()->spotContains(IID_PERMA_BRICK, getX()+1, getY()) && !getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX()+1, getY())) || (d && !getWorld()->spotContains(IID_PERMA_BRICK, getX()+1, getY())))
                return true;
            return false;
        case KEY_PRESS_UP:
            if ((!getWorld()->spotContains(IID_PERMA_BRICK, getX(), getY()+1) && !getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX(), getY()+1)) || (d && !getWorld()->spotContains(IID_PERMA_BRICK, getX(), getY()+1)) )
                return true;
            return false;
        default: return false;
    }
}


//---------------------------------------------------------
// !-- PLAYER FUNCTIONS --!
//---------------------------------------------------------
#pragma mark Player
// Players, specifically, can walk through destructible walls sometimes.
// This function redirects the player's actions as necessary.
bool Player::canMoveHere(int dir) const
{
    if (!m_walkThroughWalls)
        return Actor::canMoveHere(dir);
    return Actor::canMoveHere(dir, true);
}

Player::Player(int sx, int sy, StudentWorld* sw)
: Actor(IID_PLAYER, sx, sy, sw), m_boostedDropCapacity(sw->getMaxBoostedSprayers()), m_walkThroughWallsTime(sw->getWalkThroughLifetimeTicks()), m_boostedSprayerTime(sw->getBoostedSprayerLifetimeTicks())
{
    setVisible(true);
}

void Player::dropSprayerIfPossible(int x, int y)
{
    if ((m_capacityBoosted && m_sprayersDropped >= m_boostedDropCapacity) || (!m_capacityBoosted && m_sprayersDropped >= m_dropCapacity))
        return;
    if (getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX( ), getY()) || getWorld()->spotContains(IID_BUGSPRAYER, getX(), getY())) // We never stand on a permabrick anyhow
        return;
    getWorld()->addActor(new bugSprayer(x, y, getWorld()));
    m_sprayersDropped++;
        
}

void Player::doSomething()
{
    if (isDead())
        return;
    // StudentWorld::shouldPlayerDie() already checks to see if colocation with another Actor should kill the Player; further checks are superfluous
    int ch;
    if (getWorld()->getKey(ch))
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT:
                if (canMoveHere(KEY_PRESS_LEFT))
                    moveTo(getX()-1, getY());
                break;
            case KEY_PRESS_DOWN:
                if (canMoveHere(KEY_PRESS_DOWN))
                    moveTo(getX(), getY()-1);
                break;
            case KEY_PRESS_RIGHT:
                if (canMoveHere(KEY_PRESS_RIGHT))
                    moveTo(getX()+1, getY());
                break;
            case KEY_PRESS_UP:
                if (canMoveHere(KEY_PRESS_UP))
                    moveTo(getX(), getY()+1);
                break;
            case KEY_PRESS_SPACE:
                dropSprayerIfPossible(getX(), getY());
                break;
        }
    }
    
    // Check if Player can walk through walls
    
    if (m_walkThroughWalls && !m_walkThroughWallsTime.expired())
        m_walkThroughWallsTime.tick();
    else if (m_walkThroughWallsTime.expired())
    {
        m_walkThroughWalls = false;
        m_walkThroughWallsTime.resetTime(getWorld()->getWalkThroughLifetimeTicks());
    }
    
    
    //Check if the Player's drop capacity is boosteed
    if (m_capacityBoosted && !m_boostedSprayerTime.expired())
        m_boostedSprayerTime.tick();
    if (m_boostedSprayerTime.expired())
    {
        m_capacityBoosted = false;
        m_boostedSprayerTime.resetTime(getWorld()->getBoostedSprayerLifetimeTicks());
    }
    
    
}

//---------------------------------------------------------
// !-- ZUMI FUNCTIONS --!
//---------------------------------------------------------
#pragma mark Zumi
bool Zumi::doesZumiMove()
{
    cZumi* cz = dynamic_cast<cZumi*>(this);
    sZumi* sz = dynamic_cast<sZumi*>(this);
    if (!m_timer.expired())
    {
        m_timer.tick();
        return false;
    }
    else
    {
        if (cz != nullptr)
            m_timer.resetTime(getWorld()->getTicksPerComplexZumiMove());
        else if (sz != nullptr)
            m_timer.resetTime(getWorld()->getTicksPerSimpleZumiMove());
    }
    return true;
}

void Zumi::doSomething()
{
    if (isDead())
        return;
    if (getWorld()->spotContains(IID_PLAYER, getX(), getY()))
        getWorld()->killPlayer();
    if (doesZumiMove())
        move();
        
}

void Zumi::move()
{
    switch (m_randDir)
    {
        case KEY_PRESS_UP:
            if (canMoveHere(KEY_PRESS_UP) && !getWorld()->spotContains(IID_BUGSPRAYER, getX(), getY()+1))
                moveTo(getX(), getY()+1);
            else break;
            return;
        case KEY_PRESS_RIGHT:
            if (canMoveHere(KEY_PRESS_RIGHT) && !getWorld()->spotContains(IID_BUGSPRAYER, getX()+1, getY()))
                moveTo(getX()+1, getY());
            else break;
            return;
        case KEY_PRESS_DOWN:
            if (canMoveHere(KEY_PRESS_DOWN) && !getWorld()->spotContains(IID_BUGSPRAYER, getX(), getY()-1))
                moveTo(getX(), getY()-1);
            else break;
            return;
        case KEY_PRESS_LEFT:
            if (canMoveHere(KEY_PRESS_LEFT) && !getWorld()->spotContains(IID_BUGSPRAYER, getX()-1, getY()))
                moveTo(getX()-1, getY());
            else break;
            return;
    }
    
    m_randDir = randInt(1000, 1003);
}

void Zumi::setDead()
{
    Actor::setDead();
    getWorld()->zumiDied();
    getWorld()->playSound(SOUND_ENEMY_DIE);
    if (!probability(getWorld()->getProbOfGoodieOverall()))
        return;
    int ID = getWorld()->whichGoodie();
    switch (ID)
    {
        case IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE:
            getWorld()->addActor(new IncreaseSimultSprayerGoodie(getX(), getY(), getWorld()));
            break;
        case IID_EXTRA_LIFE_GOODIE:
            getWorld()->addActor(new ExtraLifeGoodie(getX(), getY(), getWorld()));
            break;
        case IID_WALK_THRU_GOODIE:
            getWorld()->addActor(new WalkThroughWallsGoodie(getX(), getY(), getWorld()));
            break;
    }
    
}

//---------------------------------------------------------
// !-- COMPLEX ZUMI FUNCTIONS --!
//---------------------------------------------------------
#pragma mark cZumi
cZumi::cZumi(int sx, int sy, StudentWorld* sw)
: Zumi(IID_COMPLEX_ZUMI, sx, sy, sw, sw->getTicksPerComplexZumiMove()) {}

void cZumi::move()
{
    if (smell())
    {
        int dir = search();
        switch (dir)
        {
            case -1:
                Zumi::move();
                return;
            case KEY_PRESS_UP:
                moveTo(getX(), getY()+1);
                return;
            case KEY_PRESS_RIGHT:
                moveTo(getX()+1, getY());
                return;
            case KEY_PRESS_DOWN:
                moveTo(getX(), getY()-1);
                return;
            case KEY_PRESS_LEFT:
                moveTo(getX()-1, getY());
                return;
        }
        
    }
    else
        Zumi::move();
}

int cZumi::search()
{
    
    int upSteps = 0, rightSteps = 0, downSteps = 0, leftSteps = 0;
    Coord orig(getX(), getY());
    Coord finish(getWorld()->getPlayerX(), getWorld()->getPlayerY());
    bool up = stepsToSolve({getX(), getY()+1}, orig, finish, upSteps);
    bool right = stepsToSolve({getX()+1, getY()}, orig, finish, rightSteps);
    bool down = stepsToSolve({getX(), getY()-1}, orig, finish, downSteps);
    bool left = stepsToSolve({getX()-1, getY()}, orig, finish, leftSteps);
    int dir = -1, min = 9999;
    if (up && upSteps < min)
    {
        dir = KEY_PRESS_UP;
        min = upSteps;
    }
    if (right && rightSteps < min)
    {
        dir = KEY_PRESS_RIGHT;
        min = rightSteps;
    }
    if (down && downSteps < min)
    {
        dir = KEY_PRESS_DOWN;
        min = downSteps;
    }
    if (left && leftSteps < min)
    {
        dir = KEY_PRESS_LEFT;
        min = leftSteps;
    }
    return dir;
}

bool cZumi::stepsToSolve(Coord s, Coord orig, Coord f, int& steps)
{
    string maze[VIEW_HEIGHT] =
    {
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "...............",
        "..............."
    };
    for (int y = VIEW_HEIGHT-1; y >= 0; y--)
        for (int x = 0; x < VIEW_WIDTH; x++)
            if (getWorld()->spotContains(IID_PERMA_BRICK, x, VIEW_HEIGHT-y-1) || getWorld()->spotContains(IID_DESTROYABLE_BRICK, x, VIEW_HEIGHT-y-1) || getWorld()->spotContains(IID_BUGSPRAYER, x, VIEW_HEIGHT-y-1))
                maze[y][x] = '*';
    queue<Coord> cQueue;
    if (maze[VIEW_HEIGHT-1-s.y][s.x] != '.')
        return false;
    maze[VIEW_HEIGHT-1-orig.y][orig.x] = '*';
    steps = 0;
    cQueue.push(s);
    maze[VIEW_HEIGHT-1-s.y][s.x] = '*';
    while(!cQueue.empty())
    {
        int curX = cQueue.front().x;
        int curY = cQueue.front().y;
        cQueue.pop();
        steps++;
        if (curX == f.x && curY == f.y)
            return true;
        if (maze[VIEW_HEIGHT-1-curY-1][curX] == '.') // North
        {
            cQueue.push({curX, curY+1});
            maze[VIEW_HEIGHT-1-curY-1][curX] = '*';
        }
        if (maze[VIEW_HEIGHT-1-curY][curX+1] == '.') // East
        {
            cQueue.push({curX+1, curY});
            maze[VIEW_HEIGHT-1-curY][curX+1] = '*';
        }
        if (maze[VIEW_HEIGHT-1-curY+1][curX] == '.') // South
        {
            cQueue.push({curX, curY-1});
            maze[VIEW_HEIGHT-1-curY+1][curX] = '*';
        }
        if (maze[VIEW_HEIGHT-1-curY][curX-1] == '.') // West
        {
            cQueue.push({curX-1, curY});
            maze[VIEW_HEIGHT-1-curY][curX-1] = '*';
        }
        
        
    }
    
    return false;
}

void cZumi::setDead()
{
    if (isDead())
        return;
    Zumi::setDead();
    getWorld()->increaseScore(500);
}

bool cZumi::smell()
{
    int smellDistance = getWorld()->getComplexZumiSearchDistance();
    int xDist = abs(getWorld()->getPlayerX() - getX());
    int yDist = abs(getWorld()->getPlayerY() - getY());
    return std::max(xDist, yDist) <= smellDistance;
}

//---------------------------------------------------------
// !-- SIMPLE ZUMI FUNCTIONS --!
//---------------------------------------------------------
#pragma mark sZumi
sZumi::sZumi(int sx, int sy, StudentWorld* sw)
: Zumi(IID_SIMPLE_ZUMI, sx, sy, sw, sw->getTicksPerSimpleZumiMove()) {}

void sZumi::setDead()
{
    if (isDead())
        return;
    Zumi::setDead();
    getWorld()->increaseScore(100);
}


//---------------------------------------------------------
// !-- EXIT FUNCTIONS --!
//---------------------------------------------------------
#pragma mark Exit
void exit::doSomething()
{
    if (getWorld()->killedAll() && !m_revealed)
    {
        setVisible(true);
        m_revealed = true;
        getWorld()->playSound(SOUND_REVEAL_EXIT);
    }
}

//---------------------------------------------------------
// !-- GOODIE FUNCTIONS --!
//---------------------------------------------------------
#pragma mark Goodie
void Goodie::doSomething()
{
    if (isDead())
        return;
    if (timeToDie())
        setDead();
    Actor* ap;
    if ((ap = getWorld()->spotContains(IID_PLAYER ,getX(), getY())))
    {
        Player* pp = dynamic_cast<Player*>(ap);

        int ID = getWorld()->whatObjectIsThis(this);
        if (ID == IID_EXTRA_LIFE_GOODIE)
            getWorld()->incLives();
        else if (ID == IID_WALK_THRU_GOODIE)
            pp->setWalkThroughWalls(true);
        else if (ID == IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE)
            pp->setBoostedSprayer(true);
        
        //The following happens regardless of what goodie was made
        setDead();
        getWorld()->increaseScore(1000);
        getWorld()->playSound(SOUND_GOT_GOODIE);
    }
    
}

ExtraLifeGoodie::ExtraLifeGoodie(int sx, int sy, StudentWorld* sw)
: Goodie(IID_EXTRA_LIFE_GOODIE, sx, sy, sw, sw->getGoodieLifetimeInTicks()) {}



WalkThroughWallsGoodie::WalkThroughWallsGoodie(int sx, int sy, StudentWorld* sw)
: Goodie(IID_WALK_THRU_GOODIE, sx, sy, sw, sw->getGoodieLifetimeInTicks()) {}

IncreaseSimultSprayerGoodie::IncreaseSimultSprayerGoodie(int sx, int sy, StudentWorld* sw)
: Goodie(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE, sx, sy, sw, sw->getGoodieLifetimeInTicks()) {}

//---------------------------------------------------------
// !-- BUGSPRAY/SPRAYER FUNCTIONS --!
//---------------------------------------------------------
#pragma mark BugSpray/Sprayer
void bugSprayer::doSomething()
{
    if (timeToDie() || isDead())
    {
        spray();
        getWorld()->sprayerSprayed();
        setDead();
        getWorld()->playSound(SOUND_SPRAY);
    }
}

void bugSprayer::spray()
{
    getWorld()->addActor(new bugSpray(getX(), getY(), getWorld()));
    for (int i = 1; i < 3; i++) // +X direction
    {
        if (getWorld()->spotContains(IID_PERMA_BRICK, getX() + i, getY()))
            break;
        else if (getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX() + i, getY()))
            {
                getWorld()->addActor(new bugSpray(getX() + i, getY(), getWorld()));
                break;
            }
        else
            getWorld()->addActor(new bugSpray(getX() + i, getY(), getWorld()));
    }
    for (int i = 1; i < 3; i++) // +Y direction
    {
        if (getWorld()->spotContains(IID_PERMA_BRICK, getX(), getY() + i))
            break;
        else if (getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX(), getY() + i))
        {
            getWorld()->addActor(new bugSpray(getX(), getY() + i, getWorld()));
            break;
        }
        else
            getWorld()->addActor(new bugSpray(getX(), getY() + i, getWorld()));
    }
    for (int i = -1; i > -3; i--) // -X direction
    {
        if (getWorld()->spotContains(IID_PERMA_BRICK, getX() + i, getY()))
            break;
        else if (getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX() + i, getY()))
        {
            getWorld()->addActor(new bugSpray(getX() + i, getY(), getWorld()));
            break;
        }
        else
            getWorld()->addActor(new bugSpray(getX() + i, getY(), getWorld()));
    }
    for (int i = -1; i > -3; i--) // -Y direction
    {
        if (getWorld()->spotContains(IID_PERMA_BRICK, getX(), getY() + i))
            break;
        else if (getWorld()->spotContains(IID_DESTROYABLE_BRICK, getX(), getY() + i))
        {
            getWorld()->addActor(new bugSpray(getX(), getY() + i, getWorld()));
            break;
        }
        else
            getWorld()->addActor(new bugSpray(getX(), getY() + i, getWorld()));
    }
}

void bugSpray::doSomething()
{
    if (isDead())
        return;
    else if (timeToDie())
        setDead();
    else
    {
        getWorld()->applyBugspray(getX(), getY());
    }
}


//---------------------------------------------------------
// !-- OTHER HELPER FUNCTIONS --!
//---------------------------------------------------------
#pragma mark Tools
bool TimedLifeActor::timeToDie()
{
    if (m_timer.expired())
        return true;
    m_timer.tick();
    return false;
}

int randInt(int lowest, int highest)
{
    if (highest < lowest)
        std::swap(highest, lowest);
    return lowest + (std::rand() % (highest - lowest + 1));
}

bool probability(int pct)
{
    return randInt(0, 99) < pct;
}