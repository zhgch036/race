#include"stdafx.h"
#include"3DS.h"

void Obj::LoadObj(){
	fstream fin(obj_name.c_str(), ios::in);
	if (!fin){
		cout << "open file error" << endl;
		return;
	}
	string line;
	Group temp_group;
	Vertex temp_vertex;
	Normal temp_normal;
	Uv temp_uv;
	Face temp_face;
	char temp;
	while (!fin.eof()){
		fin >> line;
		switch (line[0]){
		case 'g':
			fin >> temp_group.group_name;
			break;
		case 'u':
			fin >> temp_group.mtl_name;
			break;
		case 'v':
			if (line == "v"){
				fin >> temp_vertex.x;
				fin >> temp_vertex.y;
				fin >> temp_vertex.z;
				vertexs.push_back(temp_vertex);
			}
			else if (line == "vn"){
				fin >> temp_normal.x;
				fin >> temp_normal.y;
				fin >> temp_normal.z;
				normals.push_back(temp_normal);
			}
			else if (line == "vt"){
				fin >> temp_uv.u;
				fin >> temp_uv.v;
				fin >> temp_uv.w;
				uvs.push_back(temp_uv);
			}				
			else
				break;
			break;
		case 'f':
			for (unsigned int i = 0; i < 3; i++){
				fin >> temp_face.point[i].vertex_id;
				fin >> temp;
				fin >> temp_face.point[i].texture_id;
				fin >> temp;
				fin >> temp_face.point[i].normal_id;
			}
			temp_group.faces.push_back(temp_face);
			break;
		case '#':
			getline(fin, line);
			if (line.find("faces") < line.length()){
				groups.push_back(temp_group);
				temp_group.faces.clear();
			}
			break;
		default:
			getline(fin, line);
			break;
		}
	}
	fin.close();
}


void Mtl::LoadMtl(){
	fstream fin(mtl_name, ios::in);
	if (!fin){
		cout << "file open error" << endl;
		return;
	}
	string line;
	Meterial temp_meterial;
	while (!fin.eof()){
		fin >> line;
		if (line == "newmtl"){
			fin >> temp_meterial.meterial_name;
		}
		else if (line == "map_Ka"){
			fin >> temp_meterial.texture_name;
			meterials.push_back(temp_meterial);
		}
		else
			getline(fin, line);
	}
	fin.close();
}

void Model::DrawModel(){
	int vertex_id,normal_id,uv_id,texture_id;
	unsigned int group_num, face_num;
	glEnable(GL_TEXTURE_2D);
	for (group_num = 0; group_num < obj.groups.size(); group_num++){
		texture_id = mtl.GetTextureID(obj.groups[group_num].mtl_name);
		glBindTexture(GL_TEXTURE_2D, texture[texture_id]);
		glBegin(GL_TRIANGLES);
		for (face_num = 0; face_num < obj.groups[group_num].faces.size(); face_num++){
			vertex_id = obj.groups[group_num].faces[face_num].point[0].vertex_id - 1;
			normal_id = obj.groups[group_num].faces[face_num].point[0].normal_id - 1;
			uv_id = obj.groups[group_num].faces[face_num].point[0].texture_id - 1;
			glTexCoord2f(obj.uvs[uv_id].u, obj.uvs[uv_id].v);
			glNormal3f(obj.normals[normal_id].x, obj.normals[normal_id].y, obj.normals[normal_id].z);
			glVertex3f(obj.vertexs[vertex_id].x, obj.vertexs[vertex_id].y, obj.vertexs[vertex_id].z);
			
			vertex_id = obj.groups[group_num].faces[face_num].point[1].vertex_id - 1;
			normal_id = obj.groups[group_num].faces[face_num].point[1].normal_id - 1;
			uv_id = obj.groups[group_num].faces[face_num].point[1].texture_id - 1;
			glTexCoord2f(obj.uvs[uv_id].u, obj.uvs[uv_id].v);
			glNormal3f(obj.normals[normal_id].x, obj.normals[normal_id].y, obj.normals[normal_id].z);
			glVertex3f(obj.vertexs[vertex_id].x, obj.vertexs[vertex_id].y, obj.vertexs[vertex_id].z);
			
			vertex_id = obj.groups[group_num].faces[face_num].point[2].vertex_id - 1;
			normal_id = obj.groups[group_num].faces[face_num].point[2].normal_id - 1;
			uv_id = obj.groups[group_num].faces[face_num].point[2].texture_id - 1;
			glTexCoord2f(obj.uvs[uv_id].u, obj.uvs[uv_id].v);
			glNormal3f(obj.normals[normal_id].x, obj.normals[normal_id].y, obj.normals[normal_id].z);
			glVertex3f(obj.vertexs[vertex_id].x, obj.vertexs[vertex_id].y, obj.vertexs[vertex_id].z);		
		}
		glEnd();
	}
	glFlush();
}

Model::Model(string foder_name, string model_name){
	this->folder_name = foder_name;
	obj.obj_name = foder_name + model_name + ".obj";
	mtl.mtl_name = foder_name + model_name + ".mtl";
	obj.LoadObj();
	mtl.LoadMtl();
	texture = new GLuint[mtl.meterials.size()];
}


Model::~Model(){

}

int Mtl::GetTextureID(string meterial_name){
	for (unsigned int i = 0; i < meterials.size(); i++){
		if (meterial_name == meterials[i].meterial_name)
			return i;
	}
	return -1;
}