/**
 * Defines an includable public function abstraction of the main game. (So that we can then safely pass reference to the main game to all its children)
 *
 * @date: 3/26/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef RIKAGAMEINTERFACE_H
#define RIKAGAMEINTERFACE_H

#include "GameWindowInterface.h"

class RikaGameInterface {
public:
	virtual	void closeCallback() = 0;
	virtual	GameWindowInterface* window() = 0;
	virtual	const char* getWindowIconPath() = 0;
	virtual void imguiDiag() = 0;
};

#endif