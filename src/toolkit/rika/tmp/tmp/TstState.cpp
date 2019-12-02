#include "TstState.h"

float a = 0;

void TstState::update(long delta) {
	a += 0.1f;
	float scl = 10;
	if (InputHandler::keyPressed(0, InputHandler::GameInput::LEFT)) {
		//sprite->position.x -= 10;
		game->window()->getCamera()->translateRelative(glm::vec3(-scl,0,0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::RIGHT)) {
		//sprite->position.x += 10;
		game->window()->getCamera()->translateRelative(glm::vec3(scl, 0, 0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::UP)) {
		//sprite->position.y += 10;
		game->window()->getCamera()->translateRelative(glm::vec3(0, scl, 0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::DOWN)) {
		//sprite->position.y -= 10;
		game->window()->getCamera()->translateRelative(glm::vec3(0, -scl, 0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::B)) {
		/*sprite->size.x -= 10;
		sprite->size.y -= 10;*/
		//game->window()->getCamera()->translateRelative(glm::vec3(0, 0, scl));
		game->window()->getCamera()->rotateRelative(glm::vec3(0, scl, 0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::A)) {
		/*sprite->size.x += 10;
		sprite->size.y += 10;*/
		//game->window()->getCamera()->translateRelative(glm::vec3(0, 0, -scl));
		game->window()->getCamera()->rotateRelative(glm::vec3(0, -scl, 0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::C)) {
		game->window()->getCamera()->rotateRelative(glm::vec3(scl, 0, 0));
	}
	if (InputHandler::keyPressed(0, InputHandler::GameInput::D)) {
		game->window()->getCamera()->rotateRelative(glm::vec3(-scl, 0, 0));
	}

	//Store these so that the begin bools don't get corrupted
	bool clearDrag = false;
	bool isDrag = InputHandler::isDragMode();
	bool isDragBegin = InputHandler::isDragModeBegin();

	// Generalized handler
	if (uiHandler->dragTest(isDrag, isDragBegin, *InputHandler::getMousePos(), *InputHandler::getDragDelta(), InputHandler::getSrollDelta())) {
		clearDrag = true;
	}

	//Solo sprite placeholder
	if (isDragBegin && sprite->testOverlap(InputHandler::getMousePos())) {
		sprite->highlight = true;
	}
	if (!isDrag) {
		sprite->highlight = false;
	}
	if (sprite->highlight) {
		sprite->position += *InputHandler::getDragDelta();
		InputHandler::clearDragDelta();
	}

	//clear deltas
	if (clearDrag) {
		InputHandler::clearDragDelta();
	}
	InputHandler::clearSrollDelta();
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

bool show_demo_window = false;

void TstState::drawUI() {
	//UI overlay
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	ImGui::Begin("Sprite Editor");
	HelpMarker("This screen allows you to edit Sprites in the scene.");
	//
	if (ImGui::TreeNode("FBO Buffers"))
	{
		float thumbscl = 2.3f;
		ImGui::Columns(2);
		ImGui::Text("Sprites");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().fboHandler->fbotex1, ImVec2(120 * thumbscl, 72 * thumbscl), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::NextColumn();
		ImGui::Text("Lineart");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().fboHandler->fbotex2, ImVec2(120 * thumbscl, 72 * thumbscl), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Text("Normals");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().fboHandler->fbotex3, ImVec2(120 * thumbscl, 72 * thumbscl), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::Columns(1);
		ImGui::TreePop();
	}
	ImGui::Separator();
	//
	if (ImGui::TreeNode("Texture Picker"))
	{
		static ImGui::FileRefInfo myfile1;
		static ImGui::FileRefInfo myfile2;
		static ImGui::FileRefInfo myfile3;
		static ImGui::FileRefInfo myfile4;
		ImGui::Columns(4);
		ImGui::Text("Texture 1:");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().spriteRenderer->tex1.getID(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::FileBrowser("Look up a texture 1", "", &myfile1)) {
			game->window()->getShaders().spriteRenderer->tex1 = *myfile1.preview;
		}
		ImGui::NextColumn();
		ImGui::Text("Texture 2:");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().spriteRenderer->tex2.getID(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::FileBrowser("Look up a texture 2", "", &myfile2)) {
			game->window()->getShaders().spriteRenderer->tex2 = *myfile2.preview;
		}
		ImGui::NextColumn();
		ImGui::Text("Texture 3:");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().spriteRenderer->tex3.getID(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::FileBrowser("Look up a texture 3", "", &myfile3)) {
			game->window()->getShaders().spriteRenderer->tex3 = *myfile3.preview;
		}
		ImGui::NextColumn();
		ImGui::Text("Texture 4:");
		ImGui::Image((void*)(intptr_t)game->window()->getShaders().spriteRenderer->tex4.getID(), ImVec2(50, 50), ImVec2(0, 1), ImVec2(1, 0));
		if (ImGui::FileBrowser("Look up a texture 4", "", &myfile4)) {
			game->window()->getShaders().spriteRenderer->tex4 = *myfile4.preview;
		}
		ImGui::Columns(1);
		ImGui::TreePop();
	}
	//
	{
		//Sprites
		ImGui::Separator();
		if (ImGui::TreeNode("Sprites")) {
			if (ImGui::TreeNode("Sample Sprite"))
			{
				ImGui::Columns(2);
				ImGui::SliderFloat2("Position", (float*)&sprite->position, 0, 1000);
				ImGui::NextColumn();
				ImGui::SliderFloat2("Size", (float*)&sprite->size, 0, 2000);
				
				ImGui::NextColumn();
				ImGui::SliderFloat2("UV Start", (float*)&sprite->uvstart, 0, 1);
				ImGui::NextColumn();
				ImGui::SliderFloat2("UV Size", (float*)&sprite->uvsize, 0, 3);
				ImGui::NextColumn();


				if (ImGui::Button("Reset")) {
					sprite->position = glm::vec2(200, 200);
					sprite->size = glm::vec2(100, 100);
					sprite->uvstart = glm::vec2(0, 0);
					sprite->uvsize = glm::vec2(1, 1);
					sprite->texid = 0;
				}
				ImGui::SameLine();
				ImGui::SliderInt("Texture", (int*)&sprite->texid, 0, 3);
				ImGui::NextColumn();
				ImGui::SliderFloat("Angle", (float*)&sprite->angle, -3.14159, 3.14159);

				ImGui::NextColumn();
				ImGui::ColorPicker4("Stencil Color", &sprite->color[0]);

				ImGui::Columns(1);
				ImGui::TreePop();
			}
			//
			for (int i = 0; i < uiHandler->sprites.size(); ++i) {
				std::string name = "sprite ";
				name.append(std::to_string(i));
				if (ImGui::TreeNode(name.c_str())) {
					if (ImGui::Button("X")) {
						//uiHandler->sprites[i]->highlight.w = 1;
					}
					ImGui::Columns(2);
					ImGui::SliderFloat2("Position", (float*)&uiHandler->sprites[i]->position, 0, 1000);
					ImGui::NextColumn();
					ImGui::SliderFloat2("Size", (float*)&uiHandler->sprites[i]->size, 0, 2000);
					ImGui::NextColumn();
					ImGui::SliderFloat2("UV Start", (float*)&uiHandler->sprites[i]->uvstart, 0, 1);
					ImGui::NextColumn();
					ImGui::SliderFloat2("UV Size", (float*)&uiHandler->sprites[i]->uvsize, 0, 3);
					ImGui::NextColumn();

					if (ImGui::Button("Reset")) {
						uiHandler->sprites[i]->position = glm::vec2(200, 200);
						uiHandler->sprites[i]->size = glm::vec2(100, 100);
						uiHandler->sprites[i]->uvstart = glm::vec2(0, 0);
						uiHandler->sprites[i]->uvsize = glm::vec2(1, 1);
						uiHandler->sprites[i]->texid = 0;
					}
					ImGui::SameLine();
					ImGui::SliderInt("Texture", (int*)&uiHandler->sprites[i]->texid, 0, 3);
					ImGui::NextColumn();
					ImGui::SliderFloat("Angle", (float*)&uiHandler->sprites[i]->angle, -3.14159, 3.14159);

					ImGui::NextColumn();
					ImGui::ColorPicker4("Stencil Color", &uiHandler->sprites[i]->color[0]);
					ImGui::TreePop();
				}
			}
			//
			ImGui::TreePop();

			if (ImGui::Button("+")) {
				UISprite* s = new UISprite(0,glm::vec2(200, 200), glm::vec2(200, 200));
				uiHandler->addShape(s);
			}
		}
		//Circles
		if (ImGui::TreeNode("Nodes")) {
			for (int i = 0; i < uiHandler->circles.size(); ++i) {
				std::string name = "ring ";
				name.append(std::to_string(i));
				if (ImGui::TreeNode(name.c_str())) {
					if (ImGui::Button("X")) {
						uiHandler->circles[i]->highlight.w = 1;
					}
					ImGui::Columns(2);
					ImGui::ColorPicker4("Color", (float*)&uiHandler->circles[i]->color);
					ImGui::NextColumn();
					ImGui::SliderFloat2("Position", (float*)&uiHandler->circles[i]->position, 0, 2000);
					ImGui::SliderFloat("Radius1", &uiHandler->circles[i]->radius1, 0, 1000, "Radius: %.3f");
					ImGui::SliderFloat("Radius2", &uiHandler->circles[i]->radius2, 0, 500, "Thickness: %.3f");
					ImGui::SliderInt("Z", &uiHandler->circles[i]->z, 0, 50, "z-position: %d");
					ImGui::Columns(1);
					ImGui::TreePop();
				}
			}
			ImGui::TreePop();

			if (ImGui::Button("+")) {
				UIShapes::UICircle* c = new UIShapes::UICircle();
				UIShapes::createCircle(c, glm::vec2(0, 0), 100, 5, glm::vec4(1, 0, 0, 1));
				uiHandler->addShape(c);
			}
		}
	}
	ImGui::Separator();
	if (ImGui::TreeNode("Camera Control")) {
		Camera* cam = game->window()->getCamera();
		static float campos[3];
		static float camrot[3];
		static float fov = 3.14159f / 2;
		static float camposlast[3];
		for (int i = 0; i < 3; ++i)
			camposlast[i] = campos[i];
		ImGui::SliderFloat3("Position", campos, -5000, 5000);
		ImGui::SliderFloat3("Rotation", camrot, -3.14159, 3.14159);
		ImGui::SliderFloat("FoV", &fov, 0.1, 3.14159);
		if (ImGui::ButtonEx("Reset")) {
			campos[0] = 0;
			campos[1] = 0;
			campos[2] = 0;
			camrot[0] = 0;
			camrot[1] = 0;
			camrot[2] = 0;
			fov = 3.14159f / 2;
		}
		if (ImGui::ButtonEx("Side")) {
			campos[0] = -600;
			campos[1] = 1500;
			campos[2] = 1200;
			camrot[0] = -0.6f;
			camrot[1] = -0.6f;
			camrot[2] = 0;
			fov = 0.9;
		}
		static int projmode = 0;
		ImGui::SliderInt("Projection Mode", &projmode, 0, 1);
		game->window()->getShaders().spriteRenderer->cammode = projmode;
		//cam->translateRelative(glm::vec3(camposlast[0] - campos[0], camposlast[1] - campos[1], camposlast[2] - campos[2]));
		cam->set(glm::vec3(campos[0], campos[1], campos[2]), glm::vec3(camrot[0], camrot[1], camrot[2]));
		cam->setFoV(fov);
		ImGui::TreePop();
	}
	ImGui::End();

	game->imguiDiag();

	ImGui::Render();
}

void TstState::render(double ratio) {
	// 1st Pass : Render into a FBO
	FBOHandler* fboHandler = game->window()->getShaders().fboHandler;
	fboHandler->bind();
	//
	SpriteRenderer* spriteRenderer = game->window()->getShaders().spriteRenderer;
	spriteRenderer->clearVBO();
	/*for (int i = uiHandler->sprites.size() - 1; i > uiHandler->sprites.size()-501; --i) {
		uiHandler->sprites[i]->position.x = glm::cos(a + 20 * i / 1000.0) * 300 + 300;
		uiHandler->sprites[i]->position.y = glm::sin(a + 20 * i / 3000.0) * 100 + 200;
	}*/
	spriteRenderer->insertHandler(uiHandler);
	spriteRenderer->insertSprite(sprite);
	spriteRenderer->pushToDevice();
	//
	CircleRenderer* circleRenderer = game->window()->getShaders().circleRenderer;
	circleRenderer->clearVBO();
	circleRenderer->insertHandler(uiHandler);
	circleRenderer->pushToDevice();
	//
	BoneRenderer* boneRenderer = game->window()->getShaders().boneRenderer;
	boneRenderer->clearVBO();
	boneRenderer->insertSkeleton(skeleton);
	boneRenderer->pushToDevice();
	//
	BezierRenderer* bezierRenderer = game->window()->getShaders().bezierRenderer;
	bezierRenderer->clearVBO();
	bezierRenderer->insertShape(shape);
	bezierRenderer->pushToDevice();
	//
	game->window()->render1st(0);
	drawUI();
	game->window()->render2nd(0);
}

void TstState::init(RikaGameInterface* game) {
	setRikaGame(game);
	// Imgui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::initFB(); //my own init
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(game->window()->getGLFWWindow(), true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
	//
	uiHandler = new UIHandler();
	UIShapes::UICircle* c1 = new UIShapes::UICircle();
	UIShapes::UICircle* c2 = new UIShapes::UICircle();
	UIShapes::UICircle* c3 = new UIShapes::UICircle();
	UIShapes::createCircle(c1, glm::vec2(500, 500), 100, 5, glm::vec4(0.8, 0.2, 0.2, 1));
	UIShapes::createCircle(c2, glm::vec2(800, 500), 200, 5, glm::vec4(0.8, 0.2, 0.2, 1));
	UIShapes::createCircle(c3, glm::vec2(400, 400), 150, 10, glm::vec4(0.2, 0.8, 0.2, 1));
	uiHandler->addShape(c1);
	uiHandler->addShape(c2);
	uiHandler->addShape(c3);
	skeleton = new CharacterTools::Skeleton(uiHandler);
	CharacterTools::Bone* firstBone = skeleton->addBone(0);
	skeleton->addBone(firstBone->child);
	//
	shape = new CharacterTools::BezierShape(uiHandler);
	shape->addNode(new CharacterTools::BezierNode(glm::vec2(200, 100), glm::vec2(-40, 0), glm::vec2(40, 0)));
	shape->addNode(new CharacterTools::BezierNode(glm::vec2(500, 100), glm::vec2(-40, 0), glm::vec2(40, 0)));
	shape->addNode(new CharacterTools::BezierNode(glm::vec2(300, 150), glm::vec2(-40, 0), glm::vec2(40, 0)));
	//
	sprite = new UISprite(0, glm::vec2(0, 0), glm::vec2(100, 100));
	UISprite* bg = new UISprite(1, glm::vec2(1250, 550), glm::vec2(2500, 1100));
	bg->selectible = false;
	bg->color = glm::vec4(0, 1, 1, 1);
	uiHandler->addShape(bg);
	uiHandler->addShape(new UISprite(0,glm::vec2(900, 600), glm::vec2(300, 300)));
	uiHandler->addShape(new UISprite(0,glm::vec2(300, 600), glm::vec2(300, 300)));
	uiHandler->addShape(new UISprite(0,glm::vec2(900, 200), glm::vec2(300, 300)));
	/*for (int i = 0; i < 500; ++i)
		uiHandler->addShape(new UISprite(0, glm::vec2(glm::cos(a + 20 * i / 1000.0) * 300 + 300, glm::sin(a + 20 * i / 3000.0) * 100 + 200), glm::vec2(300, 300)));*/
}
