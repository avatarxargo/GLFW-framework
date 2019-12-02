/**
 * The overarching container for the major pieces of the game.
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef RIKAGAME_H
#define RIKAGAME_H

#include "RikaGameInterface.h"
#include "Game/RikaGameState.h"
#include "GameWindow.h"
#include "InputHandler.h"
#include "Constants/Constants.h"
#include <vector>

using namespace std;

class RikaGame: public RikaGameInterface {
private:
	//tick reffers to the low level engine tick of the loop checking both UPS and FPS.
	//time
	double lastTick = 0;
	double currentTick = 0;
	double tickDelta = 0;
	double reportTickTimer = 0;
	double accumulatorFPS = 0;
	double accumulatorUPS = 0;
	int tickCounter = 0;
	int fpsCounter = 0;
	int upsCounter = 0;
	int tpsMean = 0;
	int fpsMean = 0;
	int upsMean = 0;
	int tps_index = 0;
	int fps_index = 0;
	int ups_index = 0;
	// ===== Game Loop Variables =====
	//const:
	double simTime = 0.0;
	//const long NANOS_IN_SECOND = 1000000000L;
	const double SECOND_DURATION = 1.0;
	double UPS = 50;
	double FPS = 60;
	double SIM_UPD_STEP = SECOND_DURATION / UPS;
	double SIM_FPS_STEP = SECOND_DURATION / FPS;
	double FRAME_MAX_DURATION = (SIM_FPS_STEP<SIM_UPD_STEP) ? SIM_FPS_STEP : SIM_UPD_STEP;
	double SLEEP_MS = 1;
	//times:
	//options
	bool opt_vSynch = false;
	bool opt_park = false;
	int opt_parkTime = 50;
	bool opt_timer = false;
	int opt_timer_time = 50;
	//
	bool gameRunning = false;
	int currentState = -1;
	vector<RikaGameState*> gameStates;
	GameWindow* gameWindow;
	int playerInputNumber = 0;
public:
	float tps_records[30];
	float fps_records[30];
	float ups_records[30];
	/**
	 * creates a new gameWindow and sets up rendering context.
	 * @return true if the initialization failed.
	 */
	bool init();
	/**
	 * Clean up before closing the game process.
	 */
	void cleanUp();
	/**
	 *  Start the game loop.
	 */
	void launch();
	/**
	 *	Moves the timeline forward and then calls upon the currently active state functions.
	 */
	void gameLoop();
	/**
	 *	Moves the timeline forward, calculates delta and interpolation values.
	 */
	void time();

	/**
	 *	Displays a statistic tracking window.
	 */
	void imguiDiag();

	/**
	 * Adds a new state into the pool of game states.
	 * @return the index in the list of states.
	 */
	int insertGameState(RikaGameState* state);

	// ========================================= Getters / Setters =========================================

	int getCurrentState() {
		return currentState;
	}

	void setCurrentState(int _currentState) {
		if (_currentState >= gameStates.size() || _currentState < 0) {
			fprintf(stderr, "Atempted to set game state: %d (out of bounds for: %zd)\n",_currentState, gameStates.size());
			return;
		}
		currentState = _currentState;
	}

	// ========================================= Interfaced =========================================

	//virtual mapped classes:
	void closeCallback();
	GameWindowInterface* window();
	const char* getWindowIconPath();
};

#endif;