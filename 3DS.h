
#include"stdafx.h"

class Vertex{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class Normal{
public:
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

class Uv{
public:
	GLfloat u;
	GLfloat v;
	GLfloat w;
};

class Point{
public:
	int vertex_id;
	int normal_id;
	int texture_id;
};

class Face{
public:
	Point point[3];
};

class Group{
public:
	string group_name;
	string mtl_name;
	vector<Face>faces;
};

class Meterial{
public:
	string meterial_name;
	string texture_name;
};

class Mtl{
public:
	string mtl_name;
	vector<Meterial>meterials;
public:
	void LoadMtl();
	int GetTextureID(string meterial_name);
};

class Obj{
public:
	string obj_name;
	vector<Group>groups;
	vector<Vertex>vertexs;
	vector<Normal>normals;
	vector<Uv>uvs;
public:
	void LoadObj();
};

class Model{
public:
	string folder_name;
	GLuint *texture;
	Obj obj;
	Mtl mtl;
	Model(string foder_name, string model_name);
	~Model();
	void DrawModel();
};
