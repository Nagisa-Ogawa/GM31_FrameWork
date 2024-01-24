#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"

#include "component.h"

#pragma comment (lib, "assimp-vc143-mt.lib")


//変形後頂点構造体
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiVector3D Normal;
	int				BoneNum;
	std::string		BoneName[4];//本来はボーンインデックスで管理するべき
	float			BoneWeight[4];
};

//ボーン構造体
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

class AnimationModel : public Component
{
private:
	std::string m_fileName;

	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer**	m_VertexBuffer;
	ID3D11Buffer**	m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ
	std::unordered_map<std::string, BONE> m_Bone;//ボーンデータ（名前で参照）

	void CreateBone(aiNode* Node);
	void UpdateBoneMatrix(aiNode* Node, aiMatrix4x4 Matrix);

public:
	void Load( const char *FileName );
	void LoadAnimation( const char *FileName, const char *Name );
	void Unload();

	void Init(const char* FileName);
	void Uninit() override;
	void Update(const char *AnimationName1, int Frame1, const char* AnimationName2, int Frame2,float BlendRate);
	void Draw() override;

	std::string GetFileName() { return m_fileName; }
	void DispInspector() override;

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
			CEREAL_NVP(m_fileName)
		);
	}

};

// cerealにクラスを登録
CEREAL_REGISTER_TYPE(AnimationModel);
// cerealに継承していることを登録
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, AnimationModel);