/*
	Created by Akimatsu_404
	FileName :  XLOADER.cpp
*/

#include "XLOADER.h"

X_MODEL::X_MODEL(const char* FileName) {
	Load(FileName);
}

X_MODEL::~X_MODEL() {

	/*vertexArray.clear();
	normalArray.clear();
	texcoordArray.clear();
	materialArray.clear();
	indexArray.clear();
	textureArray.clear();
	texID.clear();*/

	m_nodeArray.clear();

}

void X_MODEL::Load(const char* FileName) {

	FILE *fp = NULL;
	fopen_s(&fp, FileName, "r");
	assert(fp != NULL);

	char buf[256];


	fseek(fp, SEEK_SET, 0);

	while (!feof(fp)) {

		fscanf_s(fp, "%s", buf, sizeof(buf));

		if (strcmp(buf, "template") == 0) {
			while (strcmp(buf, "}") != 0) {
				fscanf_s(fp, "%s", buf, sizeof(buf));
			}
		}

		if (strcmp(buf, "Frame") == 0) {
			NODE t_node;
			m_nodeArray.push_back(t_node);
			fscanf_s(fp, "%s", buf, sizeof(buf));
			m_nodeArray[m_nodeArray.size() - 1].nodeName = buf;
			fscanf_s(fp, "%*1s");

			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "FrameTransformMatrix") == 0) {
				fscanf_s(fp, "%s", buf, sizeof(buf));
				if (strcmp(buf, "{") == -1) {
					fscanf_s(fp, "%*1s");
				}
				float mat[16];
				for (int i = 0; i < 16; ++i) {
					fscanf_s(fp, "%f", &mat[i], sizeof(mat[i]));
					fscanf_s(fp, "%*1s");
				}
				fscanf_s(fp, "%*1s");
				
				for (int i = 0; i < 4; ++i) {
					for (int k = 0; k < 4; ++k) {
						m_nodeArray[m_nodeArray.size() - 1].nodeMat[i][k] = mat[k + (i * 4)];
					}
				}
			}

		}

		if (strcmp(buf, "Mesh") == 0) {
			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "{") == -1) {
				fscanf_s(fp, "%*1s");
			}
			unsigned int nVertics;
			fscanf_s(fp, "%u", &nVertics, sizeof(nVertics));
			fscanf_s(fp, "%s", buf, sizeof(buf));


			for (int count = 0; count < nVertics; ++count) {
				float vertices[3];

				for (int i = 0; i < 3; ++i) {
					fscanf_s(fp, "%f", &vertices[i], sizeof(vertices[i]));
					fscanf_s(fp, "%*1s");
				};
				fscanf_s(fp, "%*1s");

				VERTEX vtx;
				vtx.x = vertices[0];
				vtx.y = vertices[1];
				vtx.z = vertices[2];

				m_nodeArray[m_nodeArray.size() - 1].vertexArray.push_back(vtx);
			}

			//
			unsigned int nIndicies;
			fscanf_s(fp, "%u", &nIndicies, sizeof(nIndicies));
			fscanf_s(fp, "%1s", buf, sizeof(buf));

			for (int count = 0; count < nIndicies; ++count) {
				int indices;

				fscanf_s(fp, "%2s", buf, sizeof(buf));

				for (int i = 0; i < 3; ++i) {

					fscanf_s(fp, "%d", &indices, sizeof(indices));
					fscanf_s(fp, "%1s", buf, sizeof(buf));

					m_nodeArray[m_nodeArray.size() - 1].indexArray.push_back(indices);
				};
				fscanf_s(fp, "%1s", buf, sizeof(buf));

			}
		}
		else if (strcmp(buf, "MeshNormals") == 0) {
			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "{") == -1) {
				fscanf_s(fp, "%*1s");
			}

			unsigned int nNormals;
			fscanf_s(fp, "%u", &nNormals, sizeof(nNormals));
			fscanf_s(fp, "%*1s");

			for (int count = 0; count < nNormals; ++count) {
				float normals[3];

				for (int i = 0; i < 3; ++i) {
					fscanf_s(fp, "%f", &normals[i], sizeof(normals[i]));
					fscanf_s(fp, "%*1s");
				};
				fscanf_s(fp, "%*1s");

				NORMAL nor;
				nor.x = normals[0];
				nor.y = normals[1];
				nor.z = normals[2];

				m_nodeArray[m_nodeArray.size() - 1].normalArray.push_back(nor);

			}
		}
		else if (strcmp(buf, "MeshTextureCoords") == 0) {
			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "{") == -1) {
				fscanf_s(fp, "%*1s");
			}

			unsigned int nTexCoords;
			fscanf_s(fp, "%u", &nTexCoords, sizeof(nTexCoords));
			fscanf_s(fp, "%*1s");

			for (int count = 0; count < nTexCoords; ++count) {
				float texcoords[2];

				for (int i = 0; i < 2; ++i) {
					fscanf_s(fp, "%f", &texcoords[i], sizeof(texcoords[i]));
					fscanf_s(fp, "%*1s");
				};
				fscanf_s(fp, "%*1s");

				TEXCOORD tex;
				tex.u = texcoords[0];
				tex.v = texcoords[1];

				m_nodeArray[m_nodeArray.size() - 1].texcoordArray.push_back(tex);
			}


		}
		else if (strcmp(buf, "Material") == 0) {
			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "{") == -1) {
				fscanf_s(fp, "%*1s");
			}

			float diffuse[4];

			for (int i = 0; i < 4; ++i) {
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
			for (int i = 0; i < 3; ++i) {
				fscanf_s(fp, "%f", &specular[i], sizeof(specular[i]));
				fscanf_s(fp, "%*1s");
			};
			fscanf_s(fp, "%*1s");

			material.specular.x = specular[0];
			material.specular.y = specular[1];
			material.specular.z = specular[2];

			//ambient
			float ambient[3];
			for (int i = 0; i < 3; ++i) {
				fscanf_s(fp, "%f", &ambient[i], sizeof(ambient[i]));
				fscanf_s(fp, "%*1s");
			};
			fscanf_s(fp, "%*1s");

			material.ambient.x = ambient[0];
			material.ambient.y = ambient[1];
			material.ambient.z = ambient[2];


			fscanf_s(fp, "%s", buf, sizeof(buf));
			if (strcmp(buf, "TextureFilename") == 0) {
				fscanf_s(fp, "%s", buf, sizeof(buf));
				if (strcmp(buf, "{") == -1) {
					fscanf_s(fp, "%*1s");
				}

				fscanf_s(fp, "%s", buf, sizeof(buf));
				//material.FileName = buf;
				//std::string str = material.FileName;
				
				std::string str = buf;
				material.texFileName = str.substr(str.find_first_of('\"', 0) + 1, str.find_last_of('\"', 255) - 1);
				printf("Load Filename : %s\n", material.texFileName.c_str());
				
				TEXTURE tex;
				m_nodeArray[m_nodeArray.size() - 1].textureArray.push_back(tex);
				m_nodeArray[m_nodeArray.size() - 1].textureArray[m_nodeArray[m_nodeArray.size() - 1].textureArray.size() - 1] =  TEXTURE(material.texFileName.c_str());
				
				material.texNo = m_nodeArray[m_nodeArray.size() - 1].textureArray.size();
				GLuint addID;
				m_nodeArray[m_nodeArray.size() - 1].texID.push_back(addID);
				glGenTextures(1, (GLuint*) &m_nodeArray[m_nodeArray.size() - 1].texID[m_nodeArray[m_nodeArray.size() - 1].textureArray.size() - 1]);
				glBindTexture(GL_TEXTURE_2D, m_nodeArray[m_nodeArray.size() - 1].texID[m_nodeArray[m_nodeArray.size() - 1].textureArray.size() - 1]);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

				glEnable(GL_TEXTURE_2D);
				
				glTexImage2D(GL_TEXTURE_2D, 
					0, 
					GL_RGBA, 
					m_nodeArray[m_nodeArray.size() - 1].textureArray[m_nodeArray[m_nodeArray.size() - 1].textureArray.size() - 1].Width, m_nodeArray[m_nodeArray.size() - 1].textureArray[m_nodeArray[m_nodeArray.size() - 1].textureArray.size() - 1].Height,
					0, 
					GL_RGBA, GL_UNSIGNED_BYTE, 
					m_nodeArray[m_nodeArray.size() - 1].textureArray[m_nodeArray[m_nodeArray.size() - 1].textureArray.size() - 1].image);
				
				glDisable(GL_TEXTURE_2D);

			}
			fscanf_s(fp, "%*1s");
			fscanf_s(fp, "%*1s");

			m_nodeArray[m_nodeArray.size() - 1].materialArray.push_back(material);
		}

		//Animation

		//else if (strcmp(buf, "Animation") == 0) {
		//	fscanf_s(fp, "%*1s");
		//	fscanf_s(fp, "%s", buf, sizeof(buf));

		//	if(strcmp(buf, "AnimationKey")){
		//		unsigned int keyType;
		//		fscanf_s(fp, "%u", &keyType, sizeof(keyType));

		//		if(keyType == 0){//Rotate
		//		}
		//		else if(keyType == 1){//Scale
		//		}
		//		else if(keyType == 2){//Translate
		//		}
		//		else if (keyType == 3) {//matrix

		//		}

		//	}

		//}


	}




	fclose(fp);

}

