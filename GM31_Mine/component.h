#pragma once

class GameObject;

class Component
{
protected:
	GameObject* m_gameObject = nullptr;
public:
	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void Draw() {}

	virtual void DispInspector(){}

	void SetGameObject(GameObject* gameObject) { m_gameObject = gameObject; }
	GameObject* GetGameObject() { return m_gameObject; }
};
