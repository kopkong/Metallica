#include "view_control.h"
#include <gtc/matrix_transform.hpp>

ViewController::ViewController()
{
	m_Model = glm::mat4(1.0);
	m_Project = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	m_ViewPosition = glm::vec3(1.0f, -2.0f, -5.0f);
	m_LookAt = glm::vec3(0.0f,0.0f,0.0f);
	m_LookUp = glm::vec3(0.0f, 1.0f, 0.0f);

}

glm::vec3 ViewController::GetViewerPosition()
{
	return m_ViewPosition;
}

glm::mat4 ViewController::GetMVPMatrix()
{
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								m_ViewPosition,
								m_LookAt,
								m_LookUp  // Head is up 
						   );

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = m_Project * View * m_Model; // Remember, matrix multiplication is the other way around


	return MVP;

}

glm::mat4 ViewController::GetModelMatrix()
{
	return m_Model;
}

void ViewController::CameraRotate()
{

}

void ViewController::ModelRotate()
{

}