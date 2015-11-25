#pragma once
#include<string>
#include <vector>
#include<iostream>
#include <fstream>
#include <sstream>
using std::vector;
using std::ifstream;
using std::string;
using std::stringstream;

template<typename T>
struct vec2{
	T x{ 0 }, y{ 0 };
};
template<typename T>
struct vec3{
	T x{ 0 }, y{ 0 }, z{ 0 };
};

class LoadObj
{
private:
	string filename;
	vector<vec3<float>> vertices;
	vector < vec2<float>> uvs;
	vector < vec3<float>> normales;
public:
	LoadObj();
	LoadObj(string name) :filename(name){};
	bool loadObj();
	~LoadObj();
};
