
#include <algorithm>
#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "CollisionManager.h"
#include "boxCollision.h"
#include "quadCollision.h"
#include "sphereCollision.h"
#include "polygonCollision.h"
#include "Ray.h"

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

void CollisionManager::Uninit()
{
	m_BoxCollList.clear();
	m_SphereCollList.clear();
	m_QuadCollList.clear();
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
	if (coll == nullptr) return;
	m_BoxCollList.push_back(coll);
}

void CollisionManager::AddQuadCollision(QuadCollision * coll)
{
	if (coll == nullptr) return;
	m_QuadCollList.push_back(coll);
}

void CollisionManager::AddSphereCollision(SphereCollision* coll)
{
	if (coll == nullptr) return;
	m_SphereCollList.push_back(coll);
}

void CollisionManager::AddPolygonCollision(PolygonCollision* coll)
{
	if (coll == nullptr) return;
	m_PolygonCollList.push_back(coll);
}

//-------------------------------------
// OBB��OBB�̓����蔻��
//-------------------------------------
// �߂�l : �����������ǂ���
//-------------------------------------
// 
// a       : OBB�p�R���W��������1
// b       : OBB�p�R���W��������2
// out_L   : �߂荞��OBB��߂�����
// out_Dir : �߂荞��OBB��߂�����
//-------------------------------------

