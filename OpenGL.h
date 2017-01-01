#include"stdafx.h"

class OpenGL
{
public:	OpenGL();
		virtual ~OpenGL();
public:
	int effect;
	HDC	hDC;			// GDI�豸������
	HGLRC hRC;		// ������ɫ������
	BOOL SetupPixelFormat(HDC hDC);
	void init(int Width, int Height);
	void Render0();
	void Render();
	void CleanUp();
	void LoadTex();
	int Tex(GLuint *texture, int i,const char *Filename);
	void move();
	AUX_RGBImageRec* LoadBMP(const char *Filename);
	int LoadGLTextures(GLuint *texture, int i, const char *Filename);
	void light();
};



