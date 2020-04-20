#pragma once
#pragma comment(lib, "Irrlicht.lib")
#ifdef _DEBUG
#pragma comment(lib, "LuaLibd.lib")
#else
#pragma comment(lib, "Lualib.lib")
#endif
#include <irrlicht.h>
#include <iostream>
#include <lua.hpp>
#include "lua.hpp"
#include <string>
#include <assert.h>
//using namespace scene;
using namespace irr;
using namespace std;
using namespace scene;

class Vector3
{

private:
	float x, y, z = 0;
public:
	static IrrlichtDevice* device;
	static irr::scene::IAnimatedMesh* mesh;
	Vector3();

	static int Vector_New(lua_State* state);
	static int Vector_SetPosition(lua_State* state);
	static int Vector_GetPosition(lua_State* state);
	static int Vector_Delete(lua_State* state);
	static int Vector_Print(lua_State* state);

	static Vector3* checkVector(lua_State* state, int stackIndex);

	static void registerLuaCFunctions(lua_State* state);
	static void initClass(IrrlichtDevice* device, lua_State* state);
};