bool CollisionManager::Collision_BoxToBox(BoxCollision* a, BoxCollision* b, float* out_L, D3DXVECTOR3* out_Dir)
{
	// 2�I�u�W�F�N�g�̊e�����`
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight() *   (a->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp() *      (a->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z /*/ 2.0f*/);
	D3DXVECTOR3 bVecX = b->GetGameObject()->GetTransform()->GetRight() *   (b->GetSize().x /*/ 2.0f*/);
	D3DXVECTOR3 bVecY = b->GetGameObject()->GetTransform()->GetUp() *      (b->GetSize().y /*/ 2.0f*/);
	D3DXVECTOR3 bVecZ = b->GetGameObject()->GetTransform()->GetForward() * (b->GetSize().z /*/ 2.0f*/);
	// 2�I�u�W�F�N�g�Ԃ̋���
	D3DXVECTOR3 distance = ((b->GetGameObject()->GetTransform()->m_worldPosition + b->GetOffset()) -
									(a->GetGameObject()->GetTransform()->m_worldPosition + a->GetOffset()));

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
	// ���������ۂ̈����߂������ƒ������擾
	GetHitBoxSurface(a, b, out_L, out_Dir);

	return true;
}

//---------------------------------------------------------
// ��������������Е���OBB�̓��e�����̔��������߂�֐�
//---------------------------------------------------------
// �߂�l : �����Е���OBB�̓��e�����̔���
//---------------------------------------------------------
// ssAxis : ������
// vecX   : �I�u�W�F�N�g��X��
// vecY   : �I�u�W�F�N�g��Y��
// vecZ   : �I�u�W�F�N�g��Z��
//---------------------------------------------------------
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

//-------------------------------------
// OBB��OBB���Փ˂����ۂɊ�Ƃ���OBB�����������OBB�̂ǂ̖ʂƏՓ˂��Ă��邩�����m��
// �߂荞�񂾕�����߂������ƕ������Z�o����֐�
// //----------------------------------
// �߂�l : void
//-------------------------------------
// a       : ��ƂȂ�OBB�p�R���W����
// b       : ���������OBB�p�R���W����
// out_L   : �߂荞��OBB��߂�����
// out_Dir : �߂荞��OBB��߂�����
//-------------------------------------
void CollisionManager::GetHitBoxSurface(BoxCollision* a, BoxCollision* b, float* out_L, D3DXVECTOR3* out_Dir)
{
	// b��6�̕��ʂƂ��ĕ���
	// �e�v�f�̕ϐ���6���p��
	D3DXVECTOR3 bPos = b->GetGameObject()->GetTransform()->m_worldPosition + b->GetOffset();
	D3DXVECTOR3 bPoss[6] = { bPos,bPos,bPos,bPos,bPos,bPos };
	D3DXVECTOR2 bSizes[6];
	D3DXVECTOR3 bVecs[6][2];
	D3DXVECTOR3 bNormals[6];
	// ����
	bPoss   [0] -= b->GetGameObject()->GetTransform()->GetForward() * b->GetSize().z;
	bSizes  [0] = D3DXVECTOR2(b->GetSize().x, b->GetSize().y);
	bVecs   [0][0]= b->GetGameObject()->GetTransform()->GetRight();
	bVecs   [0][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[0] = b->GetGameObject()->GetTransform()->GetForward() * -1.0f;
	// ���
	bPoss   [1] += b->GetGameObject()->GetTransform()->GetForward() * b->GetSize().z;
	bSizes  [1] = D3DXVECTOR2(b->GetSize().x, b->GetSize().y);
	bVecs   [1][0] = b->GetGameObject()->GetTransform()->GetRight();
	bVecs   [1][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[1] = b->GetGameObject()->GetTransform()->GetForward();
	// �E
	bPoss   [2] += b->GetGameObject()->GetTransform()->GetRight() * b->GetSize().x;
	bSizes  [2] = D3DXVECTOR2(b->GetSize().z, b->GetSize().y);
	bVecs   [2][0] = b->GetGameObject()->GetTransform()->GetForward();
	bVecs   [2][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[2] = b->GetGameObject()->GetTransform()->GetRight();
	// ��
	bPoss   [3] -= b->GetGameObject()->GetTransform()->GetRight() * b->GetSize().x;
	bSizes  [3] = D3DXVECTOR2(b->GetSize().z, b->GetSize().y);
	bVecs   [3][0] = b->GetGameObject()->GetTransform()->GetForward() * -1.0f;
	bVecs   [3][1] = b->GetGameObject()->GetTransform()->GetUp();
	bNormals[3] = b->GetGameObject()->GetTransform()->GetRight() * -1.0f;
	// ��
	bPoss   [4] += b->GetGameObject()->GetTransform()->GetUp() * b->GetSize().y;
	bSizes  [4] = D3DXVECTOR2(b->GetSize().x, b->GetSize().z);
	bVecs   [4][0] = b->GetGameObject()->GetTransform()->GetRight();
	bVecs   [4][1] = b->GetGameObject()->GetTransform()->GetForward();
	bNormals[4] = b->GetGameObject()->GetTransform()->GetUp();
	// ��
	bPoss   [5] -= b->GetGameObject()->GetTransform()->GetUp() * b->GetSize().y;
	bSizes  [5] = D3DXVECTOR2(b->GetSize().x, b->GetSize().z);
	bVecs   [5][0] = b->GetGameObject()->GetTransform()->GetRight() * -1.0f;
	bVecs   [5][1] = b->GetGameObject()->GetTransform()->GetForward();
	bNormals[5] = b->GetGameObject()->GetTransform()->GetUp() * -1.0f;

	float minL = -1.0f;
	D3DXVECTOR3 minDir;
	// �ǂ̕��ʂ�a���Փ˂��Ă��邩���`�F�b�N
	for (int i = 0; i < 6; i++) {
		float l = -1.0f;
		D3DXVECTOR3 dir;
		// OBB�ƕ��ʂ̓����蔻��
		if (Collision_BoxToQuad(a, bPoss[i], bSizes[i], bVecs[i], bNormals[i], &l, &dir)) {
			// �Փ˂��Ă����Ȃ牟���߂��������r
			if (minL < 0.0f) {
				minL = l;
				minDir = dir;
			}
			else {
				if (minL > l) {
					minL = l;
					minDir = dir;
				}
			}
		}
	}
	if (minL >= 0.0f) {
		*out_L = minL;
		*out_Dir = minDir;
	}
}

//-------------------------------------
// OBB�Ɣ|���S���̓����蔻��
// //----------------------------------
// �߂�l : �����������ǂ���
//-------------------------------------
// a       : OBB�p�R���W����
// b       : �|���S���p�R���W����
// out_L   : �߂荞��OBB��߂�����
// out_Dir : �߂荞��OBB��߂�����
//-------------------------------------
bool CollisionManager::Collision_BoxToQuad(BoxCollision * a, QuadCollision * b, float* out_L, D3DXVECTOR3* out_Dir)
{
	D3DXVECTOR3 aPos = a->GetGameObject()->GetTransform()->m_worldPosition;
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight()   * (a->GetSize().x);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp()      * (a->GetSize().y);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z);

	D3DXVECTOR3 bPos = b->GetGameObject()->GetTransform()->m_worldPosition;
	D3DXVECTOR3 bVecX = b->GetGameObject()->GetTransform()->GetRight() * (b->GetSize()->x);
	D3DXVECTOR3 bVecZ = b->GetGameObject()->GetTransform()->GetForward() * (b->GetSize()->y);


	D3DXVECTOR3 n{};
	D3DXVec3Normalize(&n, b->GetNormal());

	// 2�I�u�W�F�N�g�Ԃ̋���
	D3DXVECTOR3 distance = (b->GetGameObject()->GetTransform()->m_worldPosition -
		a->GetGameObject()->GetTransform()->m_worldPosition);

	// �{�b�N�X��z�����ȊO�œ������Ă��邩���`�F�b�N
	D3DXVECTOR3 nBVecX;
	D3DXVec3Normalize(&nBVecX, &bVecX);
	// �I�u�W�F�N�g�̓��e�����̔���
	float hpVecA = CreateHalfProjectionLine(&nBVecX, &aVecX, &aVecY, &aVecZ);
	// �I�u�W�F�N�gB�̓��e�����̔���
	float hpVecB = D3DXVec3Length(&bVecX);
	// 2�̃I�u�W�F�N�g�̒��S�_�Ԃ̋����𕪗�����ɓ��e
	float dDistance = fabs(D3DXVec3Dot(&distance, &nBVecX));
	if (hpVecA + hpVecB <= dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}


	D3DXVECTOR3 nBVecZ;
	D3DXVec3Normalize(&nBVecZ, &bVecZ);
	// �I�u�W�F�N�g�̓��e�����̔���
	hpVecA = CreateHalfProjectionLine(&nBVecZ, &aVecX, &aVecY, &aVecZ);
	// �I�u�W�F�N�gB�̓��e�����̔���
	hpVecB = D3DXVec3Length(&bVecZ);
	// 2�̃I�u�W�F�N�g�̒��S�_�Ԃ̋����𕪗�����ɓ��e
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecZ));
	if (hpVecA + hpVecB <= dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}

	// �{�b�N�X�̊e���̒��_����e���_�܂ł̋����𕪗����i���ʂ̖@���j��ɓ��e�������̒������Z�o
	float r = 0.0f;
	r += fabs(D3DXVec3Dot(&aVecX, &n));
	r += fabs(D3DXVec3Dot(&aVecY, &n));
	r += fabs(D3DXVec3Dot(&aVecZ, &n));

	//�@�{�b�N�X�̒��_���畽�ʂ܂ł̋����𕪗����i���ʂ̖@���j��ɓ��e�������̒������Z�o
	D3DXVECTOR3 dis = aPos - bPos;
	float s = fabs(D3DXVec3Dot(&dis, &n));
	// �߂��������v�Z
	if (out_L != nullptr) {
		if (s > 0)
			*out_L = r - fabs(s);
		else
			*out_L = r + fabs(s);
		*out_Dir = n;
	}
	// �Փ˔���
	// �{�b�N�X�̒��_����e���_�܂ł̒���(r)���{�b�N�X�ƕ��ʂ܂ł̒���(s)�����傫���Ȃ瓖�����Ă���
	if (fabs(s)-r < 0.0f) {
		return true;
	}
	return false;
}

//-------------------------------------
// OBB�ƕ��ʂ̓����蔻��(�I�[�o�[���[�h)
// //----------------------------------
// �߂�l : �����������ǂ���
//-------------------------------------
// a       : OBB�p�R���W����
// bPos    : ���ʂ̒��S���W
// bSize   : ���ʂ̃T�C�Y
// bVec[2] : ���ʂ̃��[���h���W�ł̌���(�c�Ɖ�)
// bNormal : ���ʂ̖@���x�N�g��
// out_L   : �߂荞��OBB��߂�����
// out_Dir : �߂荞��OBB��߂�����
//-------------------------------------

bool CollisionManager::Collision_BoxToQuad(BoxCollision* a, D3DXVECTOR3 bPos, 
		D3DXVECTOR2 bSize, D3DXVECTOR3 bVec[2], D3DXVECTOR3 bNormal, float* out_L, D3DXVECTOR3* out_Dir)
{
	D3DXVECTOR3 aPos = a->GetGameObject()->GetTransform()->m_worldPosition;
	D3DXVECTOR3 aVecX = a->GetGameObject()->GetTransform()->GetRight() * (a->GetSize().x);
	D3DXVECTOR3 aVecY = a->GetGameObject()->GetTransform()->GetUp() * (a->GetSize().y);
	D3DXVECTOR3 aVecZ = a->GetGameObject()->GetTransform()->GetForward() * (a->GetSize().z);

	D3DXVECTOR3 bVecX = bVec[0] * bSize.x;
	D3DXVECTOR3 bVecZ = bVec[1] * bSize.y;


	D3DXVECTOR3 n{};
	D3DXVec3Normalize(&n, &bNormal);

	// 2�I�u�W�F�N�g�Ԃ̋���
	D3DXVECTOR3 distance = (bPos -
		a->GetGameObject()->GetTransform()->m_worldPosition);

	// �{�b�N�X��z�����ȊO�œ������Ă��邩���`�F�b�N
	D3DXVECTOR3 nBVecX;
	D3DXVec3Normalize(&nBVecX, &bVecX);
	// �I�u�W�F�N�g�̓��e�����̔���
	float hpVecA = CreateHalfProjectionLine(&nBVecX, &aVecX, &aVecY, &aVecZ);
	// �I�u�W�F�N�gB�̓��e�����̔���
	float hpVecB = D3DXVec3Length(&bVecX);
	// 2�̃I�u�W�F�N�g�̒��S�_�Ԃ̋����𕪗�����ɓ��e
	float dDistance = fabs(D3DXVec3Dot(&distance, &nBVecX));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}


	D3DXVECTOR3 nBVecZ;
	D3DXVec3Normalize(&nBVecZ, &bVecZ);
	// �I�u�W�F�N�g�̓��e�����̔���
	hpVecA = CreateHalfProjectionLine(&nBVecZ, &aVecX, &aVecY, &aVecZ);
	// �I�u�W�F�N�gB�̓��e�����̔���
	hpVecB = D3DXVec3Length(&bVecZ);
	// 2�̃I�u�W�F�N�g�̒��S�_�Ԃ̋����𕪗�����ɓ��e
	dDistance = fabs(D3DXVec3Dot(&distance, &nBVecZ));
	if (hpVecA + hpVecB < dDistance) {
		// �Փ˂��Ă��Ȃ�
		return false;
	}

	// �{�b�N�X�̊e���̒��_����e���_�܂ł̋����𕪗����i���ʂ̖@���j��ɓ��e�������̒������Z�o
	float r = 0.0f;
	r += fabs(D3DXVec3Dot(&aVecX, &n));
	r += fabs(D3DXVec3Dot(&aVecY, &n));
	r += fabs(D3DXVec3Dot(&aVecZ, &n));

	//�@�{�b�N�X�̒��_���畽�ʂ܂ł̋����𕪗����i���ʂ̖@���j��ɓ��e�������̒������Z�o
	D3DXVECTOR3 dis = aPos - bPos;
	float s = fabs(D3DXVec3Dot(&dis, &n));
	// �߂��������v�Z
	if (out_L != nullptr) {
		if (s > 0)
			*out_L = r - fabs(s);
		else
			*out_L = r + fabs(s);
		*out_Dir = n;
	}
	// �Փ˔���
	// �{�b�N�X�̒��_����e���_�܂ł̒���(r)���{�b�N�X�ƕ��ʂ܂ł̒���(s)�����傫���Ȃ瓖�����Ă���
	if (fabs(s) - r < 0.0f) {
		return true;
	}
	return false;

}


