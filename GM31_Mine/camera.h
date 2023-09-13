#pragma once

#include "component.h"


class Camera :public Component
{
private:
	D3DXVECTOR3 m_Target;
	D3DXMATRIX m_ViewMatrix;
	D3DXMATRIX m_ProjectionMatrix;
public:


	void Init(D3DXVECTOR3 target);
	void Uninit();
	void Update();
	void Draw();

	void SetTarget(D3DXVECTOR3 target) { m_Target = target; }
	D3DXMATRIX* GetViewMatrix() { return &m_ViewMatrix; }
	D3DXMATRIX* GetProjectionMatrix() {return &m_ProjectionMatrix;}
};

