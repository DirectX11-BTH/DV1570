
#include "Player.h"
#include <assert.h>

IrrlichtDevice* Player::device = nullptr;
scene::IAnimatedMesh* Player::mesh = nullptr;

void Player::loadMeshForClass()
{
}

Player::Player(const char* name)
{
	this->name = "properName";
	this->health = 150;

	modelNode = device->getSceneManager()->addAnimatedMeshSceneNode(mesh);
}

int Player::Player_New(lua_State* state)
{
	//Constructor in lua: Player(name)
	if (lua_isstring(state, -1))
	{
		const char* name = lua_tolstring(state, -1, nullptr);
		assert(name != nullptr);

		Player** player = reinterpret_cast<Player**>(lua_newuserdata(state, sizeof(Player*)));
		*player = new Player(name);

		luaL_getmetatable(state, "MetaPlayer"); // Pushes onto the stack, the metatable associat the name in the registry
		lua_setmetatable(state, -2); // Pops a table from the stack and sets it as the new metatable for the value at the given index


	}
	else
	{

	}
	return 1; //Nr of args pushed to the stack
}

Player* Player::checkPlayer(lua_State* state, int stackIndex)
{
	Player* playerPtr = nullptr;

	void* checkPtr = luaL_testudata(state, stackIndex, "MetaPlayer");  //Check if it is a userdata with the metatable metamonster.

	if (checkPtr != nullptr) // it's a player if not null
		playerPtr = *(Player**)checkPtr;

	assert(playerPtr != nullptr);

	return playerPtr; //If this is null, it is not an player.
}

int Player::Player_Print(lua_State* state)
{
	Player* player = checkPlayer(state, 1);

	cout << "Name: " << player->name.data() << " " << "Health: " << player->health << endl;

	return 0;
}

int Player::Player_SetHealth(lua_State* state)
{

	return 0;
}

int Player::Player_SetPosition(lua_State* state)
{
	Player* player = checkPlayer(state, 1);

	if (lua_isnumber(state, -1) && lua_isnumber(state, -2) && lua_isnumber(state, -3))
	{
		float x = lua_tonumber(state, -1);
		float y = lua_tonumber(state, -2);
		float z = lua_tonumber(state, -3);
		lua_pop(state, 3);

		player->modelNode->setPosition(core::vector3df(x, y, z));
	}
	return 0;
}

int Player::Player_Delete(lua_State* state)
{
	Player* player = checkPlayer(state, 1);

	delete player;

	cout << "Deleted Player" << endl;

	return 0;
}

int Player::Player_Move(lua_State* state)
{
	float x = lua_tonumber(state, -1);
	float y = lua_tonumber(state, -2);
	float z = lua_tonumber(state, -3);
	lua_pop(state, 3);

	Player* player = checkPlayer(state, 1);
	player->modelNode->setPosition(player->modelNode->getPosition() + core::vector3df(x, y, z));

	return 0;
}

void Player::registerLuaCFunctions(lua_State* state) //Called externally once
{
	luaL_newmetatable(state, "MetaPlayer"); //Our metatable for lua
	luaL_Reg playerFuncs[] =
	{
		{"new", Player_New},
		{"print", Player::Player_Print},
		{"setPosition", Player::Player_SetPosition},
		{"move", Player::Player_Move},
		{"__gc", Player::Player_Delete}, //Garbage collect function on lua's side
		{NULL, NULL}
	};

	luaL_setfuncs(state, playerFuncs, 0);

	lua_pushvalue(state, -1); //Push table to top, pushes a copy of the element at given index onto stack

	lua_setfield(state, -1, "__index"); //Point __index field of meta table to itself. Same as t[k] = v

	lua_setglobal(state, "Player"); //Expose 'Player' to lua, sets value of 'Monster' table.
}

void Player::initClass(IrrlichtDevice* device, lua_State* state)
{
	Player::registerLuaCFunctions(state); //Registers functions for player
	Player::device = device;
	io::path filename("actualCube.obj");
	Player::mesh = device->getSceneManager()->getMesh(filename);
}
