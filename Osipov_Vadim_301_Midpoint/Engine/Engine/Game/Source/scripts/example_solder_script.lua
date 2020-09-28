
-- Called when object is created
-- object - Is a reference to this object
function Start(object)
	return 5 + 7;
end

-- Called every frame, Unity style 
-- object - Is a reference to this object
function Update(object)
    if object.values.isSelected then object:UpdatePosition(0, 1) end
	return 0;
end

-- Called when game object is desroyed
-- object - Is a reference to this object
function Destroy(object)
	return 0;
end

function OnMouseInput(posX, posY, button)
	return 0;
end

function OnKeyboardInput(key)
	return 0;
end

-- called if this object has been selected by mouse click
function OnObjectClicked(object)
	return 0;
end

function OnCollision(other, object)
	return 0;
end
 