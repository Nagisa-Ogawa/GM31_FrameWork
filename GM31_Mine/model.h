#pragma once

#include <string>
#include "component.h"

// マテリアル構造体
struct MODEL_MATERIAL
{
	char						Name[256];
	MATERIAL					Material;
	char						TextureName[256];
	ID3D11ShaderResourceView*	Texture;

};


// 描画サブセット構造体
struct SUBSET
{
	unsigned int	StartIndex;
	unsigned int	IndexNum;
	MODEL_MATERIAL	Material;
};


// モデル構造体
struct MODEL
{
	VERTEX_3D		*VertexArray;
	unsigned int	VertexNum;

	unsigned int	*IndexArray;
	unsigned int	IndexNum;

	SUBSET			*SubsetArray;
	unsigned int	SubsetNum;
};



class Model : public Component
{
private:

	ID3D11Buffer*	m_VertexBuffer;
	ID3D11Buffer*	m_IndexBuffer;

	std::string m_fileName;

	SUBSET*	m_SubsetArray;
	unsigned int	m_SubsetNum;

	void LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum );

public:

	void Init(const char* FileName);
	void Init(MODEL* pModel);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	void LoadObj( const char *FileName, MODEL *Model );
	std::string GetFileName() { return m_fileName; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			CEREAL_NVP(m_fileName)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			m_fileName
		);
	}

};

CEREAL_REGISTER_TYPE(Model);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Model);
