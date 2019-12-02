/**
 *
 *
 * @date: 3/22/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#include "RikaGame.h"

bool RikaGame::init() {
	//time
	glfwSetTime(0);
	lastTick = 0;
	//window
	gameWindow = new GameWindow();
	bool gwInit = gameWindow->init(this);
	if (!gwInit) {
		InputHandler::init(gameWindow);
		playerInputNumber = InputHandler::registerPlayer((char*)"Player 1");
		InputHandler::PlayerInput* pli = InputHandler::getPlayerInput(playerInputNumber);
		pli->keyboard.A = GLFW_KEY_A;
		pli->keyboard.W = GLFW_KEY_W;
		pli->keyboard.S = GLFW_KEY_S;
		pli->keyboard.D = GLFW_KEY_D;
		pli->keyboard.Z = GLFW_KEY_Z;
		pli->keyboard.X = GLFW_KEY_X;
		pli->keyboard.C = GLFW_KEY_C;
		pli->keyboard.Shift = GLFW_KEY_LEFT_SHIFT;
	}
	return gwInit;
}

void RikaGame::launch() {
	gameRunning = true;
	while (gameRunning) {
		time();
		gameLoop();
	} 
}

void RikaGame::time() {
	//delta:
	currentTick = glfwGetTime();
	tickDelta = currentTick - lastTick;
	lastTick = currentTick;
	//accumulators:
	accumulatorUPS += tickDelta;
	accumulatorFPS += tickDelta;
	//report:
	reportTickTimer += tickDelta;
	tickCounter++;
	while (reportTickTimer > 1) {
		reportTickTimer -= 1;
		printf("GameLoop: (delta: %f) tps:%d fps:%d ups:%d\n", tickDelta, tickCounter, fpsCounter, upsCounter);
		tpsMean = tickCounter;
		fpsMean = fpsCounter;
		upsMean = upsCounter;
		tps_records[tps_index] = (float)tpsMean;
		fps_records[fps_index] = (float)fpsMean;
		ups_records[ups_index] = (float)upsMean;
		if (++fps_index >= 30)
			fps_index = 0;
		if (++tps_index >= 30)
			tps_index = 0;
		if (++ups_index >= 30)
			ups_index = 0;
		tickCounter = 0;
		fpsCounter = 0;
		upsCounter = 0;
	}
}

void RikaGame::imguiDiag() {
	ImGui::Begin("Performance");
	ImVec4 g = ImVec4(0, 1, 0, 1);
	ImVec4 y = ImVec4(1, 1, 0, 1);
	ImVec4 r = ImVec4(1, 0, 0, 1);
	ImGui::TextColored(ImVec4(1, 0.5, 1, 1), "Game Loop");
	ImGui::TextColored(fpsMean >= 60 ? g : fpsMean > 50 ? y : r, "FPS: %d", fpsMean);
	ImGui::TextColored(upsMean >= 50 ? g : upsMean > 45 ? y : r, "UPS: %d", upsMean);
	ImGui::TextColored(tpsMean >= 60 ? g : tpsMean > 50 ? y : r, "TPS: %d", tpsMean);

	int modes[3] = { 1,2,3 };
	ImGui::PlotLines("", fps_records, IM_ARRAYSIZE(fps_records), fps_index, "FPS", 0, 120, ImVec2(0, 80));
	ImGui::PlotLines("", ups_records, IM_ARRAYSIZE(ups_records), fps_index, "UPS", 0, 120, ImVec2(0, 80));
	ImGui::PlotLines("", tps_records, IM_ARRAYSIZE(tps_records), fps_index, "TPS", 0, 120, ImVec2(0, 80));
	ImGui::End();
}

void RikaGame::gameLoop() {
	if (currentState >= 0) {
		glfwPollEvents();

		//update:
		while (accumulatorUPS >= SIM_UPD_STEP) {
			upsCounter++;
			simTime += SIM_UPD_STEP;
			accumulatorUPS -= SIM_UPD_STEP;
			gameStates[currentState]->update(0);
		}

		//render:
		while (accumulatorFPS >= SIM_FPS_STEP) {
			while (accumulatorFPS >= SIM_FPS_STEP) {
				accumulatorFPS -= SIM_FPS_STEP;
			}
			double tillNextUPD = accumulatorUPS / SIM_UPD_STEP;
			fpsCounter++;
			gameStates[currentState]->render(0);
			glfwSwapBuffers(gameWindow->getGLFWWindow());
		}
	}
}

void RikaGame::closeCallback() {
	gameRunning = false;
	gameWindow->closeWindow();
}


GameWindowInterface* RikaGame::window() {
	return gameWindow;
}

void RikaGame::cleanUp() {
	gameWindow->closeWindow();
	glfwTerminate();
}


int RikaGame::insertGameState(RikaGameState* state) {
	gameStates.push_back(state);
	state->init(this);
	return (int)gameStates.size() - 1;
}

//getters

const char* RikaGame::getWindowIconPath() {
	return globals::GAME_ICON_PATH;
}