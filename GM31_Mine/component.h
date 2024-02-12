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
	GameObject* m_gameObject = nullptr;
	int m_ID = -1;
	bool m_destroy = false;
public:
	virtual void Init() {}
	virtual void Load() {}	// ì«Ç›çûÇ›éûÇÃèàóù
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}


	virtual void DispInspector(){}

	GameObject* GetGameObject() { return m_gameObject; }
	int	GetID() { return m_ID; }

	void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	void SetID(int ID) { m_ID = ID; }
	void SetDestroy() { m_destroy = true; }

	bool Destroy()
	{
		if (m_destroy)
		{
			Uninit();
			return true;
		}
		else
		{
			return false;
		}
	}

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(CEREAL_NVP(m_ID));
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(m_ID);
	}

};
