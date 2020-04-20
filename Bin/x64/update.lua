--myEnemy = Enemy.new("Berra")
myPlayer = Player.new("Mr")

w = false
s = false
a = false
d = false

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
	--print(deltaX, deltaY)
	local angle = math.atan(deltaX, deltaY)--math.atan(deltaY, deltaX)
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

	local x, y, z = myPlayer:getPosition()
	setCameraPos(x, 25, z)
	--myEnemy:move(0.001, 0, 0)
	--print("This is working")
end