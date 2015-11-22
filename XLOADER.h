
/*
	Created by Akimatsu_404
	FileName :  XLOADER.h
*/

#ifndef _X_LOADER_
#define _X_LOADER_

#include <vector>
#include <string>
#include <assert.h>
#include <glm\glm.hpp>

#include "png\png.h"
#include "../glut.h"


struct VECTOR3 {
	float x, y, z;
};

struct VECTOR4 {
	float x, y, z, w;
};

struct VERTEX {
	float x, y, z;
};

struct NORMAL {
	float x, y, z;
};

struct TEXCOORD {
	float u, v;
};

struct MATERIAL {

	VECTOR4 diffuse;
	float shininess;
	VECTOR3 specular;
	VECTOR3 ambient;

	int texNo;
	std::string texFileName;
};


struct NODE {

	std::string nodeName;
	glm::mat4 nodeMat;

	std::vector<VERTEX> vertexArray;
	std::vector<NORMAL> normalArray;
	std::vector<TEXCOORD> texcoordArray;
	std::vector<MATERIAL> materialArray;
	std::vector<int> indexArray;

	std::vector<TEXTURE> textureArray;
	std::vector<GLuint> texID;

};

class X_MODEL {

	std::vector<NODE> m_nodeArray;
	std::vector<glm::mat4> m_nodeMatrix;

public:
	
	X_MODEL(const char* FileName);
	~X_MODEL();

	void Load(const char* FileName);
	void Update();//各ノードの行列の処理を行う。(主に回転)
	void Draw();

};


/*
カメラ行列の回転行列のみを参照して、上部分と砲身を回転させる。

*/

#endif