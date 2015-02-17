//
//  Player.cpp
//  Project1
//
//  Created by Gautam Gupta on 1/8/14.
//  Copyright (c) 2014 Gautam Gupta. All rights reserved.
//
#include <iostream>
#include <string>
#include <cstdlib>
#include "globals.h"
#include "Player.h"
#include "Arena.h"
#include "History.h"
using namespace std;

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropBrain()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_BRAIN)
        return "There's already a brain at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_BRAIN);
    return "A brain has been dropped.";
}

string Player::move(int dir)
{
    if (attemptMove(*m_arena, dir, m_row, m_col))
    {
        if (m_arena->numberOfZombiesAt(m_row, m_col) > 0)
        {
            setDead();
            return "Player walked into a zombie and died.";
        }
        string msg = "Player moved ";
        switch (dir)
        {
            case NORTH: msg += "north"; break;
            case EAST:  msg += "east";  break;
            case SOUTH: msg += "south"; break;
            case WEST:  msg += "west";  break;
        }
        msg += ".";
        return msg;
    }
    else
        return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}
