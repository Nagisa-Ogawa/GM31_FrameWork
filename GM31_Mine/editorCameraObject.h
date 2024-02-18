#pragma once

#include "gameObject.h"


//-----------------------------------------------------
// �G�f�B�^�p�J�����I�u�W�F�N�g
//------------------------------------------------------
// �G�f�B�^���f���p�̃J�����I�u�W�F�N�g�ł�
//------------------------------------------------------
class EditorCameraObject :public GameObject
{
private:
	bool	m_isRButton = false;	// �E�N���b�N�������Ă��邩�̃t���O
	POINT	m_startMousePos{};		// �E�N���b�N���������Ƃ��̃}�E�X���W
public:
	void Init() override;
	void Update() override;

	void RotateCamera(D3DXVECTOR2 delta);	// �J�������J�������g�𒆐S�Ƃ��ĉ�]������֐�

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(EditorCameraObject);

