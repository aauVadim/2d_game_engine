#include "Logger.h"
#include "../../UtilMacros.h"

Logger* Logger::m_pInstance = nullptr;


using namespace std;
//Found on internet
namespace ConsoleForeground
{
	enum {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};
}

Logger* Logger::GetInstance() 
{
	if (m_pInstance == nullptr)
	{
		m_pInstance = new Logger();
#if _DEBUG
		m_pInstance->CreateConsoleWindow();
#endif
	}
	return m_pInstance;
}

//-------------------------------------------------------------------------------------------------
// Used to create Console window
//-------------------------------------------------------------------------------------------------
void Logger::CreateConsoleWindow()
{
	#pragma warning (disable: 4311)
	#pragma warning (disable: 4302)
	AllocConsole();

	m_handleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	int hCrt = _open_osfhandle((long)m_handleOut, _O_TEXT);
	m_pHandleFileOut = _fdopen(hCrt, "w");
	setvbuf(m_pHandleFileOut, NULL, _IONBF, 1);
	*stdout = *m_pHandleFileOut;
	SetConsoleTextAttribute(m_pHandleFileOut, ConsoleForeground::BLUE);
	m_handleIn = GetStdHandle(STD_INPUT_HANDLE);
	hCrt = _open_osfhandle((long)m_handleIn, _O_TEXT);
	m_pHandleFileIn = _fdopen(hCrt, "r");
	setvbuf(m_pHandleFileIn, NULL, _IONBF, 128);
	*stdin = *m_pHandleFileIn;
}
#if _DEBUG
void Logger::LogWarning(const char* pMessage, ...)
{
	SetConsoleTextAttribute(m_handleOut, ConsoleForeground::YELLOW);
	va_list args;
	va_start(args, pMessage);
	fprintf(m_pHandleFileOut, "WARNING: ");
	vfprintf(m_pHandleFileOut, pMessage, args);
	fprintf(m_pHandleFileOut, "\n");
	va_end(args);
	SetConsoleTextAttribute(m_handleOut, ConsoleForeground::WHITE);
}

void Logger::LogError(const char* pMessage, ...)
{
	SetConsoleTextAttribute(m_handleOut, ConsoleForeground::RED);

	va_list args;
	va_start(args, pMessage);
	fprintf(m_pHandleFileOut, "ERROR: ");
	vfprintf(m_pHandleFileOut, pMessage, args);
	fprintf(m_pHandleFileOut, "\n");
	va_end(args);

	SetConsoleTextAttribute(m_handleOut, ConsoleForeground::WHITE);
}

void Logger::Log(const char* pMessage, ...)
{
	SetConsoleTextAttribute(m_handleOut, ConsoleForeground::WHITE);

	va_list args;
	va_start(args, pMessage);
	fprintf(m_pHandleFileOut, "LOG: ");
	vfprintf(m_pHandleFileOut, pMessage, args);
	fprintf(m_pHandleFileOut, "\n");
	va_end(args);

	//Cleaning after print
	SetConsoleTextAttribute(m_handleOut, ConsoleForeground::WHITE);
}
#endif
void Logger::Shutdown()
{
	delete this;
}
