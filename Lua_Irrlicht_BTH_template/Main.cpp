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

bool w = false;
bool a = false;
bool s = false;
bool d = false;
bool space = false;
bool mouseButtonOne = false;

void DumpStack(lua_State* L)
{
    std::cout << "------- STACK DUMP -------\n";
    for(int i = lua_gettop(L); i > 0; i--)
    {
        std::cout << "Index " << i << ": " << lua_typename(L, lua_type(L, i)) << "\n";
    }
    std::cout << "--------------------------\n";
}

int checkCollision(lua_State* state)
{
	Movable* movable1 = Movable::checkMovable(state, -1);
	Movable* movable2 = Movable::checkMovable(state, -2);

	if (movable1 == nullptr || movable2 == nullptr)
	{
		std::cout << "ERROR, BOTH WERE NOT MOVABLES." << std::endl;
		DumpStack(state);
	}
	bool collided = false;

	//Do collision check s here 
	//collided = true;
	auto boundingBox1 = movable1->modelNode->getTransformedBoundingBox();
	auto boundingBox2 = movable2->modelNode->getTransformedBoundingBox();
	if (boundingBox1.intersectsWithBox(boundingBox2))
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

	lua_pushboolean(luaState, mouseButtonOne);
	lua_setglobal(luaState, "mouseButtonOne");

	lua_pushnumber(luaState, space);
	lua_setglobal(luaState, "space");

	lua_pushnumber(luaState, mouseY);
	lua_setglobal(luaState, "mouseY");

	lua_pushnumber(luaState, mouseX);
	lua_setglobal(luaState, "mouseX");
}

void checkKeys()
{
	w = false;
	s = false;
	d = false;
	a = false;
	space = false;
	mouseButtonOne = false;

	if (GetAsyncKeyState(KEY_KEY_W))
	{
		w = true;
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
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		mouseButtonOne = true;
	}
	if (GetAsyncKeyState(VK_SPACE))
	{
		space = true;
	}
}

void ConsoleThread(lua_State* luaState)
{
	lua_pushcfunction(luaState, setCameraPos);
	lua_setglobal(luaState, "setCameraPos");

	lua_pushcfunction(luaState, checkCollision);
	lua_setglobal(luaState, "checkCollision");

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
	IrrlichtDevice* device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(WIDTH, HEIGHT), 16, false, false, true, &receiver);
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	video::IVideoDriver* driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
	

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
	// collision initialize
	scene::ITriangleSelector* selector = 0;
	if(node)
	{
		selector = smgr->createOctreeTriangleSelector(node->getMesh(), node, 128);
		node->setTriangleSelector(selector);
	}

	

	//scene::ISceneNodeAnimator* animator = smgr->createCollisionResponseAnimator(selector,
	//test,							// the object to detect collision
	//core::vector3df(30,50,30),	 //how big the object is),
	//core::vector3df(0,0,0),		// direction and speed of gratity
	//core::vector3df(0,0,0));		//translation
	//selector->drop();
	
	//node->setDebugDataVisible(1);
	node->setScale(core::vector3df(10, 1, 10));
	node->setMaterialFlag(EMF_LIGHTING, true);


	test->setBoundingBox(core::aabbox3df(-3.f,-3.f,-3.f,3.f,3.f,3.f));
	smgr->addCameraSceneNode(0, vector3df(0, 25, 0), vector3df(0, 0, 0));

	//scene::ILightSceneNode* light = smgr->addLightSceneNode(0, core::vector3df(0, 25, 0), video::SColor(0.5f, 0.5f, 0.5f, 0.0f), 40.f);
	//light->setDebugDataVisible(scene::EDS_BBOX);


	luaL_dofile(luaState, "./update.lua");
	while (device->run())
	{

		driver->enableClipPlane(0,false);
		checkKeys();
		pushKeysToLua(receiver.GetMouseState().Position.X, receiver.GetMouseState().Position.Y);

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

		driver->endScene();
	}

	device->drop();

	conThread.join();
	return 0;
}