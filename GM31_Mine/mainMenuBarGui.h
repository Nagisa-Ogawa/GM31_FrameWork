#pragma once
#include "MyImGui.h"

class GameObject;

class MainMenuBarGui : public MyImGui
{
private:
	ID3D11ShaderResourceView* m_executeBtnTexture = nullptr;
	ID3D11ShaderResourceView* m_stopBtnTexture = nullptr;
public:
	void Init() override;	// �������֐�
	void Uninit() override;
	void Update()override;	// ���t���[����������֐�

};