#define PREVENT_ERROR 0.00001f		// �덷�p�萔
//------------------------------------------------------------------
// ���C�Ƌ��̃R���W�����̓����蔻��
//------------------------------------------------------------------
// �߂�l : �����������ǂ���
//------------------------------------------------------------------
// ray        : ���C�p�N���X
// sphereColl : ���̗p�R���W����
// out_T      : �ڐG�����ۂ̃��C�̎���
// out_HitPos : �ڐG�����ۂ̍��W
// ����! �����蔻��͋��̃R���W�����̃��[�J�����W�Ŕ��肵�Ă��邽��
// �ڐG�������W���g�p����ꍇ�̓��[���h���W�֍��W�ϊ�����K�v������
//-------------------------------------------------------------------
bool CollisionManager::Collision_RayToSphere(Ray* ray, SphereCollision* sphereColl, float* out_T, D3DXVECTOR3* out_HitPos)
{
	// D3DXVECTOR3 distance = *(ray->GetStartPos()) - (sphereColl->GetGameObject()->GetTransform()->m_Position + sphereColl->GetOffset());
	float r = 1.0f;
	double a = D3DXVec3Dot(ray->GetVec(), ray->GetVec());
	double b = D3DXVec3Dot(ray->GetStartPos() ,ray->GetVec());
	double c = D3DXVec3Dot(ray->GetStartPos(), ray->GetStartPos()) - (r * r);

	// �덷
	// a�͒P�ʃx�N�g���̓��ς̂��ߌ덷�ȊO�ł͕K��1�ɂȂ�
	if (a - PREVENT_ERROR <= 0.0f) {
		return false;
	}

	// isColl���}�C�i�X�ɂȂ�ꍇ��t�͎����������Ȃ����߃��C�Ƌ��͓������Ă��Ȃ�
	float isColl = b * b - (a * c);
	if (isColl < 0.0f) {
		return false;
	}

	// ���̌�������
	float t = ( -b - sqrt(b * b - a * c)) / a;

	if (out_T) {
		*out_T = t;
	}
	if (out_HitPos) {
		*out_HitPos = ray->GetRayPos(t);
	}

	return true;
}

