#pragma once

class Scene;	// �O���錾
class Manager
{
private:
	static Scene* m_Scene;
public:
	static void Init();
	static void Uninit();
	static void Update();
	static void Draw();


};