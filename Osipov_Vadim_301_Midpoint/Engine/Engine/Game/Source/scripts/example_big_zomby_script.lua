-- Just some variables I've come up with, not related to c++. 
togoPosX = 0;
togoPosY = 0;
movementSpeed = 0.5;

-- Called when object is created
function Start(object)
	return 5 + 7;
end

-- Called every frame, Unity style 
-- "object" - Reference 
function Update(object)
	-- If Object is selected
	if object.values.isSelected then 	
		-- Essentially move position is 0, so you have to check if its not 0. 
		if object.values.movePosX ~= 0 and object.values.movePosY ~= 0 then 
			newPosX = (object.values.movePosX - object.values.posX) * 0.01
			newPosY = (object.values.movePosY - object.values.posY) * 0.01
			object:UpdatePosition(newPosX, newPosY); -- Just move some direction
			object:RotateToVector(object.values.movePosX, object.values.movePosY);
		end
	end
	return 0;
end

-- Called when game object is desroyed
function Destroy(object)
	-- when object destroyed, close the window. 
	object:RequestCloseWindow()
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
	if key == 32 then -- If space bar
		object:InstantiateGameObject(10, object.values.posX, object.values.posY);
	end
	return 0;
end

-- called if this object has been selected by mouse click
function OnObjectSelect(object)
	
	return 0;
end 

-- Called when object collided with something
function OnCollision(other, object)
	if other ~= nil then other:DestroyObject()
	else object:DestroyObject() end
	return 0;
end