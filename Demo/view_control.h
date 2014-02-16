#ifndef VIEW_H
#define	VIEW_H

#include "stdafx.h"

class ViewController
{
public:

	ViewController();
	void ModelRotate();
	void CameraRotate();
	glm::mat4 GetModelMatrix();
	glm::mat4 GetMVPMatrix();
	glm::vec3 GetViewerPosition(); //Camera position

private:
	glm::mat4 m_Model;
	glm::mat4 m_Project;
	glm::vec3 m_ViewPosition;
	glm::vec3 m_LookAt;
	glm::vec3 m_LookUp;
};

#endif