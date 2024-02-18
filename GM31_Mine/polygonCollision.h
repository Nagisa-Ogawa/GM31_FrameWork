#pragma once
#include "SerializableClass.h"
#include "component.h"

struct POLYGON_POSITION
{
	D3DXVECTOR3 position[3];
};


//-----------------------------------------------------
// �|���S���R���W�����R���|�[�l���g�N���X
//------------------------------------------------------
// �}�E�X�ŃI�u�W�F�N�g���w�����������Ɏg�p���Ă��܂�
// ���ۂ̓����蔻��͖������ł�
//------------------------------------------------------
class PolygonCollision :public Component
{
private:
	POLYGON_POSITION* m_polyArray = nullptr;	// �|���S���̊e���_���W�����z��
	unsigned int m_arrayCount = 0;				// �z��̗v�f��
public:
	void Init() override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void DispInspector() override;

	POLYGON_POSITION* GetPolygonArray() { return m_polyArray; }		// �|���S���̒��_�z����擾����֐�
	unsigned int GetArrayCount() { return m_arrayCount; }

	void SetPolygonArray(POLYGON_POSITION* polyArray) { m_polyArray = polyArray; }		// �|���S���̒��_�z����Z�b�g����֐�
	void SetArrayCount(unsigned int arrayCount) { m_arrayCount = arrayCount; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(PolygonCollision);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, PolygonCollision);
