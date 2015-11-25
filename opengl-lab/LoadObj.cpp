#include "LoadObj.h"


LoadObj::LoadObj()
{
}


LoadObj::~LoadObj()
{
}
bool LoadObj::loadObj()
{
	vector<vec3<float>> temp_vert;
	vector<vec2<float>> tem_uvs;
	vector<vec3<float>> temp_norm;
	ifstream obj(filename, ifstream::in);
	while (!obj.eof())
	{
		char line[128];
		stringstream sstr;
		obj.getline(line,128);
		vec3<float> vertice;
		vec2<float> uv;
		vec3<float>normale;
		
		if (1==strcmp(line, "v"))
		{
			
			int result = sscanf(line, "v %f %f %f\n",&vertice.x,&vertice.y,&vertice.z);
			temp_vert.push_back(vertice);
		}
		
	}
	return true;
}