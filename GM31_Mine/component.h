#pragma once

class GameObject;

class Component
{
protected:
	GameObject* m_GameObject = nullptr;
public:
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	void SetGameObject(GameObject* gameObject) { m_GameObject = gameObject; }
};