void X_MODEL::Update() {

}

void X_MODEL::Draw() {

	glPushMatrix();
	glEnable(GL_NORMALIZE);

	for (int nodeCount = 0; nodeCount < m_nodeArray.size(); ++nodeCount) {

		glMultMatrixf(&m_nodeArray[nodeCount].nodeMat[0][0]);

		for (int i = 0; i < m_nodeArray[nodeCount].materialArray.size(); ++i) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, (const GLfloat *)&m_nodeArray[nodeCount].materialArray[i].ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, (const GLfloat *)&m_nodeArray[nodeCount].materialArray[i].diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, (const GLfloat *)&m_nodeArray[nodeCount].materialArray[i].specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, m_nodeArray[nodeCount].materialArray[i].shininess);

			if (m_nodeArray[nodeCount].materialArray[i].texNo > 0) {
				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glEnable(GL_TEXTURE_2D);

				glBindTexture(
					GL_TEXTURE_2D,
					m_nodeArray[nodeCount].texID[m_nodeArray[nodeCount].materialArray[i].texNo - 1]
					);
			}
			else {
				glDisable(GL_TEXTURE_2D);
				glDisableClientState(GL_TEXTURE_COORD_ARRAY);
			}
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_NORMAL_ARRAY);


		glVertexPointer(
			3,
			GL_FLOAT,
			0,
			m_nodeArray[nodeCount].vertexArray.data());

		glNormalPointer(
			GL_FLOAT,
			0,
			m_nodeArray[nodeCount].normalArray.data());

		if (m_nodeArray[nodeCount].materialArray[0].texNo > 0) {

			glTexCoordPointer(
				2,
				GL_FLOAT,
				0,
				m_nodeArray[nodeCount].texcoordArray.data());

		}
		glDrawElements(
			GL_TRIANGLES,
			m_nodeArray[nodeCount].indexArray.size(),
			GL_UNSIGNED_INT,
			m_nodeArray[nodeCount].indexArray.data());

	}
	glDisable(GL_NORMALIZE);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);



	glPopMatrix();

}
