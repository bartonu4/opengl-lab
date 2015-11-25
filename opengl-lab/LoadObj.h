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
struct my_vec2{
	my_vec2(){};
	my_vec2(T _x, T _y) :x(_x), y(_y){};
	T x{ 0 }, y{ 0 };
};
template<typename T>
struct my_vec3{
	my_vec3(){};
	my_vec3(T _x, T _y, T _z) :x(_x), y(_y), z(_z){};
	
	T x{ 0 }, y{ 0 }, z{ 0 };
};
typedef my_vec2<float> vec2f;
typedef my_vec3<float> vec3f;
typedef my_vec3<int> vec3i;
class LoadObj
{
private:
	string filename;
	
public:
	LoadObj();
	LoadObj(string name) :filename(name){};
	bool loadObj();
	~LoadObj(); 
	vector<vec3f> vertices;
	vector < vec2f> uvs;
	vector < vec3f> normales;
};
