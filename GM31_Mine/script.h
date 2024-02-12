#pragma once
#include <string>
#include <luabind/object.hpp>
#include "component.h"


class Script :public Component
{
private:
	luabind::object m_object;
	std::string m_fileName;
	time_t m_updateTime;
	bool m_isCompiled = false;
public:
	void Init(std::string fileName);
	void Load() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
	void DispInspector() override;

	void Start();	// �V�[���̎��s���ɍŏ��Ɉ�x�����Ă΂��֐�
	void CompileLua();	// Lua�t�@�C�����R���p�C������֐�
	time_t GetUpdateTime();	// Lua�t�@�C���̍X�V���Ԃ��擾����
	bool CheckUpdate();	// Lua�t�@�C�����X�V����Ă��邩���`�F�b�N

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

CEREAL_REGISTER_TYPE(Script);
CEREAL_REGISTER_POLYMORPHIC_RELATION(Component, Script);

