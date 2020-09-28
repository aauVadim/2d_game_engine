#pragma once

//-------------------------------------------------------------------------------------------------
// Pure virtual Class needed for Renderer Component to know about GORenderer
//-------------------------------------------------------------------------------------------------

class IGORenderer
{
public:
	virtual ~IGORenderer() { }
    virtual void SetVisible(bool isVisible) = 0;
    virtual bool IsVisible() const = 0;
};