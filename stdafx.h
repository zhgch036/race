// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件: 
#include <windows.h>

// C 运行时头文件
#include <stdio.h>
#include <mmsystem.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <math.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;

#include <gl/glut.h>
#include <gl/GLAUX.H>
#include <gl/GL.h>


#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")	// OpenGL32连接库
#pragma comment( lib, "glu32.lib")		// GLu32连接库
#pragma comment( lib, "GLAUX.LIB")		// GLaux连接库

// TODO:  在此处引用程序需要的其他头文件
#define MAP		32//	MAP_W*MAP_SCALE/2
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
const double PI = 3.1415926;