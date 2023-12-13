#pragma once

#include "gameObject.h"


class CameraObject:public GameObject
{
private:
	bool	m_isRButton = false;	// 右クリックを押しているかのフラグ
	POINT	m_startMousePos{};		// 右クリックを押したときのマウス座標
public:
	void Init() override;
	void Update() override;
	
};
