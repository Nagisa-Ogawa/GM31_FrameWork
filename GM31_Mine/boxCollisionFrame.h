#pragma once

#include "gameObject.h"

class BoxCollisionFrame : public GameObject
{

private:
	class BoxCollision* m_collision = nullptr;
public:
	void Init(BoxCollision* collision);
	void Draw() override;
};