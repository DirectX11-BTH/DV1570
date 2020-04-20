#include "Vector3Lua.h"

Vector3::Vector3()
{
}

int Vector3::Vector_New(lua_State* state)
{
	if (lua_isstring(state, -1))
	{
		const char* name = lua_tolstring(state, -1, nullptr);
		assert(name != nullptr);

		Vector3** vector = reinterpret_cast<Vector3**>(lua_newuserdata(state, sizeof(Vector3*)));
		*vector = new Vector3();

		luaL_getmetatable(state, "MetaVector"); // Pushes onto the stack, the metatable associat the name in the registry
		lua_setmetatable(state, -2); // Pops a table from the stack and sets it as the new metatable for the value at the given index

	}
	else
	{

	}
	return 1; //Nr of args pushed to the stack
}

int Vector3::Vector_SetPosition(lua_State* state)
{
	Vector3* vector = checkVector(state, 1);

	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3))
	{
		float x = lua_tonumber(state, -1);
		float y = lua_tonumber(state, -2);
		float z = lua_tonumber(state, -3);
		lua_pop(state, 3);
	}

	return 0;
}

int Vector3::Vector_GetPosition(lua_State* state)
{
	Vector3* vector = checkVector(state, 1);
	lua_pushnumber(state, vector->x);
	lua_pushnumber(state, vector->y);
	lua_pushnumber(state, vector->z);
	return 3;
}

int Vector3::Vector_Delete(lua_State* state)
{
	Vector3* vector = checkVector(state, 1);

	delete vector;

	cout << "Deleted Vector" << endl;

	return 0;
}

int Vector3::Vector_Print(lua_State* state)
{
	Vector3* vector = checkVector(state, 1);

	cout << vector->x << " " << vector->y << " " << vector->z << endl;

	return 0;
}

Vector3* Vector3::checkVector(lua_State* state, int stackIndex)
{
	Vector3* vectorPtr = nullptr;

	void* checkPtr = luaL_testudata(state, stackIndex, "MetaVector");  //Check if it is a userdata with the metatable metamonster.

	if (checkPtr != nullptr) // it's a vector if not null
		vectorPtr = *(Vector3**)vectorPtr;

	assert(vectorPtr != nullptr);

	return vectorPtr; //If this is null, it is not an vector.
}

void Vector3::registerLuaCFunctions(lua_State* state)
{
	luaL_newmetatable(state, "MetaEnemy"); //Our metatable for lua
	luaL_Reg enemyFuncs[] =
	{
		{"new", Vector_New},
		{"print", Vector3::Vector_Print},
		{"setPosition",Vector3::Vector_SetPosition},
		{"getPosition",Vector3::Vector_GetPosition},
		{"__gc", Vector3::Vector_Delete}, //Garbage collect function on lua's side
		{NULL, NULL}
	};

	luaL_setfuncs(state, enemyFuncs, 0);

	lua_pushvalue(state, -1); //Push table to top, pushes a copy of the element at given index onto stack

	lua_setfield(state, -1, "__index"); //Point __index field of meta table to itself. Same as t[k] = v

	lua_setglobal(state, "Enemy"); //Expose 'Enemy' to lua, sets value of 'Monster' table.
}

void Vector3::initClass(IrrlichtDevice* device, lua_State* state)
{
	Vector3::registerLuaCFunctions(state); //Registers functions for enemy
}
