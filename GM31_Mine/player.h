#pragma once

#include "gameObject.h"
#include <string>

class Player:public GameObject
{

private:
	class AnimationModel* m_Model{};
	int m_Time{};
	float m_BlendRate{};
	std::string m_AnimationName{};
	std::string m_NextAnimationName{};

	bool m_IsGround{};
	bool m_IsHitEnemy{};
	bool m_IsHitWall{};
public:
	void Init() override;
	void Update() override;

	bool* GetIsHitEnemy() { return &m_IsHitEnemy; }
	bool* GetIsHitWall() { return &m_IsHitWall; }
};
