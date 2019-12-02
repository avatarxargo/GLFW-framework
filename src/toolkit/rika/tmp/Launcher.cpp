/**
 * Testing launcher for the OpenGL engine.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "Launcher.h"

using namespace std;

void MyMessageFunc(FREE_IMAGE_FORMAT fif, const char *message) {
	cout << "\n*** " << message << " ***\n";
}

void initFreeImage() {
	#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
	#endif // FREEIMAGE_LIB
	// initialize FreeImage error handler
	FreeImage_SetOutputMessage(MyMessageFunc);
	cout << "FreeImage " << FreeImage_GetVersion() << "\n";
	cout << FreeImage_GetCopyrightMessage() << "\n\n";
	printf("%s\n", globals::LINE);
}

void clearFreeImage() {
	#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
	#endif // FREEIMAGE_LIB
}

//Linker > System > SubSystem > Window / Console
//Linker > Advanced > EntryPoint: mainCRTStartup / "" 
int main(int argc, char* argv[]) {
	printf("\nRika Engine ver: %s\n%s\n%s%s\n\n", globals::VERSION, globals::LINE, globals::CREDITS, globals::LINE);
	// Load global libraries:
	initFreeImage();
	//
	game = new RikaGame();
	bool resInit = game->init();
	game->setCurrentState(game->insertGameState(new TstState()));
	game->launch();
	//
	//system("pause");
	clearFreeImage();
	return 0;
}