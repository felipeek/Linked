#pragma once
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

class Transform
{
public:
	Transform();
	Transform(glm::vec3 position, glm::vec3 size);
	Transform(glm::vec3 position);
	Transform(glm::vec3 position, float angle, glm::vec3 axis, glm::vec3 scale);

	~Transform();
	glm::mat4 model;

	// Methods
	void translate(float x, float y, float z);
	void rotate(float angle, glm::vec3 axe);
	void scale(float x, float y, float z);

	void incTranslate(float x, float y, float z);
	void incRotateY(float amt);
	void incScale(float x, float y, float z);

	glm::vec3 getPosition();
	void setPosition(glm::vec3& pos);
	glm::vec3 getScale();
	float getAngleY();

private:
	glm::mat4 translationMatrix;
	glm::mat4 scaleMatrix;
	glm::mat4 rotationMatrix;

	glm::vec3 mPosition;
	glm::vec3 mScale;
	float angleY;

	void updateModel();
};

