#pragma comment(lib, "Irrlicht.lib")
#ifdef _DEBUG
#pragma comment(lib, "LuaLibd.lib")
#else
#pragma comment(lib, "Lualib.lib")
#endif

#include <lua.hpp>
#include <Windows.h>
#include <iostream>
#include <thread>
#include "lua.hpp"
#include <irrlicht.h>
#include "Movable.h"
#include "EventReceiver.h"
#include <string>
#include <algorithm>

//#include "Vector3Lua.h"

const int HEIGHT = 720;
const int WIDTH = 1080;

using namespace scene;
using namespace irr;
using namespace core;
using namespace video;
using namespace io;
using namespace gui;

scene::ISceneManager* smgr = nullptr;
lua_State* luaState = nullptr;
EventReceiver* globalReciever = nullptr;

bool w = false;
bool a = false;
bool s = false;
bool d = false;
bool one = false;
bool two = false;
bool three = false;
bool space = false;
bool n = false;
bool m = false;
bool mouseButtonOne = false;
bool mouseButtonTwo = false;
bool F1 = false;
bool F2 = false;
bool middleMouseButton = false;
wstring displayString = L"";

int checkCollision(lua_State* state)
{
	Movable* movable1 = Movable::checkMovable(state, -1);
	Movable* movable2 = Movable::checkMovable(state, -2);

	if (movable1 == nullptr || movable2 == nullptr)
	{
		std::cout << "ERROR, BOTH WERE NOT MOVABLES." << std::endl;

	}
	bool collided = false;

	//Do collision check s here 
	//collided = true;
	auto boundingBox1 = movable1->modelNode->getTransformedBoundingBox();
	auto boundingBox2 = movable2->modelNode->getTransformedBoundingBox();
	if (boundingBox1.intersectsWithBox(boundingBox2) && (movable1->modelNode->getPosition() != core::vector3df(0, 0, 0)) && (movable2->modelNode->getPosition() != core::vector3df(0, 0, 0)))
		collided = true;
	
	lua_pushboolean(state, collided);
	return 1;
}



int setCameraPos(lua_State* luaState)
{
	if (lua_isnumber(luaState, -1) && lua_isnumber(luaState, -2) && lua_isnumber(luaState, -3))
	{
		float y = lua_tonumber(luaState, -2);
		float z = lua_tonumber(luaState, -3);
		float x = lua_tonumber(luaState, -1);
		
		lua_pop(luaState, 3);

		smgr->getActiveCamera()->setPosition(vector3df(x, y, z));
		smgr->getActiveCamera()->setTarget(vector3df(x, 0, z));
	}

	return 0;
}

int setText(lua_State* luaState)
{
	if (lua_isstring(luaState, -1))
	{
		std::string textString = lua_tostring(luaState, -1);
		//displayString = lua_tostring(luaState, -1);
		//std::cout << displayString.data() << std::endl;

		std::wstring tempString(textString.length(), L' ');
		std::copy(textString.begin(), textString.end(), tempString.begin());

		displayString = tempString;
		
		
		/*wstring wideusername;
		for(int i = 0; i < textString.length(); ++i)
				wideusername += wchar_t( textString[i] );

		const wchar_t* your_result = wideusername.c_str();*/
		


		lua_pop(luaState, 1);
	}

	return 0;
}

int getMouseHit(lua_State* state)
{

	core::line3df ray = smgr->getSceneCollisionManager()->getRayFromScreenCoordinates(vector2d<irr::s32>(globalReciever->GetMouseState().Position.X, globalReciever->GetMouseState().Position.Y), smgr->getActiveCamera());//irr::getRayFromScreenCoordinates(mouseX, mouseY, &device);
	core::vector3d<f32>startintersection;
	//node->getBoundingBox().intersectsWithLine()
	//std::cout << ray.end.X << " " << ray.end.Z << std::endl;

	core::vector3df collisionPoint;
	core::triangle3df collisionTri;
	scene::ISceneNode* pNode = 0;
	pNode = smgr->getSceneCollisionManager()->getSceneNodeAndCollisionPointFromRay(ray, collisionPoint, collisionTri);
	//std::cout << collisionPoint.X << " " << collisionPoint.Y << " " << collisionPoint.Z << " " << std::endl;

	lua_pushnumber(state, collisionPoint.Z);
	lua_pushnumber(state, collisionPoint.Y);
	lua_pushnumber(state, collisionPoint.X);

	return 3;
}

