#pragma once

#include "SerializableClass.h"
#include "component.h"

//-----------------------------------------------------
// �J�����R���|�[�l���g�N���X
//------------------------------------------------------
class Camera :public Component
{
private:
	// ���g�p
	D3DXVECTOR3 m_position{};
	D3DXVECTOR3 m_rotation{};
	D3DXVECTOR3 m_right{};
	D3DXVECTOR3 m_up{};

	D3DXMATRIX m_cameraMatrix{};	// �J�����̍s��
	D3DXMATRIX m_viewMatrix{};		// �r���[�s��
	D3DXMATRIX m_projectionMatrix{};// �v���W�F�N�V�����s��
public:


	void Init()  override;
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	void DispInspector() override;

	D3DXVECTOR3 GetPosition()	{ return m_position; }
	D3DXVECTOR3 GetRotation()	{ return m_rotation; }
	D3DXVECTOR3 GetUp()			{ return m_up; };

	D3DXMATRIX* GetViewMatrix()	{ return &m_viewMatrix; }
	D3DXMATRIX* GetProjectionMatrix() {return &m_projectionMatrix;}

	void SetPosition(D3DXVECTOR3 position)	{ m_position = position; }
	void SetRotation(D3DXVECTOR3 rotation)	{ m_rotation = rotation; }
	void SetUp(D3DXVECTOR3 up)				{ m_up = up; }


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
		Vector3 position, rotation;
		archive(
			cereal::base_class<Component>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(Camera);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Camera);

