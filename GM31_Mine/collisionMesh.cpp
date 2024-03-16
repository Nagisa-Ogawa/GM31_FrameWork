
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <shlwapi.h>
#include "collisionMesh.h"

#define EPSILON (1e-5f)

void CollisionMesh::Uninit()
{
}

/// <summary>
/// obj�t�@�C�����烁�b�V���f�[�^���擾����֐�
/// �擾����f�[�^�͒��_���W�ƃC���f�b�N�X�y�т��̌�
/// </summary>
/// <param name="fileName">obj�t�@�C����</param>
/// <param name="vertices">���_���W���i�[����z��</param>
/// <param name="numVertices">���_��</param>
/// <param name="indices">�C���f�b�N�X���i�[����z��</param>
/// <param name="numIndices">�C���f�b�N�X��</param>
/// <returns>�����������ǂ���</returns>
bool CollisionMesh::LoadCollisionMesh(const char* fileName, float*& vertices, int* numVertices, int*& indices, int* numIndices)
{
	char dir[MAX_PATH];
	strcpy(dir, fileName);
	PathRemoveFileSpec(dir);

	char str[256];		// �ǂݍ��ރ��f���t�@�C���̃o�b�t�@
	char* s;
	char c;


	FILE* file;
	if ((file = fopen(fileName, "rt")) == NULL) {
		return false;
	}

	//�v�f���J�E���g
	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		if (strcmp(str, "v") == 0)
		{
			(*numVertices)++;
		}
		else if (strcmp(str, "f") == 0)
		{
			// �|���S�����`�����Ă��钸�_�̐��������[�v
			// ��)1/1/1 : ���_/�e�N�X�`��/�@��
			do
			{
				fscanf(file, "%s", str);
				(*numIndices)++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');
		}
	}


	//�J�E���g�����v�f�����������m��
	vertices = new float[(*numVertices) * 3];
	indices = new int[*numIndices];

	unsigned int vc = 0;
	unsigned int ic = 0;

	fseek(file, 0, SEEK_SET);

	while (true)
	{
		fscanf(file, "%s", str);

		if (feof(file) != 0)
			break;

		// ���_���W����z��Ɋi�[
		if (strcmp(str, "v") == 0)
		{
			//���_���W
			fscanf(file, "%f", &vertices[vc]);
			vc++;
			fscanf(file, "%f", &vertices[vc]);
			vc++;
			fscanf(file, "%f", &vertices[vc]);
			vc++;
		}
		else if (strcmp(str, "f") == 0)
		{
			//��
			do
			{
				fscanf(file, "%s", str);

				// �C���f�b�N�X���擾
				s = strtok(str, "/");
				indices[ic] = atoi(s) - 1;
				ic++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');
		}
	}

	fclose(file);

	return true;
}


/// <summary>
/// �t�@�C�����烁�b�V���f�[�^���쐬����֐�
/// </summary>
/// <param name="fileName">���b�V���f�[�^��ǂݍ��ރt�@�C����</param>
/// <param name="scale">�����蔻��p���b�V���̃X�P�[��</param>
/// <returns>�쐬�ł������ǂ���</returns>
bool CollisionMesh::CreateCollisionMesh(const char* fileName, const D3DXVECTOR3& scale)
{
	int numVertices = 0;
	int numIndices = 0;
	float* vertices = nullptr;
	int* indices = nullptr;

	// �t�@�C�����烁�b�V���f�[�^���擾(���_���W�ƃC���f�b�N�X)
    if (!LoadCollisionMesh(fileName, vertices, &numVertices, indices, &numIndices)) {
        return false;
    }

	// ���_����C���f�b�N�X�����ő�l�ȏ�Ȃ�I��
	if (numVertices > COLLISION_MESH_MAX_VERTICES || numIndices > COLLISION_MESH_MAX_FACES * 3) {
		return false;
	}

	// ���_�o�b�t�@�쐬
	for (int i = 0; i < numVertices; i++) {
		// ���_���W���i�[
		m_vertices[i].x = vertices[i * 3 + 0];
		m_vertices[i].y = vertices[i * 3 + 1];
		m_vertices[i].z = vertices[i * 3 + 2];
		// �X�P�[���𔽉f
		m_vertices[i] = D3DXVECTOR3(m_vertices[i].x * scale.x, m_vertices[i].y * scale.y, m_vertices[i].z * scale.z);
	}
	m_numVertices = numVertices;

	// �ʃo�b�t�@�쐬
	int nf = 0;
	for (int i = 0; i < numIndices / 3; i++) {
		D3DXVECTOR3 p[3] = {
			m_vertices[indices[i * 3 + 0]],
			m_vertices[indices[i * 3 + 1]],
			m_vertices[indices[i * 3 + 2]]
		};
		D3DXVECTOR3 vec1 = p[1] - p[0];
		D3DXVECTOR3 vec2 = p[2] - p[0];
		D3DXVECTOR3 normal;
		D3DXVec3Cross(&normal, &vec1, &vec2);
		// �O�p�|���S���̖ʐς𒲂ׂďk�ރ|���S���Ȃ�ʂƂ��Ĉ���Ȃ�
		float areaSqr = D3DXVec3LengthSq(&normal);
		// �k�ރ|���S������Ȃ��Ȃ�
		if (areaSqr > EPSILON * EPSILON) {
			m_faces[nf].vertexId[0] = indices[i * 3 + 0];
			m_faces[nf].vertexId[1] = indices[i * 3 + 1];
			m_faces[nf].vertexId[2] = indices[i * 3 + 2];
			m_faces[nf].normal = normal / sqrtf(areaSqr);
			nf++;
		}
	}
	m_numFaces = nf;

	// �G�b�W�o�b�t�@�쐬
	// 
	// �������_�̑g�ݍ��킹�̕ӂ�ۑ����Ă����e�[�u��
	int edgeIdTable[COLLISION_MESH_MAX_VERTICES * COLLISION_MESH_MAX_VERTICES / 2];
	// -1�ŏ�����
	memset(edgeIdTable, -1, sizeof(edgeIdTable));

	// �ӂ̃J�E���g
	int ec = 0;
	for (int i = 0; i < m_numFaces; i++) {
		Face& faceA = m_faces[i];
		for (int e = 0; e < 3; e++) {
			// ID������������0�ԂɁA�傫������1�ԂɊi�[
			int vertexId0 = min(faceA.vertexId[e % 3], faceA.vertexId[(e + 1) % 3]);
			int vertexId1 = max(faceA.vertexId[e % 3], faceA.vertexId[(e + 1) % 3]);
			// 2�̒��_ID����Ӄe�[�u����ID�𐶐�
			int tableId = vertexId1 * (vertexId1 - 1) / 2 + vertexId0;
			// �������_�̑g�ݍ��킹�̕ӂ��܂��o�^����Ă��Ȃ��Ȃ�
			if (edgeIdTable[tableId] == -1) {
				// ���񎞂͓o�^�̂�
				m_edges[ec].faceId[0] = i;
				m_edges[ec].faceId[1] = i;
				m_edges[ec].vertexId[0] = vertexId0;
				m_edges[ec].vertexId[1] = vertexId1;
				m_edges[ec].type = EdgeType::EDGE_TYPE_CONVEX; // �ʃG�b�W�ŏ�����
				faceA.edgeId[e] = ec;	// �ʏ��ɕӏ���o�^
				// �e�[�u���ɕۑ�
				edgeIdTable[tableId] = ec;
				ec++;
				// �ӂ̍ő�l�𒴂����Ȃ�I��
				if (ec > COLLISION_MESH_MAX_EDGES) {
					return false;
				}
			}
			// ���łɓ������_�̑g�ݍ��킹�̕ӂ��o�^����Ă����Ȃ�
			else {
				// ���L�ʂ��������̂ŁA�G�b�W�̊p�x�𔻒�
				// �O��ۑ������e�[�u�����瓯�����_�̑g�ݍ��킹�̕ӂ����ʂ��擾
				Edge& edge = m_edges[edgeIdTable[tableId]];
				Face& faceB = m_faces[edge.faceId[0]];


				// �G�b�W�Ɋ܂܂�Ȃ��`�ʂ̒��_��B�ʂ̕\�������Ŕ��f
				D3DXVECTOR3 s = m_vertices[faceA.vertexId[(e + 2) % 3]];
				D3DXVECTOR3 q = m_vertices[faceB.vertexId[0]];
				D3DXVECTOR3 vec = s - q;

				// ���ς̌��ʂ��}�C�i�X�Ȃ�ʃG�b�W�A�v���X�Ȃ牚�G�b�W�A0�Ȃ畽�R�G�b�W�ɂȂ�
				// �v�Z�덷���l������EPSILON�萔�����Ȃ畽�R�G�b�W�ɂ���
				float d = D3DXVec3Dot(&vec, &faceB.normal);

				if (d < -EPSILON) {
					// �ʃG�b�W
					edge.type = EdgeType::EDGE_TYPE_CONVEX;
				}
				else if (d > EPSILON) {
					// ���G�b�W�͑��݂��Ȃ��̂�
					// ����Ȃ炱���ɂ͓���Ȃ�
					edge.type = EdgeType::EDGE_TYPE_CONCAVE;
				}
				else {
					// ���R�G�b�W
					edge.type = EdgeType::EDGE_TYPE_FLAT;
				}

				edge.faceId[1] = i;
				faceA.edgeId[e] = edgeIdTable[tableId];		// �ʏ��ɕӏ���o�^
			}
		}
	}
	m_numEdges = ec;

	delete vertices;
	delete indices;
    return true;
}

void CollisionMesh::GetProjection(float& pmin, float& pmax, const D3DXVECTOR3& axis)
{
}
