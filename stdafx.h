// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�: 
#include <windows.h>

// C ����ʱͷ�ļ�
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
#pragma comment( lib, "opengl32.lib")	// OpenGL32���ӿ�
#pragma comment( lib, "glu32.lib")		// GLu32���ӿ�
#pragma comment( lib, "GLAUX.LIB")		// GLaux���ӿ�

// TODO:  �ڴ˴����ó�����Ҫ������ͷ�ļ�
#define MAP		32//	MAP_W*MAP_SCALE/2
#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
const double PI = 3.1415926;