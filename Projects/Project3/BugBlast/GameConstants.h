#ifndef GAMECONSTANTS_H_
#define GAMECONSTANTS_H_

// IDs for the game objects

const int IID_PLAYER	       = 0;
const int IID_SIMPLE_ZUMI      = 1;
const int IID_COMPLEX_ZUMI     = 2;
const int IID_EXIT             = 3;
const int IID_BUGSPRAYER       = 4;
const int IID_BUGSPRAY         = 5;
const int IID_EXTRA_LIFE_GOODIE= 6;
const int IID_WALK_THRU_GOODIE = 7;
const int IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE = 8;
const int IID_PERMA_BRICK      = 9;
const int IID_DESTROYABLE_BRICK= 10;

// sounds

const int SOUND_ENEMY_DIE      = 0;
const int SOUND_PLAYER_DIE     = 1;
const int SOUND_SPRAY	       = 2;
const int SOUND_GOT_GOODIE     = 3;
const int SOUND_REVEAL_EXIT    = 4;
const int SOUND_FINISHED_LEVEL = 5;

// keys the user can hit

const int KEY_PRESS_LEFT  = 1000;
const int KEY_PRESS_RIGHT = 1001;
const int KEY_PRESS_UP    = 1002;
const int KEY_PRESS_DOWN  = 1003;
const int KEY_PRESS_SPACE = ' ';

// board dimensions 

const int VIEW_WIDTH  = 15;
const int VIEW_HEIGHT = 15;

// status of each tick (did the player die?)

const int GWSTATUS_PLAYER_DIED   = 0;
const int GWSTATUS_CONTINUE_GAME = 1;
const int GWSTATUS_PLAYER_WON	 = 2;
const int GWSTATUS_FINISHED_LEVEL= 3;
const int GWSTATUS_NO_FIRST_LEVEL= 4;
const int GWSTATUS_LEVEL_ERROR   = 5;

// test parameter constants

const int NUM_TEST_PARAMS = 1;

#endif // GAMECONSTANTS_H_
