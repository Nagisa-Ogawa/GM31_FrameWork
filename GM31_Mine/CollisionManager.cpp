
#include <algorithm>
#include "main.h"
#include "manager.h"
#include "scene.h"
#include "boxCollision.h"
#include "quadCollision.h"
#include "CollisionManager.h"

CollisionManager* CollisionManager::m_Instance = NULL;

CollisionManager::CollisionManager()
{
	// �Ȃɂ����Ȃ�
}

CollisionManager::CollisionManager(const CollisionManager & manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
}

CollisionManager & CollisionManager::operator=(const CollisionManager & manager)
{
	// �C���X�^���X���R�s�[
	m_Instance = manager.m_Instance;
	return *m_Instance;
}

void CollisionManager::Init()
{
}


CollisionManager::~CollisionManager()
{
	// �C���X�^���X�����
	delete m_Instance;
}

CollisionManager * CollisionManager::GetInstance()
{
	// ���߂Ďg���Ƃ��ɃC���X�^���X�𐶐�
	// ����ȍ~�͐��������C���X�^���X��n��
	if (m_Instance == NULL)
	{
		m_Instance = new CollisionManager();
		m_Instance->Init();
	}
	return m_Instance;
}

void CollisionManager::AddBoxCollision(BoxCollision * coll)
{
	if (coll == nullptr)
		return;
	if (coll->GetIsTrigger()) {
		m_BoxTriList.push_back(coll);
	}
	else {
		m_BoxCollList.push_back(coll);
	}
}

void CollisionManager::AddQuadCollision(QuadCollision * coll)
{
	if (coll == nullptr)
		return;
	if (coll->GetIsTrigger()) {
		m_QuadTriList.push_back(coll);
	}
	else {
		m_QuadCollList.push_back(coll);
	}
}

bool CollisionManager::Collision_BoxToBox(BoxCollision* a, BoxCollision* b)
{
	// 2�I�u�W�F�N�g�̊e�����`
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight() *   (a->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp() *      (a->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z /*/ 2.0f*/);
	D3DXVECTOR3 bVecX = b->GetGameObject()->GetTransform()->GetRight() *   (b->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 bVecY = b->GetGameObject()->GetTransform()->GetUp() *      (b->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 bVecZ = b->GetGameObject()->GetTransform()->GetForward() * (b->GetSize().z /*/ 2.0f*/);
	// 2�I�u�W�F�N�g�Ԃ̋���
	D3DXVECTOR3 distance = (b->GetGameObject()->GetTransform()->m_Position -
								a->GetGameObject()->GetTransform()->m_Position);

	// �I�u�W�F�N�gA��X�����������̏ꍇ
	D3DXVECTOR3 nAVecX;
	D3DXVec3Normalize(&nAVecX, &aVecX);
	// �I�u�W�F�N�gA�̓��e�����̔���
	float hpVecA = D3DXVec3Length(&aVecX);
	// �I�u�W�F�N�gB�̓��e�����̔���
	float hpVecB = CreateHalfProjectionLine(&nAVecX, &bVecX, &bVecY, &bVecZ);
	// 2�̃I�u�W�F�N�g�̒��S�_�Ԃ̋����𕪗�����ɓ��e
	float dDistance = fabs(D3DXVec3Dot(&distance, &nAVecX));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}
	// �I�u�W�F�N�gA��Y�����������̏ꍇ
	D3DXVECTOR3 nAVecY;
	D3DXVec3Normalize(&nAVecY, &aVecY);
	hpVecA = D3DXVec3Length(&aVecY);
	hpVecB = CreateHalfProjectionLine(&nAVecY, &bVecX, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &nAVecY));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}
	// �I�u�W�F�N�gA��Z�����������̏ꍇ
	D3DXVECTOR3 nAVecZ;
	D3DXVec3Normalize(&nAVecZ, &aVecZ);
	hpVecA = D3DXVec3Length(&aVecZ);
	hpVecB = CreateHalfProjectionLine(&nAVecZ, &bVecX, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &nAVecZ));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}
	// �I�u�W�F�N�gB��X�����������̏ꍇ
	D3DXVECTOR3 nBVecX;
	D3DXVec3Normalize(&nBVecX, &bVecX);
	hpVecA = CreateHalfProjectionLine(&nBVecX, &aVecX, &aVecY, &aVecZ);
	hpVecB = D3DXVec3Length(&bVecX);
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecX));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}
	// �I�u�W�F�N�gB��Y�����������̏ꍇ
	D3DXVECTOR3 nBVecY;
	D3DXVec3Normalize(&nBVecY, &bVecY);
	hpVecA = CreateHalfProjectionLine(&nBVecY, &aVecX, &aVecY, &aVecZ);
	hpVecB = D3DXVec3Length(&bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecY));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}
	// �I�u�W�F�N�gB��Z�����������̏ꍇ
	D3DXVECTOR3 nBVecZ;
	D3DXVec3Normalize(&nBVecZ, &bVecZ);
	hpVecA = CreateHalfProjectionLine(&nBVecZ, &aVecX, &aVecY, &aVecZ);
	hpVecB = D3DXVec3Length(&bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecZ));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}

	// �O�ϕ����� : �I�u�W�F�N�gA��X���ƃI�u�W�F�N�gB��X��
	D3DXVECTOR3 crossVec;
	D3DXVec3Cross(&crossVec, &nAVecX, &nBVecX);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecY, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��X���ƃI�u�W�F�N�gB��Y��
	D3DXVec3Cross(&crossVec, &nAVecX, &nBVecY);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecY, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��X���ƃI�u�W�F�N�gB��Z��
	D3DXVec3Cross(&crossVec, &nAVecX, &nBVecZ);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecY, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��Y���ƃI�u�W�F�N�gB��X��
	D3DXVec3Cross(&crossVec, &nAVecY, &nBVecX);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��Y���ƃI�u�W�F�N�gB��Y��
	D3DXVec3Cross(&crossVec, &nAVecY, &nBVecY);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��Y���ƃI�u�W�F�N�gB��Z��
	D3DXVec3Cross(&crossVec, &nAVecY, &nBVecZ);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecZ);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��Z���ƃI�u�W�F�N�gB��X��
	D3DXVec3Cross(&crossVec, &nAVecZ, &nBVecX);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecY);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecY, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��Z���ƃI�u�W�F�N�gB��Y��
	D3DXVec3Cross(&crossVec, &nAVecZ, &nBVecY);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecY);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecZ);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}
	// �O�ϕ����� : �I�u�W�F�N�gA��Z���ƃI�u�W�F�N�gB��Z��
	D3DXVec3Cross(&crossVec, &nAVecZ, &nBVecZ);
	hpVecA = CreateHalfProjectionLine(&crossVec, &aVecX, &aVecY);
	hpVecB = CreateHalfProjectionLine(&crossVec, &bVecX, &bVecY);
	dDistance = fabs(D3DXVec3Dot(&distance, &crossVec));
	if (hpVecA + hpVecB < dDistance) {
		return false;
	}

	// ���ׂĂɓ��Ă͂܂�Ȃ�������Փ˂��Ă���
	return true;
}

