#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"
#include <iostream>
#include <string>
class StudentWorld;
// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp


//---------------------------------------------------------
// !-- Utilities used by various Actors --!
//---------------------------------------------------------
struct Timer // Used for TimedActors which must do an action after t ticks
{
public:
    Timer(int t = 0) : m_time(t) {}
    void tick() { if (m_time > 0) m_time--; }
    void dieNow() { m_time = 0; }
    bool expired() { return m_time == 0; }
    void resetTime(int t) { m_time = t; }
private:
        int m_time;
};

struct Coord // Used for the breadth-first search in cZumi
{
    Coord(int _x, int _y) : x(_x), y(_y) {}
    int x;
    int y;
};

int randInt(int lowest, int highest);
bool probability(int pct);


//---------------------------------------------------------
// !-- Actor Class Declarations --!
//---------------------------------------------------------
class Actor : public GraphObject
{
public:
    Actor(int IID, int sx, int sy, StudentWorld* sw) : GraphObject(IID, sx, sy), m_world(sw) {}
    virtual ~Actor() { setVisible(false); }
    bool isDead() const { return m_dead; }
    virtual void setDead() { m_dead = true; }
    virtual void doSomething() = 0;
    StudentWorld* getWorld() const { return m_world; }
    virtual bool canMoveHere(int dir, bool d) const; //Returns whether or not an actor can move in direction dir due to presence of a block. Overridden when player can walk thru walls.
    
private:
    bool m_dead = false;
    StudentWorld* m_world;
};




class Player : public Actor
{

public:
    // Housekeeping
    Player(int sx, int sy, StudentWorld* sw);
    ~Player() { }
    // Accessors
    int getCapacity() const { return m_dropCapacity; }
    int getSprayersDropped() const { return m_sprayersDropped; }
    bool getWalkThroughWalls() const { return m_walkThroughWalls; }
    bool canMoveHere(int dir) const; // Overrides Actor::canMoveHere when can walk thru walls
    // Mutators
    void setWalkThroughWalls(bool b) { m_walkThroughWalls = b; }
    void setBoostedSprayer(bool b) { m_capacityBoosted = b; }
    void doSomething();
    void dropSprayerIfPossible(int x, int y); //If a sprayer can be dropped, it will be
    void sprayerDied() { m_sprayersDropped--; }
private:
    int m_dropCapacity = 2;
    int m_boostedDropCapacity;
    int m_sprayersDropped = 0;
    bool m_walkThroughWalls = false;
    bool m_capacityBoosted = false;
    Timer m_walkThroughWallsTime;
    Timer m_boostedSprayerTime;
};


class Brick : public Actor
{
public:
    Brick(int IID, int sx, int sy, StudentWorld* sw) : Actor(IID, sx, sy, sw) {setVisible(true);}
    virtual ~Brick() {}
};

class dBrick : public Brick
{
public:
    dBrick(int sx, int sy, StudentWorld* sw) : Brick(IID_DESTROYABLE_BRICK, sx, sy, sw) {}
    virtual void doSomething() { }
};

class pBrick : public Brick
{
public:
    pBrick(int sx, int sy, StudentWorld* sw) : Brick(IID_PERMA_BRICK, sx, sy, sw) {}
    virtual void doSomething() { }
};

class exit : public Actor
{
public:
    exit(int sx, int sy, StudentWorld* sw) : Actor(IID_EXIT, sx, sy, sw), m_revealed(false) {}
    virtual void doSomething();
private:
    bool m_revealed;
};


class Zumi : public Actor
{
public:
    Zumi(int sID, int sx, int sy, StudentWorld* sw, int ticksPerMove) : Actor(sID, sx, sy, sw), m_timer(ticksPerMove)
    {
        setVisible(true);
        m_randDir = randInt(1000, 1003);
    }
    virtual ~Zumi() { }
    void doSomething();
    bool doesZumiMove();
    virtual void setDead();
    virtual void move();
private:
    Timer m_timer;
    int m_randDir;
};

class sZumi : public Zumi // simple Zumi
{
public:
    sZumi(int sx, int sy, StudentWorld* sw);
    void setDead();
};

class cZumi : public Zumi // Complex Zumi
{
public:
    cZumi(int sx, int sy, StudentWorld* sw);
    virtual void move();
    void setDead();
    bool smell();
    int search();
    bool stepsToSolve(Coord s, Coord orig, Coord f, int& steps);
};

//---------------------------------------------------------
// !-- Actors which have a timed lifetime --!
//---------------------------------------------------------

class TimedLifeActor : public Actor
{
public:
    TimedLifeActor(int IID, int sx, int sy, StudentWorld* sw, int time = 0) : Actor(IID, sx, sy, sw), m_timer(time) {}
    virtual void doSomething() = 0;
    bool timeToDie();
    void noTime() { m_timer.dieNow(); }
    virtual ~TimedLifeActor() { }
private:
    Timer m_timer;
};


class Goodie : public TimedLifeActor
{
public:
    Goodie(int IID, int sx, int sy, StudentWorld* sw, int lifetime) : TimedLifeActor(IID, sx, sy, sw, lifetime) {setVisible(true);}
    void doSomething();  
};

class ExtraLifeGoodie : public Goodie
{
public:
    ExtraLifeGoodie(int sx, int sy, StudentWorld* sw);
};

class WalkThroughWallsGoodie : public Goodie
{
public:
    WalkThroughWallsGoodie(int sx, int sy, StudentWorld* sw);
};

class IncreaseSimultSprayerGoodie : public Goodie
{
public:
    IncreaseSimultSprayerGoodie(int sx, int sy, StudentWorld* sw);
};

class bugSpray : public TimedLifeActor
{
public:
    bugSpray(int sx, int sy, StudentWorld* sw) : TimedLifeActor(IID_BUGSPRAY, sx, sy, sw, 3) {setVisible(true);}
    void doSomething();
};

class bugSprayer : public TimedLifeActor
{
public:
    bugSprayer(int sx, int sy, StudentWorld* sw) : TimedLifeActor(IID_BUGSPRAYER, sx, sy, sw, 40) {setVisible(true);}
    void doSomething();
    void spray();
};

#endif // ACTOR_H_
