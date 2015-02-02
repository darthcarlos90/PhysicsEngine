#include "HeightMap.h"

HeightMap::HeightMap(std::string name){
	std::ifstream file(name.c_str(), ios::binary);
	
	if(!file)
		return;

	numVertices = RAW_WIDTH * RAW_HEIGHT; //The total amount of results given by TerraGen
	numIndices = (RAW_WIDTH-1) * (RAW_HEIGHT-1)*6; //The number of posible combinations times 6, because
													//every square requieres 2 triangles, 6 vertices
	vertices = new Vector3[numVertices];
	textureCoords = new Vector2[numVertices];
	indices = new GLuint[numIndices];

	unsigned char*data = new unsigned char [numVertices];
	file.read((char*)data, numVertices*sizeof(unsigned char));
	file.close();

	for(int x = 0; x < RAW_WIDTH; ++x){
		for (int z = 0; z < RAW_HEIGHT; ++z){
			int offset = (x * RAW_WIDTH) + z;

			vertices[offset] = Vector3(x * HEIGHTMAP_X, data[offset] * HEIGHTMAP_Y, z * HEIGHTMAP_Z);
			vertex_vector.push_back(vertices[offset]);

			textureCoords[offset] = Vector2(x * HEIGHTMAP_TEX_X, z * HEIGHTMAP_TEX_Z);
		}
	}

	delete data;

	numIndices = 0;

	for(int x = 0; x < RAW_WIDTH-1; ++x){
		for( int z = 0; z < RAW_HEIGHT - 1; ++z){
			int a = (x * (RAW_WIDTH)) + z;
			int b = ((x + 1) * (RAW_WIDTH)) + z;
			int c = ((x + 1) * (RAW_WIDTH)) + (z + 1);
			int d = (x * (RAW_WIDTH)) + (z + 1);

			indices[numIndices++] = c;
			indices[numIndices++] = b;
			indices[numIndices++] = a;

			indices[numIndices++] = a;
			indices[numIndices++] = d;
			indices[numIndices++] = c;
		}
	}

	//type = GL_LINES;
	GenerateNormals();//Added on tutorial 11

	GenerateTangents(); //Added on tutorial 12

	BufferData();


}