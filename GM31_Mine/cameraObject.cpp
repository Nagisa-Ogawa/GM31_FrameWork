#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "camera.h"
#include "cameraObject.h"
#include "player.h"


void CameraObject::Init()
{
	m_transform->m_localPosition = D3DXVECTOR3(0.0f, 5.0f, -10.0f);
	m_transform->m_localRotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	AddComponent<Camera>()->Init();
}


