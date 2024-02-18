#pragma once

#include "gameObject.h"

//-----------------------------------------------------
// �{�b�N�X�R���W�����\���p�I�u�W�F�N�g
//------------------------------------------------------
// �G�f�B�^��ŃR���W������\������p�̃I�u�W�F�N�g�ł�
//------------------------------------------------------
class BoxCollisionFrame : public GameObject
{

private:
	class BoxCollision* m_collision = nullptr;
public:
	void Init(BoxCollision* collision);
	void Draw() override;

	void SetCollision(BoxCollision* coll) { m_collision = coll; }

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

CEREAL_REGISTER_TYPE(BoxCollisionFrame);