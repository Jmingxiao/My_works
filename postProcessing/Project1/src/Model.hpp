#pragma once
#include<iostream>
#include<string>
#include<vector>

#include"glm/gtc/matrix_transform.hpp"
#include"glm/glm.hpp"
#include "Object.h"

using std::vector;

class model
{
public:
	model();
	~model();

	void analysisObject();
private:

	vector<glm::vec3> position;
	vector <glm::vec2> textcoord;
	vector <glm::vec3> normal;
	vector<Vertex> verticies;

};

