#pragma once

#include <string>
#include "component.h"


//-----------------------------------------------------
// シェーダーコンポーネントクラス
//------------------------------------------------------
class Shader : public Component
{
private:
	std::string m_fileNameVS;
	std::string m_fileNamePS;
	ID3D11VertexShader* m_VertexShader = nullptr;
	ID3D11PixelShader* m_PixelShader = nullptr;
	ID3D11InputLayout* m_VertexLayout = nullptr;

public:
	void Init(const char* VSFileName,const char* PSFileName);
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	void ChangeVertexShaer(std::string fileName);
	void ChangePixelShaer(std::string fileName);

	std::string GetVSFile() { return m_fileNameVS; }
	std::string GetPSFile() { return m_fileNamePS; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<Component>(this),
			CEREAL_NVP(m_fileNameVS),
			CEREAL_NVP(m_fileNamePS)
			);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<Component>(this),
			m_fileNameVS,
			m_fileNamePS
			);
	}

};

CEREAL_REGISTER_TYPE(Shader);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Shader);

