--LUA PROJECT BY GHAZI LIDBORN AND LEO 2020 #corona

controls = [[
Keys:
Play mode:
w, a, s, d - for movements
Left button - fire

Edit mode:
m - start editor
n - exit editor
Left mouse button - place Wall
Right mouse button - place Obstacle
Middle mouse button - place spawnPad
F1 - Load saved level
F2 - Save level
Space - clear level
]]


bullets = {}
enemyBullets = {}
enemies = {}
speedDropTable = {}
heartDropTable = {}
walls = {}
obstacles = {}
spawnPoints = {}

score = 0
respawnTimer = 0
spawnWhenTimerEquals = 100
arenaSize = 40

editMode = false
reqToPlace = 10
placeTimer = 0
heightFromGround = (-0.95)
MovableLua = {
	position = {x = 0, y = 0, z = 0},
	gameObject = nil
}
local movableMeta = {__index = MovableLua}

function MovableLua:setPosition(x, y, z)
	self.position.x = x
	self.position.y = y
	self.position.z = z


	self.gameObject:setPosition(x, y, z)
end

function MovableLua:getRotation()
	return self.gameObject:getRotation()
end

function MovableLua:setRotation(yaw)
	self.gameObject:setRotation(yaw)
end


function MovableLua:setBB(bb, text)
	self.gameObject:setBB(bb, text)
end

function MovableLua:getPosition()
	return self.gameObject:getPosition()
end

function MovableLua:move(x, y, z)

	local posX, posY, posZ = self:getPosition()

	self:setPosition(posX+x, posY+y, posZ+z)
end

function MovableLua:setScale(x, y, z)
	self.gameObject:setScale(x, y, z)
end


function MovableLua:new(table)
	local t = table or {}
	
	self.__index = self
	setmetatable(t, self)

	return t
end

function MovableLua:init3DModel(path, sx, sy, sz)
	if not path then
		path = "actualCube.obj"
	end

	sx = sx or 1
	sy = sy or 1
	sz = sz or 1

	
	self.gameObject = Movable:new(sx, sy, sz, path) -- lua class is called Movable and is registered as so, takes scale

end
--------------------------------------------------------------------


Projectile = MovableLua:new() 
Projectile.xSpeed = 0
Projectile.zSpeed = 0
Projectile.invFrames = 3
Projectile.currentFrames = 0


SpeedDrop = MovableLua:new()
HeartDrop = MovableLua:new()

Player = MovableLua:new()
Player.speed = 5
Player.health = 100
Player.selectedWeapon = "AUTO"
Player.damage = 10
Player.shootDebounce = 0
Player.requiredToShoot = 5

myPlayer = Player:new()
myPlayer:init3DModel("testSphere.obj")
myPlayer:setPosition(0,0,0)
myPlayer:setScale(1,1,1)
function Player:handleCheckWeapon()

	if one then
	--switch weapon
		self.selectedWeapon = "AUTO"
		self.damage = 25
		self.requiredToShoot = 5
	
	elseif two then
	--switch weapon
		self.selectedWeapon = "SHOTGUN"
		self.damage = 40
		self.requiredToShoot = 9
	elseif three then
	--switch weapon
		self.selectedWeapon = "SNIPER"
		self.requiredToShoot = 20
		self.damage = 5000
	end

end



Enemy = MovableLua:new()
Enemy.speed = 0.5
Enemy.health = 50
Enemy.shootDebounce = 0
Enemy.requiredToShoot = 10
Enemy.walkToX = 0
Enemy.walkToZ = 0


Obstacle = MovableLua:new() 

Spawnpoint = MovableLua:new()

Wall = MovableLua:new()

