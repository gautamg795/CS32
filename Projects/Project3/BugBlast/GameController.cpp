#if defined(__APPLE__)
//#include <OpenGL/OpenGLAvailability.h>
#undef OPENGL_DEPRECATED
#undef OPENGL_DEPRECATED_MSG
#define OPENGL_DEPRECATED(from, to)
#define OPENGL_DEPRECATED_MSG(from, to, msg)
#include <GLUT/glut.h>
#elif !defined(unix)
#include "glut.h"
#endif

#include "GameController.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include "GraphObject.h"
#include "SoundFX.h"
#include <string>
#include <map>
#include <utility>
#include <cstdlib>
using namespace std;

static const double SCORE_Y = 3.8;
static const double SCORE_Z = -10;

static const int MS_PER_FRAME = 10;

static const double VISIBLE_MIN_X = -3.25;
static const double VISIBLE_MAX_X = 3.25;
static const double VISIBLE_MIN_Y = -2;
static const double VISIBLE_MAX_Y = 2;
static const double VISIBLE_MIN_Z = -20;
static const double VISIBLE_MAX_Z = -6;

static const double FONT_SCALEDOWN = 760.0;

static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;

static const int PERSPECTIVE_NEAR_PLANE = 4;
static const int PERSPECTIVE_FAR_PLANE  = 22;

static const double PI = 4 * atan(1.0);

struct Point
{
	double dx;
	double dy;
};

static void drawPrompt(string mainMessage, string secondMessage);
static void drawScoreAndLives(string);

static void drawPlayer(GraphObject* go);
static void drawSimpleZumi(GraphObject* go);
static void drawComplexZumi(GraphObject* go);
static void drawBugSpray(GraphObject* go);
static void drawBugSprayer(GraphObject* go);
static void drawSmallbot(GraphObject* go);
static void drawExit(GraphObject* go);
static void drawPermaBrick(GraphObject* go);
static void drawGoodie(GraphObject* go);
static void drawDestroyableBrick(GraphObject* go);

void GameController::initDrawersAndSounds()
{
	DrawMapType::value_type drawers[] = {
		make_pair(IID_PLAYER		   , &drawPlayer),
		make_pair(IID_SIMPLE_ZUMI      , &drawSimpleZumi),
		make_pair(IID_COMPLEX_ZUMI     , &drawComplexZumi),
		make_pair(IID_EXIT             , &drawExit),
		make_pair(IID_BUGSPRAYER       , &drawBugSprayer),
		make_pair(IID_BUGSPRAY         , &drawBugSpray),
		make_pair(IID_EXTRA_LIFE_GOODIE ,&drawGoodie),
		make_pair(IID_WALK_THRU_GOODIE  , &drawGoodie),
		make_pair(IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE  , &drawGoodie),
		make_pair(IID_PERMA_BRICK      , &drawPermaBrick),
		make_pair(IID_DESTROYABLE_BRICK, &drawDestroyableBrick),
	};

	SoundMapType::value_type sounds[] = {
		make_pair(SOUND_ENEMY_DIE              , "explode.wav"),
		make_pair(SOUND_PLAYER_DIE             , "die.wav"),
		make_pair(SOUND_SPRAY			       , "spray.wav"),
		make_pair(SOUND_GOT_GOODIE             , "goodie.wav"),
		make_pair(SOUND_REVEAL_EXIT 		   , "revealexit.wav"),
		make_pair(SOUND_FINISHED_LEVEL		   , "finished.wav")
	};
	
	for (int k = 0; k < sizeof(drawers)/sizeof(drawers[0]); k++)
		m_drawMap[drawers[k].first] = drawers[k].second;
	for (int k = 0; k < sizeof(sounds)/sizeof(sounds[0]); k++)
		m_soundMap[sounds[k].first] = sounds[k].second;
}

static void doSomethingCallback()
{
	Game().doSomething();
}

static void reshapeCallback(int w, int h)
{
	Game().reshape(w, h);
}

static void keyboardEventCallback(unsigned char key, int x, int y)
{
	Game().keyboardEvent(key, x, y);
}

static void specialKeyboardEventCallback(int key, int x, int y)
{
	Game().specialKeyboardEvent(key, x, y);
}