void pushKeysToLua(float mouseX, float mouseY)
{
	//lua_getglobal(luaState, "w");
	lua_pushboolean(luaState, w);
	lua_setglobal(luaState, "w");
	//lua_pop(luaState, -1);

	//lua_getglobal(luaState, "s");
	lua_pushboolean(luaState, s);
	lua_setglobal(luaState, "s");

	lua_pushboolean(luaState, a);
	lua_setglobal(luaState, "a");

	lua_pushboolean(luaState, d);
	lua_setglobal(luaState, "d");

	lua_pushboolean(luaState, m);
	lua_setglobal(luaState, "m");

	lua_pushboolean(luaState, mouseButtonOne);
	lua_setglobal(luaState, "mouseButtonOne");

	lua_pushboolean(luaState, mouseButtonTwo);
	lua_setglobal(luaState, "mouseButtonTwo");

	lua_pushboolean(luaState, space);
	lua_setglobal(luaState, "space");

	lua_pushnumber(luaState, mouseY);
	lua_setglobal(luaState, "mouseY");

	lua_pushnumber(luaState, mouseX);
	lua_setglobal(luaState, "mouseX");

	lua_pushboolean(luaState, one);
	lua_setglobal(luaState, "one");

	lua_pushboolean(luaState, two);
	lua_setglobal(luaState, "two");

	lua_pushboolean(luaState, three);
	lua_setglobal(luaState, "three");

	lua_pushboolean(luaState, n);
	lua_setglobal(luaState, "n");

	lua_pushboolean(luaState, middleMouseButton);
	lua_setglobal(luaState, "middleMouseButton");

	lua_pushboolean(luaState, F1);
	lua_setglobal(luaState, "F1");

	lua_pushboolean(luaState, F2);
	lua_setglobal(luaState, "F2");

}

void checkKeys()
{
	w = false;
	s = false;
	d = false;
	a = false;
	m = false;
	one = false;
	n = false;
	two = false;
	three = false;
	space = false;
	F1 = false;
	F2 = false;
	mouseButtonOne = false;
	mouseButtonTwo = false;
	middleMouseButton = false;

	if (GetAsyncKeyState(KEY_KEY_W))
	{
		w = true;
	}
	if (GetAsyncKeyState(VK_F1))
	{
		F1 = true;
	}
	if (GetAsyncKeyState(VK_F2))
	{
		F2 = true;
	}
	if (GetAsyncKeyState(KEY_KEY_S))
	{
		s = true;
	}
	if (GetAsyncKeyState(KEY_KEY_D))
	{
		d = true;
	}
	if (GetAsyncKeyState(KEY_KEY_A))
	{
		a = true;
	}
	if (GetAsyncKeyState(KEY_KEY_M))
	{
		m = true;
	}
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		mouseButtonOne = true;
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		space = true;
	}
	if (GetAsyncKeyState(KEY_KEY_N))
	{
		n = true;
	}

	if (GetAsyncKeyState(KEY_KEY_1))
	{
		one = true;
	}

	if (GetAsyncKeyState(KEY_KEY_2))
	{
		two = true;
	}

	if (GetAsyncKeyState(KEY_KEY_3))
	{
		three = true;
	}

	if (GetAsyncKeyState(VK_RBUTTON))
	{
		mouseButtonTwo = true;
	}
	if (GetAsyncKeyState(VK_MBUTTON))
	{
		middleMouseButton = true;
	}
	
}

void ConsoleThread(lua_State* luaState)
{
	lua_pushcfunction(luaState, setCameraPos);
	lua_setglobal(luaState, "setCameraPos");

	lua_pushcfunction(luaState, getMouseHit);
	lua_setglobal(luaState, "getMouseHit");

	lua_pushcfunction(luaState, checkCollision);
	lua_setglobal(luaState, "checkCollision");

	lua_pushcfunction(luaState, setText);
	lua_setglobal(luaState, "setText");

	char command[1000];
	while (GetConsoleWindow())
	{
		memset(command, 0, 1000);
		std::cin.getline(command, 1000);
		if (luaL_loadstring(luaState, command) || lua_pcall(luaState, 0, 0, 0))
			std::cout << lua_tostring(luaState, -1) << '\n';
	}
}

