/**
 * Defines a single state within the game.
 *
 * @date: 3/26/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef RIKAGAMESTATE_H
#define RIKAGAMESTATE_H

#include "../RikaGameInterface.h"
#include "../InputHandler.h"

class RikaGameState {
private:
public:
	RikaGameInterface* game;
	/**Called after the state has been added into the game state pool.*/
	virtual void init(RikaGameInterface* game) = 0;

	/**Preloads any necessary assets*/
	virtual void load(RikaGameInterface* game) = 0;

	/**Clears assets from memory, might be reloaded later*/
	virtual void unload(RikaGameInterface* game) = 0;

	/**Clears all assets from memory. Nothing is left.*/
	virtual void fullUnload(RikaGameInterface* game) = 0;

	/**Called regularly for updates.*/
	virtual void update(long delta) = 0;

	/**Called regularly for display.*/
	virtual void render(double ratio) = 0;

	/**Stores the pointer to the main game.*/
	void setRikaGame(RikaGameInterface* _game);

	/**Called when the logic initiates transition from this state to another one.*/
	void leaveStart(RikaGameState* other);

	/**Called when the logic ends transition from this state to another one.*/
	void leaveEnd(RikaGameState* other);

	/**Called when the logic initiates transition to this state from another one.*/
	void enterStart(RikaGameState* other);

	/**Called when the logic ends transition to this state from another one.*/
	void enterEnd(RikaGameState* other);
};

#endif
