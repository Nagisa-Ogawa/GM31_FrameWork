#pragma once

class MyImGui 
{
private:
public:
	virtual void Init() = 0;	// �������֐�
	virtual void Uninit() {};
	virtual void Update() = 0;	// ���t���[����������֐�
};
