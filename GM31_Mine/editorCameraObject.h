#pragma once

#include "gameObject.h"


class EditorCameraObject :public GameObject
{
private:
	bool	m_isRButton = false;	// �E�N���b�N�������Ă��邩�̃t���O
	POINT	m_startMousePos{};		// �E�N���b�N���������Ƃ��̃}�E�X���W
public:
	void Init() override;
	void Update() override;

	void RotateCamera(D3DXVECTOR2 delta);	// �J�������J�������g�𒆐S�Ƃ��ĉ�]������֐�

};
