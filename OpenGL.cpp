#include"stdafx.h"
#include"OpenGL.h"
#include"3DS.h"

#define MAX_CHAR    128
GLuint TextFont;
extern int width, height;
extern GLuint sky_texture;
extern GLuint box_texture;

GLdouble speed = 0.0;
const GLdouble radius = 50;
GLdouble eye[3] = { 600.0, -60.0, 50.0 }, target[3] = { 0.0, 90.0, 5.0 - radius }, dir[3] = { 0.0, 1.0, 0.0 };
GLdouble angle_x=PI/2, angle_y=0.0;

picture pic;
Model model("model\\track\\", "racetrack");
Model car("model\\car\\", "car");

OpenGL::OpenGL()
{
}
OpenGL::~OpenGL()
{
	CleanUp();
}
BOOL OpenGL::SetupPixelFormat(HDC hDC0)//检测安装OpenGL
{
	int nPixelFormat;					  // 象素点格式
	hDC = hDC0;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd结构的大小 
		1,                                // 版本号 
		PFD_DRAW_TO_WINDOW |              // 支持在窗口中绘图 
		PFD_SUPPORT_OPENGL |              // 支持 OpenGL 
		PFD_DOUBLEBUFFER,                 // 双缓存模式 
		PFD_TYPE_RGBA,                    // RGBA 颜色模式 
		16,                               // 24 位颜色深度 
		0, 0, 0, 0, 0, 0,                 // 忽略颜色位 
		0,                                // 没有非透明度缓存 
		0,                                // 忽略移位位 
		0,                                // 无累加缓存 
		0, 0, 0, 0,                       // 忽略累加位 
		16,                               // 32 位深度缓存     
		0,                                // 无模板缓存 
		0,                                // 无辅助缓存 
		PFD_MAIN_PLANE,                   // 主层 
		0,                                // 保留 
		0, 0, 0                           // 忽略层,可见性和损毁掩模 
	};
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"没找到合适的显示模式", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	SetPixelFormat(hDC, nPixelFormat, &pfd);//设置当前设备的像素点格式
	hRC = wglCreateContext(hDC);          //获取渲染描述句柄
	wglMakeCurrent(hDC, hRC);             //激活渲染描述句柄
	return TRUE;
}

void OpenGL::CleanUp()
{
	wglMakeCurrent(hDC, NULL);					//清除OpenGL
	wglDeleteContext(hRC);						//清除OpenGL
}

AUX_RGBImageRec* OpenGL::LoadBMP(const char *Filename)					// 载入位图图象
{
	FILE *File = NULL;							// 文件句柄
	if (!Filename)								// 确保文件名已提供
	{
		return NULL;							// 如果没提供，返回 NULL
	}
	File = fopen(Filename, "r");						// 尝试打开文件
	if (File)								// 文件存在么?
	{
		fclose(File);							// 关闭句柄
		WCHAR wfilename[256];
		memset(wfilename, 0, sizeof(wfilename));
		MultiByteToWideChar(CP_ACP, 0, Filename, strlen(Filename) + 1, wfilename, sizeof(wfilename) / sizeof(wfilename[0]));
		auxDIBImageLoad(wfilename);
		return auxDIBImageLoad(wfilename);				// 载入位图并返回指针
	}
	return NULL;								// 如果载入失败，返回 NULL
}

int OpenGL::LoadGLTextures(GLuint *texture,int i,const char *Filename)								// 载入位图(调用上面的代码)并转换成纹理
{
	int Status = FALSE;							// 状态指示器
	AUX_RGBImageRec *TextureImage[1];					// 创建纹理的存储空间
	memset(TextureImage, 0, sizeof(void *)* 1);				// 将指针设为 NULL
	// 载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage[0] = LoadBMP(Filename))
	{
		Status = TRUE;							// 将 Status 设为 TRUE
		glGenTextures(1, texture+i);					// 创建纹理
		// 使用来自位图数据生成 的典型纹理
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// 线形滤波
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// 线形滤波
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	if (TextureImage[0])							// 纹理是否存在
	{
		if (TextureImage[0]->data)					// 纹理图像是否存在
		{
			free(TextureImage[0]->data);				// 释放纹理图像占用的内存
		}
		free(TextureImage[0]);						// 释放图像结构
	}
	return Status;								// 返回 Status
}
int OpenGL::Tex(GLuint *texture,int i,const char *Filename)								// 此处开始对OpenGL进行所有设置
{
	
	if (!LoadGLTextures(texture,i,Filename))							// 调用纹理载入子例程
	{
		return FALSE;							// 如果未能载入，返回FALSE
	}
	glEnable(GL_TEXTURE_2D);						// 启用纹理映射
	glShadeModel(GL_SMOOTH);						// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// 黑色背景
	glClearDepth(1.0f);							// 设置深度缓存
	glEnable(GL_DEPTH_TEST);						// 启用深度测试
	glDepthFunc(GL_LEQUAL);							// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// 真正精细的透视修正
	return TRUE;								// 初始化 OK
}

void OpenGL::move(){

	if (KEY_DOWN(27))
		exit(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);	//视口
	gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 100000.0);
	gluLookAt(eye[0], eye[1], eye[2], target[0], target[1], target[2], dir[0], dir[1], dir[2]);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGL::Render()//OpenGL图形处理
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除 COLOR BUFFER 和 DEPTH BUFFER

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0, 0.0, -150.0);
//	model.DrawModel();
	glLoadIdentity();
	glTranslatef(eye[0], eye[1], eye[2]);
	glRotatef(angle_x*180.0 / PI+90.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, -5.0, -12.0);
//	car.DrawModel();

	glPopMatrix();
	SwapBuffers(hDC);		// 切换缓冲区
}

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
		// 为每一个ASCII字符产生一个显示列表http://blog.csdn.net/shanzhizi
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);   //编号分别是lists, lists + 1, lists + 2, lists + MAX_CHAR -1

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);  //从基数lists开始依次显示各个字符
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}


void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void OpenGL::Render0(){

	selectFont(48, ANSI_CHARSET, "Comic Sans MS");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// 清屏和深度缓冲

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);

	
	glColor3f(1.0, 1.0, 1.0);

	glRasterPos3f(0.30, 0.5, 0.0);
	drawString("Press Space To Start!");
	SwapBuffers(hDC);
}

void OpenGL::init(int width, int height)
{	
	effect = 0;
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);	//将矩阵模式改为投影矩阵，接下来的变换将只影响投影矩阵
	glLoadIdentity();	//重置矩阵
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);	//视口
	gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 10000.0);	//透视投影，w/h保证窗口大小发生变化时画面不变形
	gluLookAt(eye[0], eye[1], eye[2], target[0], target[1], target[2], dir[0], dir[1], dir[2]);	//设置摄像机位置和方位
	//	glOrtho((GLfloat)-w / 200, (GLfloat)w / 200, (GLfloat)-h / 200, (GLfloat)h / 200, 1.0,20.0);
	glMatrixMode(GL_MODELVIEW);	//将矩阵模式变回模型视图
	glLoadIdentity();	//重置矩阵
}

void OpenGL::LoadTex(){

}

void OpenGL::light(){


}