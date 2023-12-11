#pragma once

#include "scene.h"


class Editor : public Scene {

private:
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

};
