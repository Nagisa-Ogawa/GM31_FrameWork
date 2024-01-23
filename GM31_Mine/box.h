#pragma once

#include "gameObject.h"

class Box : public GameObject
{
	void Init() override;

	template <class Archive>
	void save(Archive& archive)
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

CEREAL_REGISTER_TYPE(Box);

