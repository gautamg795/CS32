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
#include "GameConstants.h"
#include <cstdlib>
#include <ctime>
using namespace std;

class GameWorld;

GameWorld* createStudentWorld();

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);

    int testParams[NUM_TEST_PARAMS];
    for (int i = 0; i < NUM_TEST_PARAMS; i++)
        testParams[i] = (i+1 < argc) ? atoi(argv[i+1]) : 0;

    srand(static_cast<unsigned int>(time(NULL)));
    GameWorld* gw = createStudentWorld();
    Game().run(gw, testParams, "Bug Blast");
}