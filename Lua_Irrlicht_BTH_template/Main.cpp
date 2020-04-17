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
#include "Enemy.h"
#include "Player.h"

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


int setCameraPos(lua_State* luaState)
{
	if (lua_isnumber(luaState, -1) && lua_isnumber(luaState, -2) && lua_isnumber(luaState, -3))
	{
		float x = lua_tonumber(luaState, -1);
		float y = lua_tonumber(luaState, -2);
		float z = lua_tonumber(luaState, -3);
		lua_pop(luaState, 3);

		smgr->getActiveCamera()->setPosition(vector3df(x, y, z));
	}

	return 0;
}

void pushKeysToLua()
{
	//sdfgadlfghl
}

void checkKeys()
{

}

void ConsoleThread(lua_State* luaState)
{
	lua_pushcfunction(luaState, setCameraPos);
	lua_setglobal(luaState, "setCameraPos");

	char command[1000];
	while (GetConsoleWindow())
	{
		memset(command, 0, 1000);
		std::cin.getline(command, 1000);
		if (luaL_loadstring(luaState, command) || lua_pcall(luaState, 0, 0, 0))
			std::cout << lua_tostring(luaState, -1) << '\n';
	}
}

int main()
{
	/*
	if(receiver.IsKeyDown(irr::KEY_KEY_W))
nodePosition.Y += MOVEMENT_SPEED * frameDeltaTime;
else if(receiver.IsKeyDown(irr::KEY_KEY_S))
nodePosition.Y -= MOVEMENT_SPEED * frameDeltaTime;

if(receiver.IsKeyDown(irr::KEY_KEY_A))
nodePosition.X -= MOVEMENT_SPEED * frameDeltaTime;
else if(receiver.IsKeyDown(irr::KEY_KEY_D))
nodePosition.X += MOVEMENT_SPEED * frameDeltaTime;
'*/

	IrrlichtDevice* device = createDevice(video::EDT_SOFTWARE, core::dimension2d<u32>(1080, 720), 16, false, false, true, 0);
	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
	video::IVideoDriver* driver = device->getVideoDriver();
	smgr = device->getSceneManager();
	gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

	luaState = luaL_newstate();
	luaL_openlibs(luaState);

	Enemy::initClass(device, luaState);
	Player::initClass(device, luaState);

	//luaL_dostring(luaState, "local myEnemy = Enemy.new('Harry') myEnemy:print() myEnemy:setPosition(-5, -5, -5)");

	std::thread conThread(ConsoleThread, luaState);
	if (!device)
		return 1;

	/*path filename("actualCube.obj");
	scene::IAnimatedMesh* test = device->getSceneManager()->getMesh(filename);
	if (!test)
	{
		// model could not be loaded
		guienv->addStaticText(L"Error", core::rect<s32>(50, 10, 260, 22), true);
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(test);
	*/
	smgr->addCameraSceneNode(0, vector3df(3, 10, 15), vector3df(0, 0, 0));

	// Now we can load the mesh by calling irr::scene::ISceneManager::getMesh()

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!", core::rect<s32>(10, 10, 260, 22), true);

	luaL_dofile(luaState, "./update.lua");
	while (device->run())
	{
		w = false;
		s = false;
		if(GetAsyncKeyState(KEY_KEY_W))
		{
			std::cout << "W" << std::endl;
			w = true;
		}
		if (GetAsyncKeyState(KEY_KEY_S))
		{
			std::cout << "S" << std::endl;
			s = true;
		}

		//lua_getglobal(luaState, "w");
		lua_pushboolean(luaState, w);
		lua_setglobal(luaState, "w");
		//lua_pop(luaState, -1);

		//lua_getglobal(luaState, "s");
		lua_pushboolean(luaState, s);
		lua_setglobal(luaState, "s");


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