static void timerFuncCallback(int val)
{
	Game().doSomething();
	glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);
}

void GameController::run(GameWorld* gw, int testParams[], string windowTitle)
{
	gw->setTestParams(testParams);
	gw->setController(this);
	m_gw = gw;
	m_gameState = welcome;
	m_lastKeyHit = INVALID_KEY;
	m_singleStep = false;
	m_curIntraFrameTick = 0;
	m_playerWon = false;

	initDrawersAndSounds();

	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); 
	glutInitWindowPosition(0, 0); 
	glutCreateWindow(windowTitle.c_str()); 

	glutKeyboardFunc(keyboardEventCallback);
	glutSpecialFunc(specialKeyboardEventCallback);
	glutReshapeFunc(reshapeCallback);
	glutDisplayFunc(doSomethingCallback);
	glutTimerFunc(MS_PER_FRAME, timerFuncCallback, 0);

	glutMainLoop(); 
}

void GameController::keyboardEvent(unsigned char key, int /* x */, int /* y */)
{
	switch (key)
	{
		case 'a': case '4': m_lastKeyHit = KEY_PRESS_LEFT;  break;
		case 'd': case '6': m_lastKeyHit = KEY_PRESS_RIGHT; break;
		case 'w': case '8': m_lastKeyHit = KEY_PRESS_UP;    break;
		case 's': case '2': m_lastKeyHit = KEY_PRESS_DOWN;  break;
		case 'f':           m_singleStep = true;            break;
		case 'r':           m_singleStep = false;           break;
		case 'q': case 'Q': m_gameState = quit;             break;
		default:            m_lastKeyHit = key;             break;
	}
}

void GameController::specialKeyboardEvent(int key, int /* x */, int /* y */)
{
	switch (key)
	{
		case GLUT_KEY_LEFT:  m_lastKeyHit = KEY_PRESS_LEFT;  break;
		case GLUT_KEY_RIGHT: m_lastKeyHit = KEY_PRESS_RIGHT; break;
		case GLUT_KEY_UP:    m_lastKeyHit = KEY_PRESS_UP;    break;
		case GLUT_KEY_DOWN:  m_lastKeyHit = KEY_PRESS_DOWN;  break;
		default:             m_lastKeyHit = INVALID_KEY;     break;
	}
}

void GameController::playSound(int soundID)
{
	SoundMapType::const_iterator p = m_soundMap.find(soundID);
	if (p != m_soundMap.end())
		SoundFX().playClip(p->second);
}

void GameController::doSomething()
{
	int result;

	switch (m_gameState)
	{
		case not_applicable:
			break;
		case welcome:
			SoundFX().playClip("theme.wav");
			m_mainMessage = "Welcome to Bug Blast!";
			m_secondMessage = "Press Enter to begin play...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = init;
			break;
		case contgame:
			m_mainMessage = "You lost a life!";
			m_secondMessage = "Press Enter to continue playing...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = cleanup;
			break;
		case finishedlevel:
			m_mainMessage = "Woot! You finished the level!";
			m_secondMessage = "Press Enter to continue playing...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = cleanup;
			break;
		case makemove:
			m_curIntraFrameTick = ANIMATION_POSITIONS_PER_TICK;
			m_nextStateAfterAnimate = not_applicable;
			result = m_gw->move();
			if (result == GWSTATUS_PLAYER_DIED)
			{
				if (m_gw->isGameOver())
					m_nextStateAfterAnimate = gameover;		// animate one last frame so the player can see what happened
				else
					m_nextStateAfterAnimate = contgame;		// animate one last frame so the player can see what happened
			}
			else if (result == GWSTATUS_FINISHED_LEVEL)
			{
				m_gw->advanceToNextLevel();
				m_nextStateAfterAnimate = finishedlevel;	// animate one last frame so the player can see what happened
			}
			
			m_gameState = animate;
			break;
		case animate:
			displayGamePlay();
			if (m_curIntraFrameTick-- <= 0)
			{
				if (m_nextStateAfterAnimate != not_applicable)
					m_gameState = m_nextStateAfterAnimate;
				else
				{
					int key;
					if (!m_singleStep  ||  getLastKey(key))
						m_gameState = makemove;
				}
			}
			break;
		case cleanup:
			m_gw->cleanUp();
			m_gameState = init;
			break;
		case gameover:
			{
				ostringstream oss;
				if (m_playerWon)
					oss << "You finished the game! Final score: " << m_gw->getScore() << "!";
				else
					oss << "Game Over! Your final score was " << m_gw->getScore() << "!";
				m_mainMessage = oss.str();
			}
			m_secondMessage = "Press Enter to quit...";
			m_gameState = prompt;
			m_nextStateAfterPrompt = quit;
			break;
		case prompt:
			drawPrompt(m_mainMessage, m_secondMessage);
			{
				int key;
				if (getLastKey(key) && key == '\r')
					m_gameState = m_nextStateAfterPrompt;
			}
			break;
		case init:
			result = m_gw->init();
			SoundFX().abortClip();
			if (result == GWSTATUS_PLAYER_WON)
			{
				m_playerWon = true;
				m_gameState = gameover;
			}
			else if (result == GWSTATUS_NO_FIRST_LEVEL)
			{
				m_mainMessage = "Cannot find first level data file!";
				m_secondMessage = "Press Enter to quit...";
				m_gameState = prompt;
				m_nextStateAfterPrompt = quit;
			}
			else if (result == GWSTATUS_LEVEL_ERROR)
			{
				m_mainMessage = "Error in level data file encoding!";
				m_secondMessage = "Press Enter to quit...";
				m_gameState = prompt;
				m_nextStateAfterPrompt = quit;
			}
			else
				m_gameState = makemove;
			break;
		case quit:
			exit(0);
	}
}

