#pragma once

#include <list>

class BoxCollision;
class QuadCollision;
class SphereCollision;
class Ray;

class CollisionManager
{
private:
	// �V���O���g���p�C���X�^���X
	static CollisionManager* m_Instance;
	// �R���X�g���N�^
	CollisionManager();
	// �R�s�[�R���X�g���N�^
	CollisionManager(const CollisionManager& manager);
	// ������Z�q
	CollisionManager& operator=(const CollisionManager& manager);

	// �e�R���W�����p���X�g
	std::list<BoxCollision*> m_BoxCollList;
	std::list<QuadCollision*> m_QuadCollList;
	std::list<SphereCollision*> m_SphereCollList;

	// ����������
	void Init();
	// ���e�����̔����̒������擾����֐�
	float CreateHalfProjectionLine(D3DXVECTOR3* sAxis, D3DXVECTOR3* vecX, D3DXVECTOR3* vecY, D3DXVECTOR3* vecZ = 0);
	// OBB��OBB���Փ˂����ۂ�OBB�����������OBB�̂ǂ̖ʂƏՓ˂��Ă��邩�����m��
	// �߂荞�񂾕�����߂������ƕ������Z�o����֐�
	void GetHitBoxSurface(BoxCollision* a, BoxCollision* b, float* l, D3DXVECTOR3* dir);
public:
	// �f�X�g���N�^
	virtual ~CollisionManager();
	static CollisionManager* GetInstance();

	// �R���W�����R���|�[�l���g���擾����֐�
	void AddBoxCollision(BoxCollision* coll);
	void AddQuadCollision(QuadCollision* coll);
	void AddSphereCollision(SphereCollision* coll);

	//----------------------------
	// �����蔻��֌W
	//----------------------------
	// OBB��OBB�̓����蔻��
	bool Collision_BoxToBox(BoxCollision* a, BoxCollision* b, float* l, D3DXVECTOR3* dir);
	// OBB�Ɣ|���S���̓����蔻��
	bool Collision_BoxToQuad(BoxCollision* a, QuadCollision* b, float* l,D3DXVECTOR3* dir);
	// OBB�ƕ��ʂ̓����蔻��
	bool Collision_BoxToQuad(BoxCollision* a, D3DXVECTOR3 bPos,D3DXVECTOR3 bSize,
						D3DXVECTOR3 bVec[2], D3DXVECTOR3 bNormal, float* l, D3DXVECTOR3* dir);
	// ���C�Ƌ��̂̓����蔻��
	bool Collision_RayToSphere(Ray* ray, SphereCollision* sphereColl, float* out_T, D3DXVECTOR3* out_HitPos);
	// ���C�Ɨ����̂̓����蔻��
	bool Collision_RayToBox(Ray* ray, BoxCollision* boxColl, float* out_T, D3DXVECTOR3* out_HitPos);
	// �X�N���[�����W�i�N���C�A���g���W�j���烍�[�J�����W�n�ւ̍��W�ϊ�
	void ScreenToLocalPosition(D3DXMATRIX* worldMatrix, D3DXMATRIX* viewMatrix,
				D3DXMATRIX* projectionMatrix, POINT mousePos, float mouseZ, D3DXVECTOR3* out_Pos);

	std::list<BoxCollision*> GetBoxCollList() { return m_BoxCollList; }
	std::list<QuadCollision*> GetQuadCollList() { return m_QuadCollList; }
	std::list<SphereCollision*> GetSphereCollList() { return m_SphereCollList; }

};