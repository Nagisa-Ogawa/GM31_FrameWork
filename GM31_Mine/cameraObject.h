#pragma once

#include "gameObject.h"


class CameraObject:public GameObject
{
private:
	bool	m_isRButton = false;	// �E�N���b�N�������Ă��邩�̃t���O
	POINT	m_startMousePos{};		// �E�N���b�N���������Ƃ��̃}�E�X���W
public:
	void Init() override;
	void Update() override;
	
	template <class Archive>
	void save(Archive& archive)
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

CEREAL_REGISTER_TYPE(CameraObject);