//-----------------------------------------------------------------------
// ���C�ƃ{�b�N�X�R���W�����̓����蔻��
//-----------------------------------------------------------------------
// �߂�l : �����������ǂ���
//-----------------------------------------------------------------------
// ray			: ���C�p�N���X
// boxColl		: �{�b�N�X�p�R���W�����̃T�C�Y(boxCollision.obj�̃T�C�Y)
// out_T		: �ڐG�����ۂ̃��C�̎���
// out_HitPos	: �ڐG�����ۂ̍��W
// ����! �����蔻��̓{�b�N�X�R���W�����̃��[�J�����W�Ŕ��肵�Ă��邽��
// �ڐG�������W���g�p����ꍇ�̓��[���h���W�֍��W�ϊ�����K�v������܂�
//------------------------------------------------------------------------
bool CollisionManager::Collision_RayToBox(Ray* ray, BoxCollision* boxColl, float* out_T, D3DXVECTOR3* out_HitPos)
{
	D3DXVECTOR3 h = D3DXVECTOR3(1.0f,1.0f,1.0f);
	D3DXVECTOR3 p = *(ray->GetStartPos());
	D3DXVECTOR3 d = *(ray->GetVec());
	bool xFlag = false, yFlag = false, zFlag = false;
	float tx1 = 0.0f;
	float tx2 = 0.0f;
	float ty1 = 0.0f;
	float ty2 = 0.0f;
	float tz1 = 0.0f;
	float tz2 = 0.0f;
	// X������
	if (d.x == 0.0f) {
		// -h.x<=p.x<=h.x�łȂ��Ȃ瓖�����Ă��Ȃ�
		if (-h.x > p.x || p.x > h.x)
			return false;
		xFlag = true;
	}
	else {
		// tx1 = min{(h.x-p.x)/d.x , (-h.x-p.x)/d.x}
		// tx2 = max{(h.x-p.x)/d.x , (-h.x-p.x)/d.x}
		tx1 = std::min((h.x - p.x) / d.x, ( -h.x - p.x) / d.x);
		tx2 = std::max((h.x - p.x) / d.x, (-h.x - p.x) / d.x);
	}
	if (d.y == 0.0f) {
		// Y������
		if (-h.y > p.y || p.y > h.y)
			return false;
		yFlag = true;
	}
	else {
		ty1 = std::min((h.y - p.y) / d.y, (-h.y - p.y) / d.y);
		ty2 = std::max((h.y - p.y) / d.y, (-h.y - p.y) / d.y);
	}
	// Z������
	if (d.z == 0.0f) {
		if (-h.z > p.z || p.z > h.z)
			return false;
		zFlag = true;
	}
	else {
		tz1 = std::min((h.z - p.z) / d.z, (-h.z - p.z) / d.z);
		tz2 = std::max((h.z - p.z) / d.z, (-h.z - p.z) / d.z);
	}

	// �e����t1,t2����t���Z�o
	float t = -99999.0f;

	if (!xFlag) {
		if (tx1 >= 0.0f) {
			if (tx1 <= ty2 && tx1 <= tz2 &&
					tx1 >= ty1 && tx1 >= tz1) {
				if (t >= tx1 || t < 0.0f) {
					t = tx1;
				}
			}
		}
	}
	if (!yFlag) {
		if (ty1 >= 0.0f) {
			if (ty1 <= tx2 && ty1 <= tz2 &&
					ty1 >= tx1 && ty1 >= tz1) {
				if (t >= ty1 || t < 0.0f) {
					t = ty1;
				}
			}
		}
	}
	if (!zFlag) {
		if (tz1 >= 0.0f) {
			if (tz1 <= tx2 && tz1 <= ty2 &&
					tz1 >= tx1 && tz1 >= ty1) {
				if (t >= tz1 || t < 0.0f) {
					t = tz1;
				}
			}
		}
	}



	if (t >= 0.0f) {
		if (out_T) {
			*out_T = t;
		}
		if (out_HitPos) {
			*out_HitPos = ray->GetRayPos(t);
		}
		return true;
	}
	else {
		return false;
	}
}

