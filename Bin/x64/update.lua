MovableLua = {
	position = {x = 0, y = 0, z = 0},
	gameObject = nil
}
local movableMeta = {__index = MovableLua}

function MovableLua:getPosition(x, y, z)
	self.gameObject:getPosition(x, y, z)
end

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


function MovableLua.new(table)
	local t = table or {}
	t.gameObject = Movable:new(3,1,1) -- lua class is called Movable and is registered as so, takes scale
	setmetatable(t, movableMeta)
	--self.__index = self -- kommer nu kolla i sin egen prototyp när index inte hittas
    --setmetatable(v, self)


	--self.__index = self
	--setmetatable(t, self)
	return t
end
--------------------------------------------------------------------
--PlayerClass = MovableLua:new()
--PlayerClass.health = 100
Projectile = {}
function Projectile.new(xPos, yPos, zPos, xSpeed, zSpeed)
	local t = MovableLua.new()
	--t.gameObject = Movable.new(3,1,1)
	t.xSpeed = xSpeed
	t.zSpeed = zSpeed

	t:setPosition(xPos, yPos, zPos)
	return t
end

local myPlayer = MovableLua.new()--PlayerClass.new(nil, 0, 0, 0, 1, 1, 1)
--myPlayer.Health = 100

bullets = {}
w = false
s = false
a = false
d = false

local shootDebounce = 0
local requiredToShoot = 5

--[[Vector = {x = 0, y = 0, z = 0}
VectorMeta = {__index = Vector}
setmetatable(Vector, VectorMeta)

function Vector3:new(v)
	if not v then
		v = {}
	end

	self.__index = self

	setmetatable(v, self)

	return v -- return instance
end

function Vector3:Length()
{
	return math.sqrt(self.x^2 + self.y^2 + self.z^2)
}

function Vector3:Normalize()
	local length = self:Length()
	return Vector3.new(self.x/length, self.y/length, self.z/length)
end]]

function update()
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

		local xSpeed = math.sin(angle)
		local zSpeed = math.cos(angle)

		xPos, yPos, zPos = myPlayer:getPosition()
		
		bullet = Projectile.new(xPos, yPos, zPos, xSpeed, zSpeed)
		bullet:setScale(0.5, 0.5, 0.5)

		table.insert(bullets, bullet)
		shootDebounce = 0
	end

	for i,v in pairs(bullets) do -- v is the bullet in the array
		local xSpeed = v.xSpeed
		local zSpeed = v.zSpeed
		v:move(xSpeed, 0, zSpeed)
	end

	local x, y, z = myPlayer:getPosition()
	setCameraPos(x, 25, z)

	if shootDebounce < requiredToShoot then
		shootDebounce = shootDebounce + 0.05
	end

end