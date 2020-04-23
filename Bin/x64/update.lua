bullets = {}
enemies = {}

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

function MovableLua:init3DModel()
	self.gameObject = Movable:new(3,1,1) -- lua class is called Movable and is registered as so, takes scale
end
--------------------------------------------------------------------

Projectile = MovableLua:new() -- this is how you do inheritance 💥🧠💥
Projectile.xSpeed = 0
Projectile.zSpeed = 0

Player = MovableLua:new()
Player.speed = 0.5
Player.health = 100

Enemy = MovableLua:new()
Enemy.speed = 0.5
Enemy.health = 100
Enemy.requiredToShoot = 200

function Enemy:update(playerVar)
	--print(playerVar.position.X)
	local x, y, z = self:getPosition()
	local xPlr, yPlr, zPlr = playerVar:getPosition()

	self:move(math.random(-20,20)/100, 0, math.random(-20,20)/100)
	local deltaX = xPlr - x
	local deltaY = zPlr - z

	local angle = math.atan(deltaX, deltaY)
	self:setRotation(-(angle * 180)/math.pi)
end

myPlayer = Player:new()--PlayerClass.new(nil, 0, 0, 0, 1, 1, 1)
myPlayer:init3DModel()

myEnemy = Enemy:new()
myEnemy:init3DModel()
enemies[#enemies+1] = myEnemy

--myPlayer:init3DModel()

--local myEnemy = Enemy.new()--PlayerClass.new(nil, 0, 0, 0, 1, 1, 1)
--myEnemy:init3DModel()
--myPlayer.Health = 100



w = false
s = false
a = false
d = false

local shootDebounce = 0
local requiredToShoot = 5



function update()
	print("Collision: ", checkCollision(myEnemy.gameObject, myPlayer.gameObject))
	local middleX = screenWidth/2
	local middleY = screenHeight/2

	deltaX = middleX-mouseX
	deltaY = middleY-mouseY
	local angle = math.atan(deltaX, deltaY)
	myPlayer:setRotation(-(angle * 180)/math.pi)

	if w then
		myPlayer:move(0, 0, 0.03)
	end
	if s then
		myPlayer:move(0, 0, -0.03)
	end

	if a then
		myPlayer:move(0.03, 0, 0)
	end

	if d then
		myPlayer:move(-0.03, 0, 0)
	end

	--if space then
		
	--end

	if mouseButtonOne and shootDebounce >= requiredToShoot then

		local xSpeed = math.sin(angle)*0.5
		local zSpeed = math.cos(angle)*0.5

		xPos, yPos, zPos = myPlayer:getPosition()
		
		local bullet = Projectile:new()
		bullet:init3DModel()
		bullet:setScale(0.5, 0.5, 0.5)
		bullet:setPosition(xPos, yPos, zPos)
		bullet.xSpeed = xSpeed
		bullet.zSpeed = zSpeed

		table.insert(bullets, bullet)
		shootDebounce = 0
	end

	for i,v in pairs(bullets) do -- v is the bullet in the array
		local xSpeed = v.xSpeed
		local zSpeed = v.zSpeed
		v:move(xSpeed, 0, zSpeed)
	end

	for i,v in pairs(enemies) do
		v:update(myPlayer)
	end

	local x, y, z = myPlayer:getPosition()
	setCameraPos(x, 25, z)

	if shootDebounce < requiredToShoot then
		shootDebounce = shootDebounce + 0.05
	end

end