bool CollisionManager::Collision_RayToPolygon(Ray* ray, PolygonCollision* polyColl, float* out_T)
{
	const float EPSILON = 1e-6f;	// �v�Z�덷�����p�ϐ�
	POLYGON_POSITION* polyPosArray = polyColl->GetPolygonArray();	// �|���S���̒��_���W�z��
	// �|���S���̐��������[�v
	for (int i = 0; i < polyColl->GetArrayCount(); i++) {
		POLYGON_POSITION poly = polyPosArray[i];
		D3DXVECTOR3 edgeA = poly.position[1] - poly.position[0];
		D3DXVECTOR3 edgeB = poly.position[2] - poly.position[0];

		D3DXVECTOR3 v1;
		D3DXVec3Cross(&v1, ray->GetVec(), &edgeB);
		float det = D3DXVec3Dot(&edgeA, &v1);

		// �O�p�|���S���Ƀ��C�����s�Ɍ�����Ă��鎞��det = 0�ɂȂ�
		// det��0�Ɍ���Ȃ��߂��Ȃ�������Ă��Ȃ�����ɂ���
		if (-EPSILON < det && det < EPSILON) continue;

		float invDet = 1.0f / det;
		D3DXVECTOR3 r = *(ray->GetStartPos()) - poly.position[0];
		// u�� 0<= u <= 1 ���ł͂Ȃ��Ȃ瓖�����Ă��Ȃ�
		float u = D3DXVec3Dot(&v1, &r) * invDet;
		if (u < 0.0f || u>1.0f) continue;

		D3DXVECTOR3 v2;
		D3DXVec3Cross(&v2, &r, &edgeA);
		// v�� 0 <= v <= 1 ���� u + v <= 1 �łȂ��Ȃ瓖�����Ă��Ȃ�
		float v = D3DXVec3Dot(ray->GetVec(), &v2) * invDet;
		if (v < 0.0f || u + v>1.0f) continue;

		// t�� 0 <= t �Ȃ瓖�����ĂȂ�
		float t = D3DXVec3Dot(&edgeB, &v2) * invDet;
		if (t < 0.0f) continue;

		// �������Ă���
		*out_T = t;
		return true;
	}
	// �S�Ẵ|���S����������Ȃ������瓖�����Ă��Ȃ�
	return false;
}


