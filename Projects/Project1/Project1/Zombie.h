//
//  Zombie.h
//  Project1
//
//  Created by Gautam Gupta on 1/8/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//

#ifndef __Project1__Zombie__
#define __Project1__Zombie__
class Arena;
class Zombie
{
public:
    // Constructor
    Zombie(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* defined(__Project1__Zombie__) */
