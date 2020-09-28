#pragma once

//Temporary class enum that is used as configuration file
//In the future it will be moved to .xml or .ini
enum class PlatformConfiguration
{
	g_kSDL = 0,
	g_kDirectX,
	g_kOpenGL
};


const int g_kCameraRectSize = 50;
//Resolution: 
const int g_kWindowWidth = 800;
const int g_kWindowHeight = 600;