//------------------------------------------------------------------
// �X�N���[�����W�̍��W�����[�J�����W�֕ϊ�����֐�
//------------------------------------------------------------------
// �߂�l : �Ȃ�
//------------------------------------------------------------------
// worldMatrix		: ���[���h�ϊ��s��
// viewMatrix		: �r���[�ϊ��s��
// projectionMatrix	: �v���W�F�N�V�����ϊ��s��
// mousePos			: �ϊ�����X�N���[�����W
// mouseZ			: �X�N���[�����W�ɕt��������z����(0.0�`1.0)
// out_Pos			: �ϊ���̍��W
//-------------------------------------------------------------------
void CollisionManager::ScreenToLocalPosition(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix, 
						D3DXMATRIX* projectionMatrix, POINT mousePos, float mouseZ, D3DXVECTOR3* out_Pos)
{
	// �r���[�|�[�g�s����쐬
	D3DXMATRIX vpMatrix;
	D3DXMatrixIdentity(&vpMatrix);
	D3D11_VIEWPORT vp;
	UINT numViewports = 1;
	Renderer::GetDeviceContext()->RSGetViewports(&numViewports, &vp);

	vpMatrix._11 = (float)vp.Width / 2;
	vpMatrix._22 = -1.0f * (float)(vp.Height / 2);
	vpMatrix._33 = (float)vp.MaxDepth - vp.MinDepth;
	vpMatrix._41 = (float)(vp.TopLeftX + vp.Width / 2);
	vpMatrix._42 = (float)(vp.TopLeftY + vp.Height / 2);
	vpMatrix._43 = vp.MinDepth;

	D3DXVECTOR3 world;
	world.x = (float)mousePos.x;
	world.y = (float)mousePos.y;
	world.z = mouseZ;


	D3DXMATRIX invMatrix, invViewport, invPorjection, invView, invWorld;
	// �r���[�|�[�g�s��̋t�s����쐬
	D3DXMatrixInverse(&invViewport, 0, &vpMatrix);
	// �v���W�F�N�V�����s��̋t�s����쐬
	D3DXMatrixInverse(&invPorjection, 0, projectionMatrix);
	// �r���[�s��̋t�s��
	D3DXMatrixInverse(&invView, 0, viewMatrix);
	// ���[���h�s��̋t�s��
	D3DXMatrixInverse(&invWorld, 0, worldMatrix);
	// �S�Ă̋t�s��������ă��[�J�����W�n��
	invMatrix = invViewport * invPorjection * invView * invWorld;
	// �X�N���[�����W�����[�J�����W��
	D3DXVec3TransformCoord(&world, &world, &invMatrix);

	if (out_Pos) {
		*out_Pos = world;
	}

}

