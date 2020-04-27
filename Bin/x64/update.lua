bullets = {}
enemyBullets = {}
enemies = {}
speedDropTable = {}
heartDropTable = {}

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

	--print("INIT 3D MODEL", path, sx, sy, sz)
	
	self.gameObject = Movable:new(sx, sy, sz, path) -- lua class is called Movable and is registered as so, takes scale

end
--------------------------------------------------------------------
--debug obj's
--12272_Koala_v1_L3.obj
--Heart.obj

Projectile = MovableLua:new() -- this is how you do inheritance 💥🧠💥  
Projectile.xSpeed = 0
Projectile.zSpeed = 0

SpeedDrop = MovableLua:new()
HeartDrop = MovableLua:new()

Player = MovableLua:new()
Player.speed = 0.5
Player.health = 100

Enemy = MovableLua:new()
Enemy.speed = 0.5
Enemy.health = 5
Enemy.shootDebounce = 0
Enemy.requiredToShoot = 10

function Enemy:update(playerVar)
	local x, y, z = self:getPosition()
	local xPlr, yPlr, zPlr = playerVar:getPosition()

	self:move(math.random(-20,20)/100, 0, math.random(-20,20)/100)
	local deltaX = xPlr - x
	local deltaY = zPlr - z

	local angle = math.atan(deltaX, deltaY)
	self:setRotation(-(angle * 180)/math.pi)

	--self.shootDebounce = 0
	--print(self.shootDebounce)
	if self.shootDebounce >= self.requiredToShoot then
		local bullet = Projectile:new()
		bullet:init3DModel("actualCube.obj")
		bullet:setScale(0.5, 0.5, 0.5)
		bullet:setPosition(x, y, z)
		local xSpeed = math.sin(angle)*0.1
		local zSpeed = math.cos(angle)*0.1
		bullet.xSpeed = xSpeed
		bullet.zSpeed = zSpeed
		self.shootDebounce = 0

		if #enemyBullets >= 20 then -- removes bullets if too many
			
			table.remove(enemyBullets, 1)
		end

		table.insert(enemyBullets, bullet)
	end
end

function Enemy:dropItems()
	local speedDrop = SpeedDrop:new()
	speedDrop:init3DModel("actualCube.obj") 
	speedDrop:setScale(0.5, 0.5, 0.5)
	speedDrop:setPosition(self.position.x, self.position.y, self.position.z)
	table.insert(speedDropTable, speedDrop)


	local heartDrop = HeartDrop:new()
	heartDrop:init3DModel("actualCube.obj")
	heartDrop:setScale(0.5, 0.5, 0.5)
	heartDrop:setPosition(self.position.x + math.random(-40,40)/10, self.position.y, self.position.z + math.random(-40,40)/10)
	table.insert(heartDropTable, heartDrop)
end

myPlayer = Player:new()--PlayerClass.new(nil, 0, 0, 0, 1, 1, 1)
myPlayer:init3DModel()
myPlayer:setPosition(0,0,0)

function initEnemies()
	for i = 1,2 do
		local myEnemy = Enemy:new()
		myEnemy:init3DModel("actualCube.obj") 
		myEnemy:setPosition(5,0,5)
		myEnemy.shootDebounce = math.random(0,10)
		enemies[#enemies+1] = myEnemy
	end
end
initEnemies()

w = false
s = false
a = false
d = false

local shootDebounce = 0
local requiredToShoot = 5

function handleBulletCollisions()
	for i,bulletObject in pairs(bullets) do
		for _,enemyObject in pairs (enemies) do
			collision = checkCollision(bulletObject.gameObject, enemyObject.gameObject)
			local x,y,z = enemyObject:getPosition()

			if collision then 
				enemyObject.health = enemyObject.health - 10
		
				table.remove(bullets, i)
			end
		end
	end

	for i,bulletObject in pairs(enemyBullets) do
		collision = checkCollision(bulletObject.gameObject, myPlayer.gameObject)
		local x,y,z = myPlayer:getPosition()

		if collision then 
			myPlayer.health = myPlayer.health - 10

			table.remove(enemyBullets, i)
		end
	end
end

function update()
	--print("Collision: ", checkCollision(myEnemy.gameObject, myPlayer.gameObject))
	local middleX = screenWidth/2
	local middleY = screenHeight/2

	deltaX = middleX-mouseX
	deltaY = middleY-mouseY
	local angle = math.atan(deltaX, deltaY)
	myPlayer:setRotation(-(angle * 180)/math.pi)

	if w then
		myPlayer:move(0, 0, 0.06)
	end
	if s then
		myPlayer:move(0, 0, -0.06)
	end

	if a then
		myPlayer:move(0.06, 0, 0)
	end

	if d then
		myPlayer:move(-0.06, 0, 0)
	end

	--if space then
		
	--end

	if mouseButtonOne and shootDebounce >= requiredToShoot then

		local xSpeed = math.sin(angle)*0.5
		local zSpeed = math.cos(angle)*0.5

		xPos, yPos, zPos = myPlayer:getPosition()
		
		local bullet = Projectile:new()
		bullet:init3DModel("actualCube.obj")
		bullet:setScale(0.5, 0.5, 0.5)
		bullet:setPosition(xPos, yPos, zPos)
		bullet.xSpeed = xSpeed
		bullet.zSpeed = zSpeed

		table.insert(bullets, bullet)
		shootDebounce = 0

		if #bullets >= 20 then -- removes bullets if too many
			
			table.remove(bullets, 1)
		end
	end

	for i,v in pairs(bullets) do -- v is the bullet in the array
		local xSpeed = v.xSpeed
		local zSpeed = v.zSpeed
		v:move(xSpeed, 0, zSpeed)
	end

	for i,v in pairs(enemyBullets) do -- v is the bullet in the array
		local xSpeed = v.xSpeed
		local zSpeed = v.zSpeed
		v:move(xSpeed, 0, zSpeed)
	end

	for i,v in pairs(enemies) do -- update all enemies
		if v.health > 0 then
			v:update(myPlayer)
		else -- enemy died
			v:dropItems()
			table.remove(enemies, i)
		end
	end


	local x, y, z = myPlayer:getPosition()
	setCameraPos(x, 25, z)

	if shootDebounce < requiredToShoot then
		shootDebounce = shootDebounce + 0.05
	end

	--print(#speedDropTable)
	for i,v in pairs(speedDropTable) do
		local collided = checkCollision(v.gameObject, myPlayer.gameObject)
		if collided then
			myPlayer.speed = myPlayer.speed + 1
			table.remove(speedDropTable, i)
			print("REMOVED", i, "Remaining:", #speedDropTable)
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
			enemy.shootDebounce = enemy.shootDebounce + 0.05
		end
	end

	handleBulletCollisions()
	collectgarbage("collect")
end