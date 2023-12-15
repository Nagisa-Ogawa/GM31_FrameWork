#pragma once

#include "factory.h"

struct MODEL;
class  Ball;

class BallFactory : public Factory<BallFactory, Ball>
{
	friend class Factory<BallFactory, Ball>;
private:
	MODEL* m_pModel = nullptr;

	BallFactory();
	~BallFactory();

	void Init();
public:
	Ball* ActiveObject(std::string name) override;
	void HideObject(Ball* bullet) override;
	void Uninit() override;
	void Clear() override;

	void CollisionBallToBall();
};


