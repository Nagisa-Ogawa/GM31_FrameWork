#pragma once


#include "component.h"

#define STAGE_WIDTH (21)
#define STAGE_HEIGHT (21)


//-----------------------------------------------------
// �t�B�[���h�R���|�[�l���g�N���X
//------------------------------------------------------
// �w�肵���I�u�W�F�N�g��Y���W��
// �t�B�[���h�̒��_�ɍ��킹��R���|�[�l���g
//------------------------------------------------------

class MeshField : public Component
{

private:
	ID3D11Buffer*				m_vertexBuffer{};
	ID3D11Buffer*				m_indexBuffer{};
	ID3D11ShaderResourceView*	m_texture{};
	//ID3D11VertexShader*			m_vertexShader{};
	//ID3D11PixelShader*			m_pixelShader{};
	//ID3D11InputLayout*			m_vertexLayout{};

	VERTEX_3D					m_vertex[STAGE_WIDTH][STAGE_HEIGHT]{};

	int m_targetID = -1;
	GameObject* m_target = nullptr;
	std::string m_texturePath;

public:
	void Init(std::string filePath);
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	// Get�n�֐�
	int GetTargetID() { return m_targetID; }
	GameObject* GetTarget() { return m_target; }
	std::string GetTexturePath() { return m_texturePath; }
	// Set�n�֐�
	void SetTargetID(int id) { m_targetID = id; }
	void SetTarget(GameObject* object) { m_target = object; }

	void ChangeTexture(std::string texturePath);	// �e�N�X�`����ύX����֐�

	float GetHeight(D3DXVECTOR3 pos);	// �t�B�[���h�̒��_��񂩂獂�������߂�֐�

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_targetID),
			CEREAL_NVP(m_texturePath)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this),
			m_targetID,
			m_texturePath
		);
	}


};

CEREAL_REGISTER_TYPE(MeshField);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, MeshField);