function createArena()
	local wall1 = Wall:new() -- top
	wall1:init3DModel()
	wall1:setPosition(0,0,arenaSize)
	wall1:setScale(arenaSize,1,3)
	table.insert(walls, wall1)

	local wall2 = Wall:new() -- bot
	wall2:init3DModel()
	wall2:setPosition(0,0,-arenaSize)
	wall2:setScale(arenaSize,1,3 )
	table.insert(walls, wall2)

	local wall3 = Wall:new() -- left
	wall3:init3DModel()
	wall3:setPosition(-arenaSize,0,0)
	wall3:setScale(3,1,arenaSize)
	table.insert(walls, wall3)

	local wall4 = Wall:new() -- right
	wall4:init3DModel()
	wall4:setPosition(arenaSize,0,0)
	wall4:setScale(3,1,arenaSize)
	table.insert(walls, wall4)

	local startSpawn = Spawnpoint:new() -- default spawner, needed for logic
	startSpawn:init3DModel("spawnPoint.obj")
	startSpawn:setPosition(10,heightFromGround,10)
	startSpawn:setScale(2,1,2)
	table.insert(spawnPoints, startSpawn)
end

	local startSpawn = Spawnpoint:new() -- default spawner, needed for logic
	startSpawn:init3DModel("spawnPoint.obj")
	startSpawn:setPosition(10,heightFromGround,10)
	startSpawn:setScale(2,1,2)
	table.insert(spawnPoints, startSpawn)

function readFile()
local file = io.open("save.boll", "r")
			removeObjects()
			io.input(file)

			local nrOfSpawnPoints = io.read() 
			print("Spawnpoints ".. nrOfSpawnPoints)
			for i=1,nrOfSpawnPoints do
				local x = io.read()
				local z = io.read()
				local startSpawn = Spawnpoint:new() -- default spawner
				startSpawn:init3DModel("spawnPoint.obj")
				startSpawn:setPosition(x,heightFromGround,z)
				startSpawn:setScale(2,1,2)
				table.insert(spawnPoints, startSpawn)
			end
			--READS OBSTACLES FROM FILE
			local nrOfObstacles = io.read() 
			for i=1,nrOfObstacles do
				local x = io.read()
				local z = io.read()
				local obstacle = Obstacle:new() -- default obstacle
				obstacle:init3DModel("obstacle.obj")
				obstacle:setPosition(x,heightFromGround,z)
				obstacle:setScale(2,1,2)
				table.insert(obstacles, obstacle)
			end
			--READS WALLS FROM FILE
			local nrOfWalls = io.read() 
			for i=1,nrOfWalls do
				local x = io.read()
				local z = io.read()
				local wall = Wall:new() -- default wall
				wall:init3DModel("treeLua.obj")
				wall:setPosition(x,0,z)
				wall:setScale(1,1,1)
				wall:setBB(1, "treeLua.obj")
				table.insert(walls, wall)
			end

			if #spawnPoints == 0 then
				local startSpawn = Spawnpoint:new() -- default spawner, needed for logic
				startSpawn:init3DModel("spawnPoint.obj")
				startSpawn:setPosition(10,heightFromGround,10)
				startSpawn:setScale(2,0.1,2)
				table.insert(spawnPoints, startSpawn)	
			end


			io.close(file)
end

function removeObjects()
		print("REMOVING")
		-- removes all objects from scene
		for bulletIndex,enemyBullet in pairs(enemyBullets) do
			enemyBullets[bulletIndex] = nil
		end 

		for i,v in pairs(bullets) do
			bullets[i] = nil
		end

		for i,v in pairs(enemies) do
			enemies[i] = nil
		end

		for i,v in pairs(speedDropTable) do
			speedDropTable[i] = nil
		end

		for i,v in pairs(spawnPoints) do
			spawnPoints[i] = nil
		end
		--secretTextToShowWeMadeThisProgram Jakob, Ghazi and the other dude <3 'Barry'
		for i,v in pairs(heartDropTable) do

			heartDropTable[i] = nil
		end

end

