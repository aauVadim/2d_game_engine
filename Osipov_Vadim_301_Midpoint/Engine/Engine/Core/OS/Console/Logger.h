#pragma once
#include <cstdarg>

#include <stdio.h>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <string>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class Logger
{
private:
	static Logger* m_pInstance;
	//Console In
	HANDLE m_handleIn;
	FILE* m_pHandleFileIn;

	//Console out
	HANDLE m_handleOut;
	FILE* m_pHandleFileOut;

public:
	enum class LogType
	{
		k_GeneralLog = 0,
		k_Warning,
		k_Error
	};
	static Logger* GetInstance();
	void Shutdown();

#if _DEBUG	
	//---------------------------------------------------------------------------------------------
	// Log Message - Will appear White(Normal) in Console
	//---------------------------------------------------------------------------------------------
	void Log(const char* pMessage, ...);

	//---------------------------------------------------------------------------------------------
	// Log Warning Message - Will appear Yellow in Console 
	//---------------------------------------------------------------------------------------------
	void LogWarning(const char* pMessage, ...);

	//---------------------------------------------------------------------------------------------
	// Log Error Message - Will appear Red in Console
	//---------------------------------------------------------------------------------------------
	void LogError(const char* pMessage, ...);
#else
	void Log(const char*, ...) { }
	void LogWarning(const char*, ...) { }
	void LogError(const char*, ...) { }
#endif

private:
	Logger() { };
	void CreateConsoleWindow();
};