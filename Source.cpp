#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <vector>
#include <Windows.h>
#include "glut.h"


struct TEXRGB {

	char R;
	char G;
	char B;
	char A;

};


GLuint textures[1];

TEXRGB* texrgb;

char type[2] = { 0 };
int size = 0;
int aa = 0;
int offset = 0;
int bb = 0;
int size1 = 0;
int size4 = 0;


void LoadTexture(const char* FileName)
{
	FILE* fp;

	fopen_s(&fp, FileName, "rb");

	char R;
	char G;
	char B;

	fread(type, sizeof(char), 2, fp);

	fread(&size, 4, 1, fp);

	fread(&aa, 2, 1, fp);

	fread(&aa, 2, 1, fp);

	fread(&offset, 4, 1, fp);

	fseek(fp, 0, SEEK_SET);

	BITMAPFILEHEADER bf;
	fread_s(&bf, sizeof(BITMAPFILEHEADER), sizeof(BITMAPFILEHEADER), 1, fp);

	BITMAPINFOHEADER bi;
	fread_s(&bi, sizeof(BITMAPINFOHEADER), sizeof(BITMAPINFOHEADER), 1, fp);

	fseek(fp, 0, SEEK_SET);

	fseek(fp, offset, SEEK_CUR);

	size1 = (size - offset) / 3;

	texrgb = new TEXRGB[size1];

	for (int i = 0; i<size1; i++)
	{
		fread(&B, 1, 1, fp);
		fread(&G, 1, 1, fp);
		fread(&R, 1, 1, fp);

		texrgb[i].R = R;
		texrgb[i].G = G;
		texrgb[i].B = B;

		texrgb[i].A = 255;
	}

	for (int y = 0; y < bi.biHeight / 2; ++y) {
		for (int x = 0; x < bi.biWidth; ++x) {

			TEXRGB temp = { 0, 0, 0, 0 };
			temp = texrgb[bi.biWidth * y + x];
			texrgb[bi.biWidth * y + x] = texrgb[bi.biWidth * (bi.biHeight - 1 - y) + x];
			texrgb[bi.biWidth * (bi.biHeight - 1 - y) + x] = temp;

		}

	}

}


void Tex_Init(const char* filename) {

	LoadTexture(filename);

	glGenTextures(
		1,
		textures
		);

	glBindTexture(
		GL_TEXTURE_2D,
		textures[0]
		);

	glTexImage2D(
		GL_TEXTURE_2D,      // GLenum target
		0,                  // GLint level
		GL_RGB,             // GLint internalformat
		256, 256,               // GLsizei width, height
		0,                  // GLint border
		GL_RGBA,             // GLenum format
		GL_UNSIGNED_BYTE,   // GLenum type
		texrgb);            // const GLvoid *pixels


	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		GL_NEAREST
		);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		GL_NEAREST
		);

	delete texrgb;



}



struct VECTOR3{
	float x, y, z;
};

struct VECTOR4{
	float x, y, z, w;
};

struct VERTEX{
	float x, y, z;
};

struct NORMAL{
	float x, y, z;
};

struct TEXCOORD{
	float u, v;
};

struct MATERIAL{

	VECTOR4 diffuse;
	float shininess;
	VECTOR3 specular;
	VECTOR3 ambient;

	std::string FileName;
};

std::vector<VERTEX> vertexArray;
std::vector<NORMAL> normalArray;
std::vector<TEXCOORD> texcoordArray;
std::vector<MATERIAL> materialArray;
std::vector<int> indexArray;


