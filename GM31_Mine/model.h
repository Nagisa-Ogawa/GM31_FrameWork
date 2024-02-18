#pragma once

#include <string>
#include "component.h"

// �}�e���A���\����
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// �`��T�u�Z�b�g�\����
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// ���f���\����
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};


//-----------------------------------------------------
// ���f���R���|�[�l���g�N���X
//------------------------------------------------------
// .obj�t�@�C���̂ݓǂݍ��݉\�i�O�p�|���S���̂݁j
//------------------------------------------------------

class Model : public Component
{
private:

	ID3D11Buffer*	m_vertexBuffer;
	ID3D11Buffer*	m_indexBuffer;

	std::string m_fileName;

	SUBSET*	m_subsetArray;
	unsigned int	m_subsetNum;

	class PolygonCollision* m_polyColl = nullptr;	// ���̃��f���p�̓����蔻��i�}�E�X�I�����m�p�j

	void LoadMaterial( const char *fileName, MODEL_MATERIAL **materialArray, unsigned int *materialNum );

public:

	void Init(const char* fileName,bool hasColl);
	void Init(MODEL* pModel);
	void Load() override;
	void Uninit() override;
	void Draw() override;
	void DispInspector() override;

	void ChangeModel(std::string fileName);
	void LoadObj( const char *fileName, MODEL *Model );
	std::string GetFileName() { return m_fileName; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_fileName)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this),
			m_fileName
		);
	}

};

CEREAL_REGISTER_TYPE(Model);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Model);
