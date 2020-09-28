#pragma once

class FPSTimer
{
public:
	virtual ~FPSTimer() { }
	virtual bool Update() = 0;
	virtual int GetTickCount() = 0;
};