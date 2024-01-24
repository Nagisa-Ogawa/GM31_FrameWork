#pragma once
#include <string>
#include <luabind/object.hpp>
#include "component.h"


class Script :public Component
{
private:
	luabind::object m_object;
	std::string m_fileName;
public:
	void Init(std::string fileName);
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	void Start();	// �V�[���̎��s���ɍŏ��Ɉ�x�����Ă΂��֐�
	void CompileLua();	// Lua�t�@�C�����R���p�C������֐�

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

CEREAL_REGISTER_TYPE(Script);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Script);

