#pragma once
#include <d3dx9math.h>

#define COLLISION_MESH_MAX_VERTICES	(34)	// �����蔻��p���b�V���f�[�^�̍ő咸�_��
#define COLLISION_MESH_MAX_EDGES	(96)	// �����蔻��p���b�V���f�[�^�̍ő�Ӑ�
#define COLLISION_MESH_MAX_FACES	(64)	// �����蔻��p���b�V���f�[�^�̍ő�ʐ�

/// �G�b�W�̎��
enum EdgeType {
	EDGE_TYPE_CONVEX,	// �ʃG�b�W
	EDGE_TYPE_CONCAVE,	// ���G�b�W
	EDGE_TYPE_FLAT,		// ���R�G�b�W
};

/// �G�b�W
struct Edge {
	int type;	// �G�b�W�̎��
	int vertexId[2];	// �[�_�̒��_�C���f�b�N�X
	int faceId[2];	// ���L����ʃC���f�b�N�X
};

/// �R�p�`��
struct Face {
	int vertexId[3]; ///< ���_�C���f�b�N�X
	int edgeId[3]; ///< �G�b�W�C���f�b�N�X
	D3DXVECTOR3 normal; ///< �ʖ@���x�N�g��
};


//-----------------------------------------------------
// �����蔻��Ɏg�p����ʃ��b�V���f�[�^��\���N���X
//------------------------------------------------------
class CollisionMesh
{
private:
	int m_numVertices; ///< ���_��
	int m_numFaces; ///< �ʐ�
	int m_numEdges; ///< �G�b�W��
	D3DXVECTOR3 m_vertices[COLLISION_MESH_MAX_VERTICES]; ///< ���_�z��
	Edge m_edges[COLLISION_MESH_MAX_EDGES]; ///< �G�b�W�z��
	Face m_faces[COLLISION_MESH_MAX_FACES]; ///< �ʔz��

	bool LoadCollisionMesh(const char* fileName,float*& vertices,int* numVertices, int*& indices, int* numIndices);
public:
	/// ������
	void Init()
	{
		m_numVertices = 0;
		m_numFaces = 0;
		m_numEdges = 0;
	}

	void Uninit();


	// �ʃ��b�V�����쐬����֐�
	bool CreateCollisionMesh(const char* fileName,const D3DXVECTOR3& scale = D3DXVECTOR3(1.0f,1.0f,1.0f));

	// ����ɓʃ��b�V���𓊉e���A�ŏ��l�ƍő�l��Ԃ��֐�
	void GetProjection(float& pmin, float& pmax,const D3DXVECTOR3& axis);
};




