#pragma once

#include "gameObject.h"


//-----------------------------------------------------
// ���R���W�����\���p�I�u�W�F�N�g
//------------------------------------------------------
// �G�f�B�^��ŃR���W������\������p�̃I�u�W�F�N�g�ł�
//------------------------------------------------------
class SphereCollisionFrame : public GameObject
{

private:
	class Transform* m_collTransform{};
	D3DXVECTOR3 m_Offset{};
public:
	void Init(float radius, D3DXVECTOR3 offset);
	void Update() override;

	void SetCollTransform(Transform* transform) { m_collTransform = transform; }
};