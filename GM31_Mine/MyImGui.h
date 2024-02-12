#pragma once

class MyImGui 
{
private:
public:
	virtual void Init() = 0;	// ‰Šú‰»ŠÖ”
	virtual void Uninit() {};
	virtual void Update() = 0;	// –ˆƒtƒŒ[ƒ€ˆ—‚·‚éŠÖ”
};
