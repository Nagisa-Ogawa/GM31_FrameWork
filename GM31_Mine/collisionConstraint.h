#pragma once
#include <d3dx9math.h>


//-----------------------------------------------------
// Õ“Ë‚ÌS‘©‚ğ•\‚·ƒNƒ‰ƒX
//------------------------------------------------------
class CollisionConstraint
{
	D3DXVECTOR3 m_axis;		// S‘©²
	float m_cnExpDenom;		// S‘©®‚Ì•ª•ê
	float m_initCnImpulse;	// ‰ŠúS‘©—Í
	float m_lowerLimit;		// S‘©—Í‚Ì‰ºŒÀ
	float m_upperLimit;		// S‘©—Í‚ÌãŒÀ
	float m_accumImpulse;	// ’~Ï‚³‚ê‚éS‘©—Í
};
