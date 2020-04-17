--myEnemy = Enemy.new("Berra")
myPlayer = Player.new("Mr")

w = false
s = false

function update()

	if w then
		myPlayer:move(0.01, 0, 0)
	end
	if s then
		myPlayer:move(-0.01, 0, 0)
	end
	--myEnemy:move(0.001, 0, 0)
	--print("This is working")
end