#pragma once
#include <string>
#include <map>
#include "MyImGui.h"


class HierarchyGui : public MyImGui
{
private:
	std::map<std::string, bool> m_gameObjecMap;
public:
	void Init() override;	// �������֐�
	void Update()override;	// ���t���[����������֐�
};


