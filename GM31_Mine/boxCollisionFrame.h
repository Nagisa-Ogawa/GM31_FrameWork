#pragma once

#include "gameObject.h"

class BoxCollisionFrame : public GameObject
{

private:
	class BoxCollision* m_collision = nullptr;
public:
	void Init(BoxCollision* collision);
	void Draw() override;

	void SetCollision(BoxCollision* coll) { m_collision = coll; }

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

CEREAL_REGISTER_TYPE(BoxCollisionFrame);