void pushDimensionToLua()
{
	lua_pushnumber(luaState, WIDTH);
	lua_setglobal(luaState, "screenWidth");

	lua_pushnumber(luaState, HEIGHT);
	lua_setglobal(luaState, "screenHeight");
}
	
int main()
{

	EventReceiver receiver;
	IrrlichtDevice* device = createDevice(video::EDT_DIRECT3D9, core::dimension2d<u32>(WIDTH, HEIGHT), 16, false, false, true, &receiver);
	globalReciever = &receiver;
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	video::IVideoDriver* driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	//IGUIStaticText* textPtr = guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", core::rect<s32>(10, 10, 300, 55), true, true, 0, -1, true);
	//textPtr->font
	IGUISkin* skin = guienv->getSkin();
	gui::IGUIFont* font2 = device->getGUIEnvironment()->getFont("myfont.xml");
	skin->setFont(font2);
	driver->getMaterial2D().TextureLayer[0].BilinearFilter=true;
    driver->getMaterial2D().AntiAliasing=video::EAAM_FULL_BASIC;
	scene::ILightSceneNode* light = smgr->addLightSceneNode(0, core::vector3df(0, 25, 0), video::SColor(0.5f, 0.5f, 0.5f, 0.0f), 100.f);
	video::SLight testSlight = light->getLightData();

	//light->setLightData()
	//driver->turnLightOn(0, false);
	
	//biggerFont->
	//guienv->addStaticText(L"Test", true, rect<int>(10,10,200,30));

	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	Movable::initClass(device, luaState);
	pushDimensionToLua();

	std::thread conThread(ConsoleThread, luaState);
	if (!device)
		return 1;

	path filename("testQuad.obj");
	scene::IAnimatedMesh* test = device->getSceneManager()->getMesh(filename);
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(test);

	node->setMaterialFlag(video::EMF_LIGHTING, false);
	
	// collision initialize
	scene::ITriangleSelector* selector = 0;
	if(node)
	{
		selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
		node->setTriangleSelector(selector);
	}


	node->setScale({ 100, 1, 100 });


	test->setBoundingBox(core::aabbox3df(-3.f,-3.f,-3.f,3.f,3.f,3.f));
	smgr->addCameraSceneNode(0, vector3df(0, 25, 0), vector3df(0, 0, 0));

	//light->setDebugDataVisible(scene::EDS_BBOX);

	
	luaL_dofile(luaState, "./update.lua");
	while (device->run())
	{
		//::cout << driver->getFPS()<< std::endl;
		driver->enableClipPlane(0,false);
		checkKeys();
		pushKeysToLua(receiver.GetMouseState().Position.X, receiver.GetMouseState().Position.Y);

		/*
		irr::core::line3df collRay;

		collRay = pCollMgr->getRayFromScreenCoordinates(mousePos, pSceneMgr->getActiveCamera()); // this appears to be getting unique values
	
		// get the collision 
		core::vector3df collPoint; 
		core::triangle3df collTriangle; 
		scene::ISceneNode* pNode = 0; 
	
		pNode = pCollMgr->getSceneNodeAndCollisionPointFromRay(collRay, collPoint, collTriangle);
		*/

		//ground.getIntersectionWithLine(ray.start,ray.end,startintersection);
		//Separat skript som körs varje game loop - idk?
		//luaL_dofile("./update.lua");
		
		lua_getglobal(luaState, "update");
		int error = lua_pcall(luaState, 0, 0, 0, 0);

		if (error)
		{
			std::cout << "error: " << lua_tostring(luaState, lua_gettop(luaState)) << std::endl;
			lua_pop(luaState, 1);
		}

		
		driver->beginScene(true, true, video::SColor(255, 11, 11, 11));

		smgr->drawAll();
		guienv->drawAll();

		font2->draw(displayString.data(),
                core::rect<s32>(5,5,300,300),
                video::SColor(255, 255,255,255));

		driver->endScene();
	}

	device->drop();

	conThread.join();
	return 0;
}