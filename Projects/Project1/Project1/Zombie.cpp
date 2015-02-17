//
//  Zombie.cpp
//  Project1
//
//  Created by Gautam Gupta on 1/8/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//
#include <iostream>
#include <cstdlib>
#include "Zombie.h"
#include "globals.h"
#include "Arena.h"
using namespace std;
Zombie::Zombie(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A zombie must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Zombie created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_health = INITIAL_ZOMBIE_HEALTH;
    m_idleTurnsRemaining = 0;
}

int Zombie::row() const
{
    return m_row;
}

int Zombie::col() const
{
    return m_col;
}

bool Zombie::isDead() const
{
    return m_health == 0;
}

void Zombie::move()
{
    if (m_idleTurnsRemaining > 0)
    {
        m_idleTurnsRemaining--;
        return;
    }
    
    // Attempt to move in a random direction; if we can't move, don't move
    if (attemptMove(*m_arena, randInt(0, NUMDIRS-1), m_row, m_col))
    {
        if (m_arena->getCellStatus(m_row, m_col) == HAS_BRAIN)
        {
            m_arena->setCellStatus(m_row, m_col, EMPTY);
            m_health--;
        }
    }
    
    if (m_health < INITIAL_ZOMBIE_HEALTH)
        m_idleTurnsRemaining = POISONED_IDLE_TIME;
}
