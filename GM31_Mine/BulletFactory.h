#pragma once

#include <list>
#include "factory.h"

struct MODEL;
class  Bullet;

class BulletFactory : public Factory<BulletFactory, Bullet>
{
	friend class Factory<BulletFactory, Bullet>;
private:
	MODEL* m_pModel = nullptr;

	BulletFactory();
	~BulletFactory();

	void Init();
public:
	Bullet* ActiveObject() override;
	void HideObject(Bullet* bullet) override;
	void Uninit() override;
	void Clear() override;
};


