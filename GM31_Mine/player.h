#pragma once

#include "gameObject.h"
#include <string>

enum PLAYER_STATE
{
	GROUND,
	JUMP,
};

class Player:public GameObject
{

private:

	PLAYER_STATE m_PlayerState = GROUND;

	class AnimationModel* m_Model{};
	//int m_Time{};
	//float m_BlendRate{};
	//std::string m_AnimationName{};
	//std::string m_NextAnimationName{};

	bool m_IsGround{};
	bool m_IsHitEnemy{};
	bool m_IsHitWall{};
public:
	void Init() override;
	void Update() override;

	bool* GetIsHitEnemy() { return &m_IsHitEnemy; }
	bool* GetIsHitWall() { return &m_IsHitWall; }

	template <class Archive>
	void save(Archive& archive) const
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

	template <class Archive>
	void load(Archive& archive)
	{
		archive(
			cereal::base_class<GameObject>(this)
		);
	}

};

CEREAL_REGISTER_TYPE(Player);
