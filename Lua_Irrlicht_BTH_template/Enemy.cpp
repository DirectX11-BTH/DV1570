
#include "Enemy.h"
#include <assert.h>

IrrlichtDevice* Enemy::device = nullptr;
scene::IAnimatedMesh* Enemy::mesh = nullptr;

void Enemy::loadMeshForClass()
{
}

Enemy::Enemy(const char* name)
{
	this->name = "properName";
	this->health = 150;

	modelNode = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
}

int Enemy::Enemy_New(lua_State* state)
{
	//Constructor in lua: Enemy(name)
	if (lua_isstring(state, -1))
	{
		const char* name = lua_tolstring(state, -1, nullptr);
		assert(name != nullptr);

		Enemy** enemy = reinterpret_cast<Enemy**>(lua_newuserdata(state, sizeof(Enemy*)));
		*enemy = new Enemy(name);

		luaL_getmetatable(state, "MetaEnemy"); // Pushes onto the stack, the metatable associat the name in the registry
		lua_setmetatable(state, -2); // Pops a table from the stack and sets it as the new metatable for the value at the given index

		
	}
	else
	{

	}
	return 1; //Nr of args pushed to the stack
}

Enemy* Enemy::checkEnemy(lua_State* state, int stackIndex)
{
	Enemy* enemyPtr = nullptr;

	void* checkPtr = luaL_testudata(state, stackIndex, "MetaEnemy");  //Check if it is a userdata with the metatable metamonster.

	if (checkPtr != nullptr) // it's a enemy if not null
		enemyPtr = *(Enemy**)checkPtr;

	assert(enemyPtr != nullptr);

	return enemyPtr; //If this is null, it is not an enemy.
}

int Enemy::Enemy_Print(lua_State* state)
{
	Enemy* enemy = checkEnemy(state, 1);
	
	cout << "Name: " << enemy->name.data() << " " << "Health: " << enemy->health << endl;

	return 0;
}

int Enemy::Enemy_SetHealth(lua_State* state)
{
	
	return 0;
}

int Enemy::Enemy_SetPosition(lua_State* state)
{
	Enemy* enemy = checkEnemy(state, 1);

	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3))
	{
		float x = lua_tonumber(state, -1);
		float y = lua_tonumber(state, -2);
		float z = lua_tonumber(state, -3);
		lua_pop(state, 3);

		enemy->modelNode->setPosition(core::vector3df(x, y, z));
	}
	return 0;
}

int Enemy::Enemy_Delete(lua_State* state)
{
	Enemy* enemy = checkEnemy(state, 1);

	delete enemy;

	cout << "Deleted Enemy" << endl;

	return 0;
}

void Enemy::registerLuaCFunctions(lua_State* state) //Called externally once
{
	luaL_newmetatable(state, "MetaEnemy"); //Our metatable for lua
	luaL_Reg enemyFuncs[] =
	{
		{"new", Enemy_New},
		{"print", Enemy::Enemy_Print},
		{"setPosition", Enemy::Enemy_SetPosition},
		{"__gc", Enemy::Enemy_Delete}, //Garbage collect function on lua's side
		{NULL, NULL}
	};

	luaL_setfuncs(state, enemyFuncs, 0);

	lua_pushvalue(state, -1); //Push table to top, pushes a copy of the element at given index onto stack

	lua_setfield(state, -1, "__index"); //Point __index field of meta table to itself. Same as t[k] = v

	lua_setglobal(state, "Enemy"); //Expose 'Enemy' to lua, sets value of 'Monster' table.
}

void Enemy::initClass(IrrlichtDevice* device, lua_State* state)
{
	Enemy::registerLuaCFunctions(state); //Registers functions for enemy
	Enemy::device = device;
	io::path filename("actualCube.obj");
	Enemy::mesh = device->getSceneManager()->getMesh(filename);
}
