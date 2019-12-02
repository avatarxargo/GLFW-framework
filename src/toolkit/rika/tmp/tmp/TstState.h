/**
 * Testing state for experimentation 
 *
 * @date: 3/26/2019
 * @author: Hrusa
 * @email: hrusa@pansyn.com
 */

#ifndef TST_STATE_H
#define TST_STATE_H


#include "core/Game/RikaGameState.h"
#include <stdio.h>
#include <string>
#include "display/UI/UISprite.h"
#include "display/Characters/CharacterTools.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "../../../ImGuiFileBrowser.h"

class TstState : public RikaGameState {
public:
	UISprite* sprite;
	UIShapes::UICircle circle;
	UIHandler* uiHandler;
	void drawUI();
	CharacterTools::Skeleton* skeleton;
	CharacterTools::BezierShape* shape;
	virtual void init(RikaGameInterface* game);
	virtual void update(long delta);
	virtual void render(double ratio);
	virtual void load(RikaGameInterface* game) {/*TODO: implement*/}
	virtual void unload(RikaGameInterface* game) {/*TODO: implement*/}
	virtual void fullUnload(RikaGameInterface* game) {/*TODO: implement*/}
};

#endif
