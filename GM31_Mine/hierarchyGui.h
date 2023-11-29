#pragma once
#include <string>
#include <map>
#include "MyImGui.h"


class HierarchyGui : public MyImGui
{
private:
	std::map<std::string, bool> m_gameObjecMap;
public:
	void Init() override;	// 初期化関数
	void Update()override;	// 毎フレーム処理する関数
};