void GameController::displayGamePlay()
{
	glEnable(GL_DEPTH_TEST); // must be done each time before displaying graphics or gets disabled for some reason
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(0, 0, 0, 0, 0, -1, 0, 1, 0);
	
	std::set<GraphObject*>& graphObjects = GraphObject::getGraphObjects();
	for (std::set<GraphObject*>::iterator it = graphObjects.begin(); it != graphObjects.end(); it++)
	{
		GraphObject* cur = *it;
		if (cur->isVisible())
		{
			cur->animate();
			DrawMapType::const_iterator p = m_drawMap.find(cur->getID());
			if (p != m_drawMap.end())
				(*p->second)(cur);  // draw routine for the current object
		}
	}
	
	drawScoreAndLives(m_gameStatText);
	
	glutSwapBuffers();
}

void GameController::reshape (int w, int h) 
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h); 
	glMatrixMode (GL_PROJECTION); 
	glLoadIdentity ();
	gluPerspective(45.0, double(WINDOW_WIDTH) / WINDOW_HEIGHT, PERSPECTIVE_NEAR_PLANE, PERSPECTIVE_FAR_PLANE);
	glMatrixMode (GL_MODELVIEW); 
} 

static void convertToGlutCoords(double x, double y, double& gx, double& gy, double& gz)
{
	x /= VIEW_WIDTH;
	y /= VIEW_HEIGHT;
	gx = 2 * VISIBLE_MIN_X + .3 + x * 2 * (VISIBLE_MAX_X - VISIBLE_MIN_X);
	gy = 2 * VISIBLE_MIN_Y +      y * 2 * (VISIBLE_MAX_Y - VISIBLE_MIN_Y);
	gz = .6 * VISIBLE_MIN_Z;
}

static void doOutputStroke(GLfloat x, GLfloat y, GLfloat z, GLfloat size, const char* str, bool centered)
{
	if (centered)
	{
		double len = glutStrokeLength(GLUT_STROKE_ROMAN, reinterpret_cast<const unsigned char*>(str)) / FONT_SCALEDOWN;
		x = -len / 2;
		size = 1;
	}
	GLfloat scaledSize = size / FONT_SCALEDOWN;
	glPushMatrix();
	glLineWidth(1);
	glLoadIdentity();
	glTranslatef(x, y, z);
	glScalef(scaledSize, scaledSize, scaledSize);
	for ( ; *str != '\0'; str++)
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *str);
	glPopMatrix();
}

static void outputStroke(GLfloat x, GLfloat y, GLfloat z, GLfloat size, const char* str)
{
	doOutputStroke(x, y, z, size, str, false);
}

