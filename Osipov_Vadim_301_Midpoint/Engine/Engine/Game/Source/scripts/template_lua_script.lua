-- Called when object is created
function Start(object)
	return 0;
end

-- Called every frame, Unity style 
-- "object" - Reference 
function Update(object)
	return 0;
end

-- Called when game object is desroyed
function Destroy(object)
	return 0;
end

-- button is passed as number
-- Called once per mouse click on screen. 
-- # Buttons: 1 - LMB, 2 - RMB, 3 - MMB
function OnMouseInput(posX, posY, button, object)
	return 0;
end

-- key is passed as number. 
-- Called once per keyboard input while screen is active
function OnKeyboardInput(key, object)
	return 0;
end

-- called if this object has been selected by mouse click
function OnObjectSelect(object)
	return 0;
end 

-- Called when object collided with something
function OnCollision(other, object)
	return 0;
end