float CollisionManager::CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ)
{
	float r1 = fabs(D3DXVec3Dot(sAxis, vecX));
	float r2 = fabs(D3DXVec3Dot(sAxis, vecY));
	float r3 = 0.0f;
	if (vecZ) {
		r3 = fabs(D3DXVec3Dot(sAxis, vecZ));
	}

	return r1 + r2 + r3;
}


bool CollisionManager::Collision_BoxToQuad(BoxCollision * a, QuadCollision * b, float* l, D3DXVECTOR3* dir)
{
	D3DXVECTOR3 aPos = a->GetGameObject()->GetTransform()->m_Position;
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight()   * (a->GetSize().x);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp()      * (a->GetSize().y);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z);

	D3DXVECTOR3 n{};
	D3DXVec3Normalize(&n, b->GetNormal());
	D3DXVECTOR3 bPos = b->GetGameObject()->GetTransform()->m_Position;

	// �{�b�N�X�̊e���̒��_����e���_�܂ł̋����𕪗����i���ʂ̖@���j��ɓ��e�������̒������Z�o
	float r = 0.0f;
	r += fabs(D3DXVec3Dot(&aVecX, &n));
	r += fabs(D3DXVec3Dot(&aVecY, &n));
	r += fabs(D3DXVec3Dot(&aVecZ, &n));

	//�@�{�b�N�X�̒��_���畽�ʂ܂ł̋����𕪗����i���ʂ̖@���j��ɓ��e�������̒������Z�o
	float s = fabs(D3DXVec3Dot(&(aPos - bPos), &n));
	// �߂��������v�Z
	if (l != nullptr) {
		if (s > 0)
			*l = r - fabs(s);
		else
			*l = r + fabs(s);
		*dir = n;
	}
	// �Փ˔���
	// �{�b�N�X�̒��_����e���_�܂ł̒���(r)���{�b�N�X�ƕ��ʂ܂ł̒���(s)�����傫���Ȃ瓖�����Ă���
	if (fabs(s)-r < 0.0f) {
		return true;
	}
	return false;
}