static void outputStrokeCentered(GLfloat y, GLfloat z, const char* str)
{
	doOutputStroke(0, y, z, 1, str, true);
}

static void drawPolyFromBaseXY(double x, double y, Point points[], int nPoints)
{
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	double xmult = double(VISIBLE_MAX_X - VISIBLE_MIN_X) / VIEW_WIDTH;
	double ymult = double(VISIBLE_MAX_Y - VISIBLE_MIN_Y) / VIEW_HEIGHT;
	
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++)
		glVertex3f(gx + xmult*(points[i].dx-.5), gy + ymult*(points[i].dy-.5), gz);
	glEnd();
}

static void drawLineFromBaseXY(double x, double y, Point points[], int nPoints)
{
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	double xmult = double(VISIBLE_MAX_X - VISIBLE_MIN_X) / VIEW_WIDTH;
	double ymult = double(VISIBLE_MAX_Y - VISIBLE_MIN_Y) / VIEW_HEIGHT;
	
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < nPoints; i++)
		glVertex3f(gx + xmult*(points[i].dx-.5), gy + ymult*(points[i].dy-.5), gz);
	glEnd();
}

static void drawPolyFromBaseXYFlat(double x, double y, Point points[], int nPoints, double scale = 1.0)
{
	glBegin(GL_POLYGON);
	for (int i = 0; i < nPoints; i++)
	{
		double gx, gy, gz;
		convertToGlutCoords(x+points[i].dx*scale-.5,y+ points[i].dy*scale-.5, gx, gy, gz);
		glVertex3f(gx, gy, gz);
	}
	glEnd();
}

static void drawPrompt(string mainMessage, string secondMessage)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f (1.0, 1.0, 1.0);
	glLoadIdentity ();
	outputStrokeCentered(1, -5, mainMessage.c_str());
	outputStrokeCentered(-1, -5, secondMessage.c_str());
	glutSwapBuffers();
}

static void drawScoreAndLives(string gameStatText)
{
	static int RATE = 1;
	static GLfloat rgb[3] = { .6, .6, .6 };
	for (int k = 0; k < 3; k++)
	{
		rgb[k] += (-RATE + rand() % (2*RATE+1)) / 100.0;
		if (rgb[k] < .6)
			rgb[k] = .6;
		else if (rgb[k] > 1.0)
			rgb[k] = 1.0;
	}
	glColor3f(rgb[0], rgb[1], rgb[2]);
	outputStrokeCentered(SCORE_Y, SCORE_Z, gameStatText.c_str());
}

static void drawPlayer(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x,y);
	
	glPushMatrix();
	
	glColor3f(1, .1, .1);
	glLineWidth(2);	

	Point head[] = { {.5, 1.0}, {.40,.90}, {.5,.85},{.60,.95},{.5,1.0}};
	drawLineFromBaseXY(x, y, head, sizeof(head)/sizeof(Point));

	glColor3f(.1, 1.0, .5);
	Point torso[] = { {.5,.9},{.5,.4}};
	drawLineFromBaseXY(x, y, torso, sizeof(torso)/sizeof(Point));

	glColor3f(1.0, 1.0, 0);
	double armDelta[] = {.25,.2,.1,0,-.1,-.2,-.25,-.2,-.1,0,.1,.2};
	int armIndex = ((go->getAnimationNumber()/10) % (sizeof(armDelta)/sizeof(double)));

	Point arms[] = { {.3,.7+armDelta[armIndex]},{.7,.7-armDelta[armIndex]}};
	drawLineFromBaseXY(x, y, arms, sizeof(arms)/sizeof(Point));

	double legDelta[] = {.1,0,-.1,0};
	int legIndex = ((go->getAnimationNumber()/10) % (sizeof(legDelta)/sizeof(double)));
	Point legs[] = { {.3,0+legDelta[legIndex]}, {.5,.4}, {.7,0-legDelta[legIndex]} };
	drawLineFromBaseXY(x, y, legs, sizeof(legs)/sizeof(Point));

	glPopMatrix();
}

