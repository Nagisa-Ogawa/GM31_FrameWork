#pragma once

#include "component.h"


class TitleCamera :public Component
{
private:
	D3DXVECTOR3 m_Target;
	D3DXMATRIX m_ViewMatrix;
public:


	void Init(D3DXVECTOR3 target);
	void Uninit();
	void Update();
	void Draw();

	void SetTarget(D3DXVECTOR3 target) { m_Target = target; }
};