void roadModel(){

	FILE *fp = NULL;

	//fopen_s(&fp, "Source.cpp", "r");
	fopen_s(&fp, "bigship1.xt", "r");
	assert(fp != NULL);

	char buf[256];
	

	fseek(fp, SEEK_SET, 0);

	while (!feof(fp)){

		fscanf_s(fp, "%s", buf, sizeof(buf));

		if (strcmp(buf, "template") == 0){
			while (strcmp(buf, "}") != 0){
				fscanf_s(fp, "%s", buf, sizeof(buf));
			}
		}


		if (strcmp(buf, "Mesh") == 0){
		//	printf("%s\n", buf);

			fscanf_s(fp, "%s", buf, sizeof(buf));
		//	printf("%s\n", buf);

			unsigned int nVertics;
			fscanf_s(fp, "%u", &nVertics, sizeof(nVertics));
		//	printf("%u\n", nVertics);

			fscanf_s(fp, "%s", buf, sizeof(buf));
		//	printf("%s\n", buf);

			

			for (int count = 0; count < nVertics; ++count){
				float vertices[3];
				
				for (int i = 0; i < 3; ++i){
					fscanf_s(fp, "%f", &vertices[i], sizeof(vertices[i]));
		//			printf("%f\n", vertices[i]);
					fscanf_s(fp, "%*1s");
				};
				fscanf_s(fp, "%*1s");

				VERTEX vtx;
				vtx.x = vertices[0];
				vtx.y = vertices[1];
				vtx.z = vertices[2];

	//			printf("\n");
				vertexArray.push_back(vtx);
			}

			//
			unsigned int nIndicies;
			fscanf_s(fp, "%u", &nIndicies, sizeof(nIndicies));
			//printf("%u\n", nIndicies);
			fscanf_s(fp, "%1s", buf, sizeof(buf));

			for (int count = 0; count < nIndicies; ++count){
				int indices;

				fscanf_s(fp, "%2s", buf, sizeof(buf));
				//printf("%s\n", buf);

				for (int i = 0; i < 3; ++i){

					fscanf_s(fp, "%d", &indices, sizeof(indices));
					//printf("%d\n", indices);
					fscanf_s(fp, "%1s", buf, sizeof(buf));

					indexArray.push_back(indices);
				};
				fscanf_s(fp, "%1s", buf, sizeof(buf));

				//printf("\n");	
			}
		}
		else if (strcmp(buf, "MeshNormals") == 0){
			fscanf_s(fp, "%s", buf, sizeof(buf));

			unsigned int nNormals;
			fscanf_s(fp, "%u", &nNormals, sizeof(nNormals));
			//printf("%u\n", nNormals);
			fscanf_s(fp, "%*1s");

		for (int count = 0; count < nNormals; ++count){
			float normals[3];

			for (int i = 0; i < 3; ++i){
				fscanf_s(fp, "%f", &normals[i], sizeof(normals[i]));
		//			printf("%f\n", vertices[i]);
					fscanf_s(fp, "%*1s");
				};
				fscanf_s(fp, "%*1s");

				NORMAL nor;
				nor.x = normals[0];
				nor.y = normals[1];
				nor.z = normals[2];

	//			printf("\n");
				normalArray.push_back(nor);

			}
		}
		else if (strcmp(buf, "MeshTextureCoords") == 0){
			fscanf_s(fp, "%s", buf, sizeof(buf));

			unsigned int nTexCoords;
			fscanf_s(fp, "%u", &nTexCoords, sizeof(nTexCoords));
			//printf("%u\n", nTexCoords);
			fscanf_s(fp, "%*1s");

			for (int count = 0; count < nTexCoords; ++count){
				float texcoords[2];

				for (int i = 0; i < 2; ++i){
					fscanf_s(fp, "%f", &texcoords[i], sizeof(texcoords[i]));
					//printf("%f\n", texcoords[i]);
					fscanf_s(fp, "%*1s");
				};
				fscanf_s(fp, "%*1s");

				TEXCOORD tex;
				tex.u = texcoords[0];
				tex.v = texcoords[1];
				
				texcoordArray.push_back(tex);
			}


		}
		else if (strcmp(buf, "Material") == 0){
			fscanf_s(fp, "%s", buf, sizeof(buf));
			float diffuse[4];

			for (int i = 0; i < 4; ++i){
				fscanf_s(fp, "%f", &diffuse[i], sizeof(diffuse[i]));
				fscanf_s(fp, "%*1s");
			};
			fscanf_s(fp, "%*1s");

			MATERIAL material;
			material.diffuse.x = diffuse[0];
			material.diffuse.y = diffuse[1];
			material.diffuse.z = diffuse[2];
			material.diffuse.w = diffuse[3];

			//shinness
			float shinness;
			fscanf_s(fp, "%f", &shinness, sizeof(shinness));
			fscanf_s(fp, "%*1s");

			material.shininess = shinness;

			//specular
			float specular[3];
			for (int i = 0; i < 3; ++i){
				fscanf_s(fp, "%f", &specular[i], sizeof(specular[i]));
				fscanf_s(fp, "%*1s");
			};
			fscanf_s(fp, "%*1s");

			material.specular.x = specular[0];
			material.specular.y = specular[1];
			material.specular.z = specular[2];

			//ambient
			float ambient[3];
			for (int i = 0; i < 3; ++i){
				fscanf_s(fp, "%f", &ambient[i], sizeof(ambient[i]));
				fscanf_s(fp, "%*1s");
			};
			fscanf_s(fp, "%*1s");

			material.ambient.x = ambient[0];
			material.ambient.y = ambient[1];
			material.ambient.z = ambient[2];

			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "TextureFilename") == 0){
				fscanf_s(fp, "%*1s");

				fscanf_s(fp, "%s", buf, sizeof(buf));
				material.FileName = buf;
				std::string str = material.FileName;
				material.FileName = str.substr(str.find_first_of('\"', 0) + 1, str.find_last_of('\"', 255) - 1);

				//material.FileName[5] = '\0';

				printf("filename : %s\n", material.FileName.c_str());

				//fscanf_s(fp, "%*1s");

			}
			fscanf_s(fp, "%*1s");
			fscanf_s(fp, "%*1s");

			materialArray.push_back(material);
		}


	}

	//LoadTexture(materialArray[0].FileName.c_str());
	Tex_Init(materialArray[0].FileName.c_str());

	fclose(fp);


}


void display(void){

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
		90,	
		1,	
		0.1,
		100);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
		0, 0, 15,  
		0, 0, 0,  
		0, 1, 0); 

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);


	static float a = 0;
	a += 0.1;
	glPushMatrix();
	glRotatef(a, 0, 1, 0);
	glColor3f(1, 1, 0);

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const GLfloat *)&materialArray[0].ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const GLfloat *)&materialArray[0].diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const GLfloat *)&materialArray[0].specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materialArray[0].shininess);

	glBindTexture(
		GL_TEXTURE_2D,
		textures[0]
		);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	
		glVertexPointer(
			3,
			GL_FLOAT,
			0,
			vertexArray.data());

		glNormalPointer(
			GL_FLOAT,
			0,
			normalArray.data());

		glTexCoordPointer(
			2,
			GL_FLOAT,
			0,
			texcoordArray.data());

		glDrawElements(
			GL_TRIANGLES,
			indexArray.size(),
			GL_UNSIGNED_INT,
			indexArray.data());

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();

	glFlush();
}

void Init(){

	roadModel();



}



void timer(int value){

	glutTimerFunc(
		1000 / 60,	
		timer,
		0	
		);
	glutPostRedisplay();
}



int main(int argc, char *argv[]){

	glutInit(
		&argc,
		argv
		);

	glutInitWindowSize(800, 800);
	glutCreateWindow("test");


	glutDisplayFunc(display);

	glutTimerFunc(
		0,	
		timer,	
		0	
		);

	Init();

	glutMainLoop();

}