static void drawComplexZumi(GraphObject* go)
{

	//

	double x, y;

	glPushMatrix();

	go->getAnimationLocation(x,y);
	glLineWidth(1);
		 
	glColor3f (1.0, 0.2, 0.3);

	double scale = 1;

	Point scorpbase[] = {{.2,.4},{.6,.35},{.6,.65},{.2,.6}};
	drawPolyFromBaseXYFlat(x,y,scorpbase,sizeof(scorpbase)/sizeof(Point),scale );

	Point scorptail1[] = {{.2,.4},{-.1,.7},{.2,.6}};
	drawPolyFromBaseXYFlat(x,y,scorptail1,sizeof(scorptail1)/sizeof(Point),scale );

	Point scorptail2[] = {{-.12,.65},{.1,.8},{-.08,.65}};
	drawPolyFromBaseXYFlat(x,y,scorptail2,sizeof(scorptail2)/sizeof(Point),scale );

	Point claw1a[] = {{.6,.35},{.8,.2},{.6,.4}};
	drawPolyFromBaseXYFlat(x,y,claw1a,sizeof(claw1a)/sizeof(Point),scale );

	Point claw1b[] = {{.8,.2},{.7,.35},{.8,.25}};
	drawPolyFromBaseXYFlat(x,y,claw1b,sizeof(claw1b)/sizeof(Point),scale );

	Point claw2a[] = {{.6,.65},{.8,.8},{.6,.6}};
	drawPolyFromBaseXYFlat(x,y,claw2a,sizeof(claw2a)/sizeof(Point),scale );

	Point claw2b[] = {{.8,.8},{.7,.65},{.8,.75}};
	drawPolyFromBaseXYFlat(x,y,claw2b,sizeof(claw2b)/sizeof(Point),scale );

	glPopMatrix();
}

static void drawSimpleZumi(GraphObject* go)
{
	glPushMatrix();

	double x, y;
	go->getAnimationLocation(x,y);
	double wx,wy,wz;
	convertToGlutCoords(x,y,wx,wy,wz);
	glTranslatef(wx,wy-.4,wz);

	double legDelta[] = { .1,.05,0,-.05,-.1,-.05,0,.05};
	int legIndex = (go->getAnimationNumber()/10) % (sizeof(legDelta)/sizeof(double));

	glLineWidth(2);
	glColor3f (.25, 1, 1);
	glTranslatef(0,.3,0);
	for (int i=0;i<4;i++)
	{
		glBegin(GL_LINE_STRIP);
		glVertex3f(-.1,.05*i,0);
		glVertex3f(-.3,.15*i-.2+legDelta[legIndex],0);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex3f(.1,.05*i,0);
		glVertex3f(.3,.15*i-.2-legDelta[legIndex],0);
		glEnd();
	}

	glPopMatrix();

	glPushMatrix();

	glTranslatef(wx,wy,wz);
	glColor3f (1, .25, 0.5);
	glLineWidth(1);

	glTranslatef(0,.15,0);
	glutWireSphere((VISIBLE_MAX_X-VISIBLE_MIN_X)/(double)VIEW_WIDTH/6,10,5);
	glTranslatef(0,-.20,0);
	glutWireSphere((VISIBLE_MAX_X-VISIBLE_MIN_X)/(double)VIEW_HEIGHT/3,10,5);
	glPopMatrix();

}



static void drawExit(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x, y);
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	glPushMatrix();
	
	glTranslatef(gx, gy, gz);
	glColor3f(1.0, 0.0, 0.0);
	
	glLineWidth(1);
	
	glBegin( GL_POLYGON );
	double r = .3;
	for( float i = 0; i < 10; i++)
	{
		double theta = 2*PI * i / 10.0;
		double cx = cos(theta) * r;
		double cy = sin(theta) * r;
		glVertex3f(cx, cy, -.01);
	}
	glEnd();
	
	glPopMatrix();
	
	glPushMatrix();

	x -= .225;
	y -= .15;
	//glLineWidth(1);
	convertToGlutCoords(x, y, gx, gy, gz);
	glColor3f(.0, 0.2, 1.0);	
	outputStroke(gx, gy, gz, 1.25, "EXIT");

	glPopMatrix();
}

