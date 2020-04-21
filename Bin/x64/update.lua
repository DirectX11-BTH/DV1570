myEnemy = Enemy.new("Berra")
myPlayer = Player.new("Mr")

myEnemy:setPosition(10, 0, 0)

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

	if space then
		
	end

	if mouseButtonOne and shootDebounce >= requiredToShoot then
		local xSpeed = math.sin(angle)
		local zSpeed = math.cos(angle)

		xPos, yPos, zPos = myPlayer:getPosition()
		
		bullet = Projectile.new(xPos, yPos, zPos, xSpeed, zSpeed)

		table.insert(bullets, bullet)
		shootDebounce = 0
	end

	for i,v in pairs(bullets) do -- v is the bullet in the array
		local xSpeed, zSpeed = v:getVelocity()
		v:move(xSpeed, 0, zSpeed)
	end

	local x, y, z = myPlayer:getPosition()
	setCameraPos(x, 25, z)

	if shootDebounce < requiredToShoot then
		shootDebounce = shootDebounce + 0.05
	end

end