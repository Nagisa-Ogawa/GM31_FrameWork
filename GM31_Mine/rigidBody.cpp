
#include "main.h"
#include "rigidBody.h"


/// <summary>
/// ‰Šú‰»ˆ—
/// </summary>
void RigidBody::Init()
{
	m_mass = 1.0f;
	D3DXMatrixIdentity(&m_inertia);
	m_restitution = 0.2f;
	m_friction = 0.6f;
}

void RigidBody::Load()
{
}

void RigidBody::Uninit()
{
}

void RigidBody::Update()
{
}

void RigidBody::Draw()
{
}

void RigidBody::DispInspector()
{
}
