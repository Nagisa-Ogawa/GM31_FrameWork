#pragma once
#include <d3dx9math.h>

#define COLLISION_MESH_MAX_VERTICES	(34)	// 当たり判定用メッシュデータの最大頂点数
#define COLLISION_MESH_MAX_EDGES	(96)	// 当たり判定用メッシュデータの最大辺数
#define COLLISION_MESH_MAX_FACES	(64)	// 当たり判定用メッシュデータの最大面数

/// エッジの種類
enum EdgeType {
	EDGE_TYPE_CONVEX,	// 凸エッジ
	EDGE_TYPE_CONCAVE,	// 凹エッジ
	EDGE_TYPE_FLAT,		// 平坦エッジ
};

/// エッジ
struct Edge {
	int type;	// エッジの種類
	int vertexId[2];	// 端点の頂点インデックス
	int faceId[2];	// 共有する面インデックス
};

/// ３角形面
struct Face {
	int vertexId[3]; ///< 頂点インデックス
	int edgeId[3]; ///< エッジインデックス
	D3DXVECTOR3 normal; ///< 面法線ベクトル
};


//-----------------------------------------------------
// 当たり判定に使用する凸メッシュデータを表すクラス
//------------------------------------------------------
class CollisionMesh
{
private:
	int m_numVertices; ///< 頂点数
	int m_numFaces; ///< 面数
	int m_numEdges; ///< エッジ数
	D3DXVECTOR3 m_vertices[COLLISION_MESH_MAX_VERTICES]; ///< 頂点配列
	Edge m_edges[COLLISION_MESH_MAX_EDGES]; ///< エッジ配列
	Face m_faces[COLLISION_MESH_MAX_FACES]; ///< 面配列

	bool LoadCollisionMesh(const char* fileName,float*& vertices,int* numVertices, int*& indices, int* numIndices);
public:
	/// 初期化
	void Init()
	{
		m_numVertices = 0;
		m_numFaces = 0;
		m_numEdges = 0;
	}

	void Uninit();


	// 凸メッシュを作成する関数
	bool CreateCollisionMesh(const char* fileName,const D3DXVECTOR3& scale = D3DXVECTOR3(1.0f,1.0f,1.0f));

	// 軸上に凸メッシュを投影し、最小値と最大値を返す関数
	void GetProjection(float& pmin, float& pmax,const D3DXVECTOR3& axis);
};