function Enemy:update(playerVar)
	local x, y, z = self:getPosition()
	
	local xPlr, yPlr, zPlr = playerVar:getPosition()

	if self.walkToX == nil or self.walkToZ == nil or (math.abs(x - self.walkToX) < 2 and math.abs(z - self.walkToZ) < 2) then
		self.walkToX = math.random(-18,18)
		self.walkToZ = math.random(-18,18)

	end

	if self.walkToX > x then
		self:move(0.08, 0, 0)
	else
		self:move(-0.08, 0, 0)
	end

	if self.walkToZ > z then
		self:move(0, 0, 0.08)
	else
		self:move(0, 0, -0.08)
	end

	local deltaX = xPlr - x
	local deltaY = zPlr - z

	local angle = math.atan(deltaX, deltaY)
	self:setRotation(-(angle * 180)/math.pi)


	if self.shootDebounce >= self.requiredToShoot then
		local bullet = Projectile:new()
		bullet:init3DModel("actualCube.obj")
		bullet:setScale(0.5, 0.5, 0.5)
		bullet:setPosition(x, y, z)
		local xSpeed = math.sin(angle)*0.7
		local zSpeed = math.cos(angle)*0.7
		bullet.xSpeed = xSpeed
		bullet.zSpeed = zSpeed
		self.shootDebounce = 0

		bullet.currentFrames = Projectile.currentFrames
		bullet.invFrames = Projectile.invFrames

		if #enemyBullets >= 20 then -- removes bullets if too many
			
			table.remove(enemyBullets, 1)
		end

		table.insert(enemyBullets, bullet)
	end
end

function Enemy:dropItems()
	if math.random(1,2) == 1 then

		local speedDrop = SpeedDrop:new()
		speedDrop:init3DModel("actualCube.obj") 
		speedDrop:setScale(0.5, 0.5, 0.5)
		speedDrop:setPosition(self.position.x, self.position.y, self.position.z)
		table.insert(speedDropTable, speedDrop)


		local heartDrop = HeartDrop:new()
		heartDrop:init3DModel("actualCube.obj")
		heartDrop:setScale(0.5, 0.5, 0.5)
		heartDrop:setPosition(self.position.x + (math.random(-2,2)), self.position.y, self.position.z + (math.random(-2,2)))
		table.insert(heartDropTable, heartDrop)
	end
end

