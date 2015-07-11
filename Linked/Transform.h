#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform
{
public:
	Transform();
	Transform(float x, float y, float z);
	~Transform();
	glm::mat4 model;

	// Methods
	void translate(float x, float y, float z);
	void rotate(float angle, glm::vec3 axe);
	void scale(float x, float y, float z);

	void incTranslate(float x, float y, float z);
	void incRotateY(float amt);
	void incScale(float x, float y, float z);

private:
	glm::mat4 translationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;

	glm::vec3 mPosition;
	glm::vec3 mScale;
	float angleY;

	void updateModel();
};

