#pragma once

#include "component.h"


class Camera :public Component
{
private:
	D3DXVECTOR3 m_position{};
	D3DXVECTOR3 m_target{};
	D3DXVECTOR3 m_right{};
	D3DXVECTOR3 m_up{};


	D3DXMATRIX m_viewMatrix{};
	D3DXMATRIX m_projectionMatrix{};
public:


	void Init(D3DXVECTOR3 position); 
	void Uninit() override;
	void Update() override;
	void Draw() override;

	D3DXVECTOR3 GetPosition()	{ return m_position; }
	D3DXVECTOR3 GetTarget()		{ return m_target; }
	D3DXVECTOR3 GetUp()			{ return m_up; };

	D3DXMATRIX* GetViewMatrix()	{ return &m_viewMatrix; }
	D3DXMATRIX* GetProjectionMatrix() {return &m_projectionMatrix;}

	void SetPosition(D3DXVECTOR3 position)	{ m_position = position; }
	void SetTarget(D3DXVECTOR3 target)		{ m_target = target; }
	void SetUp(D3DXVECTOR3 up)				{ m_up = up; }
};

