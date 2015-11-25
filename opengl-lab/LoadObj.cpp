#include "LoadObj.h"


LoadObj::LoadObj()
{
}


LoadObj::~LoadObj()
{
}
bool LoadObj::loadObj()
{
	vector<vec3f> temp_vert;
	vector<vec2f> temp_uvs;
	vector<vec3f> temp_norm;
	ifstream obj(filename, ifstream::in);
	while (!obj.eof())
	{
		char line[128];
		stringstream sstr;
		obj.getline(line,128);
		vec3f vertice;
		vec2f uv;
		vec3f normale;
		
		vec3i vertices_ind, uvs_ind, normales_ind;
		if (line[0]=='v'&&line[1]==' ')
		{
			
			int result = sscanf(line, "v %f %f %f\n",&vertice.x,&vertice.y,&vertice.z);
			temp_vert.push_back(vertice);
		} 
		if (line[0]=='v'&&line[1] == 't' )
		{

			int result = sscanf(line, "vt %f %f\n", &uv.x, &uv.y);
			temp_uvs.push_back(uv);
		}
		if (line[0]=='v' && line[1] == 'n' )
		{

			int result = sscanf(line, "vn %f %f %f\n", &normale.x, &normale.y, &normale.z);
			temp_norm.push_back(normale);
		}
		if (line[0]==102)
		{

			int result = sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d/n", 
				&vertices_ind.x, &uvs_ind.x, &normales_ind.x, 
				&vertices_ind.y, &uvs_ind.y, &normales_ind.y,
				&vertices_ind.z, &uvs_ind.z, &normales_ind.z);
			
			
			vertices.push_back(vec3f(temp_vert[vertices_ind.x - 1].x, temp_vert[vertices_ind.y - 1].y, temp_vert[vertices_ind.z - 1].z));		
			normales.push_back(vec3f(temp_norm[normales_ind.x - 1].x, temp_vert[normales_ind.y - 1].y, temp_vert[normales_ind.z - 1].z));
			uvs.push_back(vec2f(temp_uvs[uvs_ind.x - 1].x, temp_uvs[uvs_ind.y - 1].y));
		}
		
	}
	return true;
}