function initEnemies()
	for i = 1,2 do
		local myEnemy = Enemy:new()
		myEnemy:init3DModel("ghost.obj") 
		myEnemy:setScale(1,3,1)
		local spawnX, spawnY, spawnZ = spawnPoints[math.random(#spawnPoints)]:getPosition()
		myEnemy:setPosition(spawnX, 0, spawnZ)
		myEnemy.shootDebounce = math.random(0,10)
		enemies[#enemies+1] = myEnemy
	end
end
initEnemies()

w = false 
s = false
a = false
d = false
middleMouseButton = false


function handleBulletCollisions()
	for i,bulletObject in pairs(bullets) do
		for _,enemyObject in pairs (enemies) do

			collision = checkCollision(bulletObject.gameObject, enemyObject.gameObject)
			local x,y,z = enemyObject:getPosition()
			local bX, bY, bZ = bulletObject:getPosition()

			if collision and bX ~= 0 and bZ ~= 0 and bulletObject.currentFrames >= bulletObject.invFrames then 
				enemyObject.health = enemyObject.health - myPlayer.damage
		
				table.remove(bullets, i)
			end
		end
	end

	for i,bulletObject in pairs(enemyBullets) do
		collision = checkCollision(bulletObject.gameObject, myPlayer.gameObject)
		local x,y,z = myPlayer:getPosition()

		if collision and bulletObject.currentFrames >= bulletObject.invFrames then 
			myPlayer.health = myPlayer.health - 10

			table.remove(enemyBullets, i)
		end
	end
end

createArena()
readFile()
function update()
	setCameraPos(x, 25, z)
	local middleX = screenWidth/2
	local middleY = screenHeight/2

	deltaX = middleX-mouseX
	deltaY = middleY-mouseY
	local angle = math.atan(deltaX, deltaY)
	myPlayer:setRotation(-(angle * 180)/math.pi)

	x, y, z = myPlayer.gameObject:getPosition()
	if w and z < arenaSize then -- up
		myPlayer:move(0, 0, 0.06*myPlayer.speed)
	end
	if s and z > -arenaSize then -- down
		myPlayer:move(0, 0, -0.06*myPlayer.speed)
	end
	if a and x < arenaSize then -- left
		myPlayer:move(0.06*myPlayer.speed, 0, 0)
	end

	if d and x > -arenaSize then -- right
		myPlayer:move(-0.06*myPlayer.speed, 0, 0)
	end

	if(myPlayer.speed > Player.speed) then
		myPlayer.speed = myPlayer.speed-0.002;
	end
	
	if not editMode then
		for i,wall in pairs(walls) do
			collided = checkCollision(myPlayer.gameObject, wall.gameObject)
			if collided then
				local objX, objY, objZ = wall.gameObject:getPosition()
				local objSX, objSY, objSZ = wall.gameObject:getScale()

			

				local pX, pY, pZ = myPlayer.gameObject:getPosition()

				if pX < (objX + objSX) or pX < (objX - objSX) then -- object is on the right
					myPlayer:move(-0.06*myPlayer.speed, 0, 0)
				end
			
				if pX > (objX + objSX) or pX > (objX - objSX) then -- object is on the left
					myPlayer:move(0.06*myPlayer.speed, 0, 0)
				end

				if pZ > (objZ + objSZ) or pZ > (objZ - objSZ) then -- object is below
					myPlayer:move(0, 0, 0.06*myPlayer.speed)
				end

				if pZ < (objZ + objSZ) or pZ < (objZ - objSZ) then -- object is above
					myPlayer:move(0, 0, -0.06*myPlayer.speed)
				end
				-- determine which direction and push player out again 
			end
	end

		if mouseButtonOne and myPlayer.shootDebounce >= myPlayer.requiredToShoot then

			local xSpeed = math.sin(angle)*5
			local zSpeed = math.cos(angle)*5

			xPos, yPos, zPos = myPlayer:getPosition()
		
			if myPlayer.selectedWeapon == "AUTO" then
				local bullet = Projectile:new()
				bullet:init3DModel("actualCube.obj")
				bullet:setScale(0.5, 0.5, 0.5)
				bullet:setPosition(xPos, yPos, zPos)
				bullet.xSpeed = xSpeed
				bullet.zSpeed = zSpeed
				bullet.currentFrames = Projectile.currentFrames
				bullet.invFrames = Projectile.invFrames

				table.insert(bullets, bullet)
				myPlayer.shootDebounce = 0
			end

			if myPlayer.selectedWeapon == "SHOTGUN" then
				for i = 1,5 do
					local bullet = Projectile:new()
					bullet:init3DModel("actualCube.obj")
					bullet:setScale(0.5, 0.5, 0.5)
					bullet:setPosition(xPos, yPos, zPos)
					bullet.xSpeed = xSpeed*0.4  + (math.random(-7,7)/10)
					bullet.zSpeed = zSpeed*0.4 + (math.random(-7,7)/10)
					bullet.currentFrames = Projectile.currentFrames
					bullet.invFrames = Projectile.invFrames

					table.insert(bullets, bullet)
					myPlayer.shootDebounce = 0
				end
			end

			if myPlayer.selectedWeapon == "SNIPER" then
				local bullet = Projectile:new()
				bullet:init3DModel("actualCube.obj")
				bullet:setScale(0.2, 0.2, 0.2)
				bullet:setPosition(xPos, yPos, zPos)
				bullet.xSpeed = xSpeed*1.2
				bullet.zSpeed = zSpeed*1.2

				bullet.currentFrames = Projectile.currentFrames
				bullet.invFrames = Projectile.invFrames
				table.insert(bullets, bullet)
				myPlayer.shootDebounce = 0
			end

			if #bullets >= 20 then -- removes bullets if too many
			
				table.remove(bullets, 1)
			end
		end

		for i,v in pairs(bullets) do -- v is the bullet in the array
			local xSpeed = v.xSpeed
			local zSpeed = v.zSpeed
			v.currentFrames = v.currentFrames + 1
			v:move(xSpeed, 0, zSpeed)
		end

		for i,v in pairs(enemyBullets) do -- v is the bullet in the array
			local xSpeed = v.xSpeed
			local zSpeed = v.zSpeed
			v.currentFrames = v.currentFrames + 1
			v:move(xSpeed, 0, zSpeed)
		end

		for i,v in pairs(enemies) do -- update all enemies
		
			if v.health > 0 then
				v:update(myPlayer)

			else -- enemy died
				v:dropItems()
				table.remove(enemies, i)
				score = score + 10
			end

		end


		local x, y, z = myPlayer:getPosition()
	

		if myPlayer.shootDebounce < myPlayer.requiredToShoot then
			myPlayer.shootDebounce = myPlayer.shootDebounce + 0.2
		end

		for i,v in pairs(speedDropTable) do
			local collided = checkCollision(v.gameObject, myPlayer.gameObject)
			if collided then
				myPlayer.speed = myPlayer.speed + 0.2
				table.remove(speedDropTable, i)
			end
		end

		for i,v in pairs(heartDropTable) do
			if checkCollision(v.gameObject, myPlayer.gameObject) then
				myPlayer.health = myPlayer.health + 10
				table.remove(heartDropTable, i)
			end
		end


		for _,enemy in pairs(enemies) do
			if enemy.shootDebounce < enemy.requiredToShoot then
				enemy.shootDebounce = enemy.shootDebounce + 0.2
			end
		end

		setText("Health: ".. myPlayer.health .. "\nSpeed: "..(math.ceil(myPlayer.speed*100)/100).. "\nWeapon Selected: "..myPlayer.selectedWeapon.."\nScore: "..score.."\n".. controls)



		myPlayer:handleCheckWeapon()

		handleBulletCollisions()

		-- spawns new enemies
		respawnTimer = respawnTimer + 1
		if respawnTimer >= spawnWhenTimerEquals then
			local enemy = Enemy:new()

			local type = math.random(1,3)


			if type == 1 then
				enemy:init3DModel("ghost.obj")
				enemy:setScale(1,1,1)
			elseif type == 2 then
				enemy:init3DModel("ghostSmall.obj")
				enemy:setScale((0.6),1,(0.6))
				enemy.health = 60
				enemy.requiredToShoot = Enemy.requiredToShoot-2
			elseif type == 3 then
				enemy:init3DModel("ghostBig.obj")
				enemy:setScale(1.5,1.5,1.5)
				enemy.requiredToShoot = Enemy.requiredToShoot-5
				enemy.health = 130
			end

			local spawnX, spawnY, spawnZ = spawnPoints[math.random(#spawnPoints)]:getPosition()
			enemy:setPosition(spawnX, 1, spawnZ)
			table.insert(enemies, enemy)
		respawnTimer = 0
		end

		for bulletIndex,bullet in pairs(bullets) do
			for _,wall in pairs(walls) do
				coll = checkCollision(bullet.gameObject, wall.gameObject)
				if coll then
					table.remove(bullets, bulletIndex)
				end
			end
		end

		for bulletIndex,bullet in pairs(enemyBullets) do
			for _,wall in pairs(walls) do
				coll = checkCollision(bullet.gameObject, wall.gameObject)
				if coll then
					table.remove(enemyBullets, bulletIndex)
				end
			end
		end

		for obstacleIndex,obstacle in pairs(obstacles) do
			collision = checkCollision(myPlayer.gameObject, obstacle.gameObject)
			if collision then
				myPlayer.health = myPlayer.health - 0.03
				score = score - 0.03
			end
		end

		if not (myPlayer.health > 0) then
			myPlayer:setPosition(0,0,0)
		
			for i,v in pairs(enemies) do
				table.remove(enemies, i)
			end
		
			for bulletIndex,bullet in pairs(enemyBullets) do
				table.remove(enemyBullets,bulletIndex)
			end

			for bulletIndex,bullet in pairs(bullets) do
				table.remove(bullets,bulletIndex)
			end

			for i,v in pairs(speedDropTable) do
				table.remove(speedDropTable,i)
			end

			for i,v in pairs(heartDropTable) do
				table.remove(heartDropTable,i)
			end

			myPlayer.speed = Player.speed
			myPlayer.health = Player.health
			score = 0
		end
	end

	if m  and placeTimer == 0 then
		placeTimer = 0
		editMode = true
		-- removes all objects from scene
		for bulletIndex,enemyBullet in pairs(enemyBullets) do
			enemyBullets[bulletIndex] = nil
		end 

		for i,v in pairs(bullets) do
			bullets[i] = nil
		end

		for i,v in pairs(enemies) do
			enemies[i] = nil
		end

		for i,v in pairs(speedDropTable) do
			speedDropTable[i] = nil
		end

		for i,v in pairs(spawnPoints) do
			spawnPoints[i] = nil
		end
		--secretTextToShowWeMadeThisProgram Jakob, Ghazi and the other dude <3 'Barry'
		for i,v in pairs(heartDropTable) do

			heartDropTable[i] = nil
		end
	end
	

	if editMode then
		if space then
			placeTimer = 0
		--clear all objects in list
			for i,v in pairs(obstacles) do
				obstacles[i] = nil
			end

			for i,v in pairs(spawnPoints) do
				spawnPoints[i] = nil
			end

			for i,v in pairs(walls) do
				walls[i] = nil
			end
			--creates edges
			createArena()
		end

		local x, y, z = getMouseHit()

		 
		if mouseButtonOne and (reqToPlace < placeTimer) then
			placeTimer = 0
			
			local wall = Wall:new()
			wall:init3DModel("treeLua.obj")
			wall:setPosition(x,0,z)
			wall:setScale(1,1,1)
			table.insert(walls, wall)
		end 
		if mouseButtonTwo and (reqToPlace < placeTimer) then
			placeTimer = 0
			local obstacle = Obstacle:new()
			obstacle:init3DModel("obstacle.obj")
			obstacle:setPosition(x,heightFromGround,z)
			obstacle:setScale(2,1,2)
			table.insert(obstacles, obstacle)
		end

		if middleMouseButton and (reqToPlace < placeTimer) then
			placeTimer = 0
			local spawner = Spawnpoint:new()
			spawner:init3DModel("spawnPoint.obj")
			spawner:setPosition(x,heightFromGround,z)
			spawner:setScale(2,1,2)
			table.insert(spawnPoints, spawner)
		end


		if (placeTimer <= reqToPlace) then
			placeTimer = placeTimer + 1
		end

		if editMode and n then
			placeTimer = 0
			editMode = false

			
		end

		--READ FROM FILE
		if F1 and (reqToPlace < placeTimer) then
			readFile()
		end

		--WRITE TO FILE
		if F2 and (reqToPlace < placeTimer) then
			placeTimer = 0
			local file = io.open("save.boll", "w")
			io.output(file)
			--WRITES SPAWNPOINTS TO FILE
			io.write(tostring(#spawnPoints)) -- write nr of spawnpoints first
			io.write("\n")

			for i,v in pairs(spawnPoints) do
				local x, y, z = v.gameObject:getPosition()
				io.write(x)
				io.write("\n")
				io.write(z)
				io.write("\n")
			end
			--WRITES OBSTACLES TO FILE
			io.write(tostring(#obstacles)) -- write nr of obstacles
			io.write("\n")
			for i,v in pairs(obstacles) do
				local x, y, z = v.gameObject:getPosition()
				io.write(x)
				io.write("\n")
				io.write(z)
				io.write("\n")
			end
			--WRITES WALLS TO FILE
			io.write(tostring((#walls)-4)) -- write nr of walls
			io.write("\n")
			for i,v in pairs(walls) do
				if i > 4 then
					local x, y, z = v.gameObject:getPosition()
					io.write(x)
					io.write("\n")
					io.write(z)
					io.write("\n")
				end
			end


			io.close(file)

		end
	end
	collectgarbage("collect")
end