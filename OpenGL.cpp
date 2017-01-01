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
BOOL OpenGL::SetupPixelFormat(HDC hDC0)//��ⰲװOpenGL
{
	int nPixelFormat;					  // ���ص��ʽ
	hDC = hDC0;
	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),    // pfd�ṹ�Ĵ�С 
		1,                                // �汾�� 
		PFD_DRAW_TO_WINDOW |              // ֧���ڴ����л�ͼ 
		PFD_SUPPORT_OPENGL |              // ֧�� OpenGL 
		PFD_DOUBLEBUFFER,                 // ˫����ģʽ 
		PFD_TYPE_RGBA,                    // RGBA ��ɫģʽ 
		16,                               // 24 λ��ɫ��� 
		0, 0, 0, 0, 0, 0,                 // ������ɫλ 
		0,                                // û�з�͸���Ȼ��� 
		0,                                // ������λλ 
		0,                                // ���ۼӻ��� 
		0, 0, 0, 0,                       // �����ۼ�λ 
		16,                               // 32 λ��Ȼ���     
		0,                                // ��ģ�建�� 
		0,                                // �޸������� 
		PFD_MAIN_PLANE,                   // ���� 
		0,                                // ���� 
		0, 0, 0                           // ���Բ�,�ɼ��Ժ������ģ 
	};
	if (!(nPixelFormat = ChoosePixelFormat(hDC, &pfd)))
	{
		MessageBox(NULL, L"û�ҵ����ʵ���ʾģʽ", L"Error", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;
	}
	SetPixelFormat(hDC, nPixelFormat, &pfd);//���õ�ǰ�豸�����ص��ʽ
	hRC = wglCreateContext(hDC);          //��ȡ��Ⱦ�������
	wglMakeCurrent(hDC, hRC);             //������Ⱦ�������
	return TRUE;
}

void OpenGL::CleanUp()
{
	wglMakeCurrent(hDC, NULL);					//���OpenGL
	wglDeleteContext(hRC);						//���OpenGL
}

AUX_RGBImageRec* OpenGL::LoadBMP(const char *Filename)					// ����λͼͼ��
{
	FILE *File = NULL;							// �ļ����
	if (!Filename)								// ȷ���ļ������ṩ
	{
		return NULL;							// ���û�ṩ������ NULL
	}
	File = fopen(Filename, "r");						// ���Դ��ļ�
	if (File)								// �ļ�����ô?
	{
		fclose(File);							// �رվ��
		WCHAR wfilename[256];
		memset(wfilename, 0, sizeof(wfilename));
		MultiByteToWideChar(CP_ACP, 0, Filename, strlen(Filename) + 1, wfilename, sizeof(wfilename) / sizeof(wfilename[0]));
		auxDIBImageLoad(wfilename);
		return auxDIBImageLoad(wfilename);				// ����λͼ������ָ��
	}
	return NULL;								// �������ʧ�ܣ����� NULL
}

int OpenGL::LoadGLTextures(GLuint *texture,int i,const char *Filename)								// ����λͼ(��������Ĵ���)��ת��������
{
	int Status = FALSE;							// ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[1];					// ��������Ĵ洢�ռ�
	memset(TextureImage, 0, sizeof(void *)* 1);				// ��ָ����Ϊ NULL
	// ����λͼ��������޴������λͼû�ҵ����˳�
	if (TextureImage[0] = LoadBMP(Filename))
	{
		Status = TRUE;							// �� Status ��Ϊ TRUE
		glGenTextures(1, texture+i);					// ��������
		// ʹ������λͼ�������� �ĵ�������
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		// ��������
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// �����˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// �����˲�
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	}
	if (TextureImage[0])							// �����Ƿ����
	{
		if (TextureImage[0]->data)					// ����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);				// �ͷ�����ͼ��ռ�õ��ڴ�
		}
		free(TextureImage[0]);						// �ͷ�ͼ��ṹ
	}
	return Status;								// ���� Status
}
int OpenGL::Tex(GLuint *texture,int i,const char *Filename)								// �˴���ʼ��OpenGL������������
{
	
	if (!LoadGLTextures(texture,i,Filename))							// ������������������
	{
		return FALSE;							// ���δ�����룬����FALSE
	}
	glEnable(GL_TEXTURE_2D);						// ��������ӳ��
	glShadeModel(GL_SMOOTH);						// ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					// ��ɫ����
	glClearDepth(1.0f);							// ������Ȼ���
	glEnable(GL_DEPTH_TEST);						// ������Ȳ���
	glDepthFunc(GL_LEQUAL);							// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			// ������ϸ��͸������
	return TRUE;								// ��ʼ�� OK
}

void OpenGL::move(){

	if (KEY_DOWN(27))
		exit(0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);	//�ӿ�
	gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 100000.0);
	gluLookAt(eye[0], eye[1], eye[2], target[0], target[1], target[2], dir[0], dir[1], dir[2]);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGL::Render()//OpenGLͼ�δ���
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//��� COLOR BUFFER �� DEPTH BUFFER

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
	SwapBuffers(hDC);		// �л�������
}

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
		// Ϊÿһ��ASCII�ַ�����һ����ʾ�б�http://blog.csdn.net/shanzhizi
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);   //��ŷֱ���lists, lists + 1, lists + 2, lists + MAX_CHAR -1

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);  //�ӻ���lists��ʼ������ʾ�����ַ�
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// ��������Ȼ���

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
	glMatrixMode(GL_PROJECTION);	//������ģʽ��ΪͶӰ���󣬽������ı任��ֻӰ��ͶӰ����
	glLoadIdentity();	//���þ���
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);	//�ӿ�
	gluPerspective(60.0, (GLfloat)width / (GLfloat)height, 0.1, 10000.0);	//͸��ͶӰ��w/h��֤���ڴ�С�����仯ʱ���治����
	gluLookAt(eye[0], eye[1], eye[2], target[0], target[1], target[2], dir[0], dir[1], dir[2]);	//���������λ�úͷ�λ
	//	glOrtho((GLfloat)-w / 200, (GLfloat)w / 200, (GLfloat)-h / 200, (GLfloat)h / 200, 1.0,20.0);
	glMatrixMode(GL_MODELVIEW);	//������ģʽ���ģ����ͼ
	glLoadIdentity();	//���þ���
}

void OpenGL::LoadTex(){

}

void OpenGL::light(){


}