static void drawGoodie(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x, y);
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	double brightness = go->getBrightness();
	
	glPushMatrix();
	
	glTranslatef(gx, gy, gz);
	glColor3f(0.0*brightness, 0.0*brightness, 1.0*brightness);
	
	glLineWidth(1);

	glBegin( GL_POLYGON );
	double r = .2;
	for( float i = 0; i < 10; i++)
	{
		double theta = 2*PI * i / 10.0;
		double cx = cos(theta) * r;
		double cy = sin(theta) * r;
		glVertex3f(cx, cy, -.01);
	}
	glEnd();
	
	glPopMatrix();
	
	glPushMatrix();
	
	glColor3f(1.0*brightness, 0.2*brightness, 0.3*brightness);
	
	char goodieChar[2] = "";
	switch (go->getID())
	{
		case IID_EXTRA_LIFE_GOODIE: goodieChar[0] = 'X'; break;
		case IID_WALK_THRU_GOODIE:  goodieChar[0] = 'W'; break;
		case IID_INCREASE_SIMULTANEOUS_SPRAYER_GOODIE:   goodieChar[0] = 'S'; break;
	}
	
	outputStroke(gx, gy, gz, 1, goodieChar);

	glPopMatrix();
}

static void drawBugSprayer(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x, y);
	double gx, gy, gz;
	convertToGlutCoords(x, y, gx, gy, gz);
	
	glPushMatrix();
	
	glTranslatef(gx, gy, gz);
	glColor3f(1.0, 0.0, 0.0);
	
	glLineWidth(1);
	
	glBegin(GL_LINE_STRIP);
	double r = .3;
	for( float i = 0; i <= 10; i++)
	{
		double theta = 2*PI * i / 10.0;
		double cx = cos(theta) * r;
		double cy = sin(theta) * r;
		glVertex3f(cx, cy, -.01);
	}
	glEnd();

	glPopMatrix();

	glColor3f(0.0, 1.0, 1.0);
	double secondHand = 2*3.14159*((go->getAnimationNumber()/10) % 12) / 12.0;
	double hx = cos(secondHand) * .5;
	double hy = sin(secondHand) * .8;
	Point coords[] = { {0,0},{hx,hy}};
	drawLineFromBaseXY(x+.25, y+.25, coords, sizeof(coords)/sizeof(Point));
	

	
	glPushMatrix();
}


static void drawBugSpray(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x, y);
	
	glPushMatrix();
	
	double length;

	length = (rand() % 100) / 100.0;
	
	glLineWidth(1);
	
	for (int i = 0; i < 10; i++)
	{
		double theta = 2*PI * (rand() % 1000) / 1000.0;
		double dx = cos(theta) * length;
		double dy = sin(theta) * length;
		Point line[] = { { .5, .5 }, { dx+.5, dy+.5 } };
		glColor3f(rand() % 100 / 100.0, rand() % 100 / 100.0, rand() % 100 / 100.0);
		drawLineFromBaseXY(x, y, line, sizeof(line)/sizeof(line[0]));
	}
	glPopMatrix();
}

static void drawPermaBrick(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x, y);
	
	glPushMatrix();
	
	glLineWidth(1);
	
	for (double distFromEdge=0.5;distFromEdge >=0;distFromEdge -= .1)
	{
		glColor3f (0.5+distFromEdge, 1.0, 0.0);
		Point square[] = { { distFromEdge, distFromEdge }, { 1-distFromEdge, distFromEdge }, 
		{ 1-distFromEdge, 1-distFromEdge }, { distFromEdge, 1-distFromEdge }, {distFromEdge,distFromEdge} };
		drawPolyFromBaseXY(x, y, square, sizeof(square)/sizeof(square[0]));
	}
		
	glPopMatrix();	
}

static void drawDestroyableBrick(GraphObject* go)
{
	double x, y;
	go->getAnimationLocation(x, y);
	
	glPushMatrix();
	
	glLineWidth(1);
	
	for (double distFromEdge=0.0;distFromEdge < .5;distFromEdge += .1)
	{
		glColor3f (0.5+distFromEdge, 0, 1.0);
		Point square[] = { { distFromEdge, distFromEdge }, { 1-distFromEdge, distFromEdge }, 
		{ 1-distFromEdge, 1-distFromEdge }, { distFromEdge, 1-distFromEdge }, {distFromEdge,distFromEdge} };
		drawLineFromBaseXY(x, y, square, sizeof(square)/sizeof(square[0]));
	}
		
	glPopMatrix();	
}
