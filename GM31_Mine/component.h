#pragma once
#include <memory>

#include "cereal/cereal.hpp"
#include "cereal/archives/json.hpp"
#include "cereal/types/memory.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/polymorphic.hpp"

class GameObject;

class Component
{
protected:
	GameObject* m_gameObject;
public:
	virtual void Init() {}
	virtual void Load() {}	// ì«Ç›çûÇ›éûÇÃèàóù
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}


	virtual void DispInspector(){}

	void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	GameObject* GetGameObject() { return m_gameObject; }

	template <class Archive>
	void save(Archive& archive) const
	{
	}

	template <class Archive>
	void load(Archive& archive)
	{
	}

};
