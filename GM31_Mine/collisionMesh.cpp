
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <shlwapi.h>
#include "collisionMesh.h"

#define EPSILON (1e-5f)

void CollisionMesh::Uninit()
{
}

/// <summary>
/// objファイルからメッシュデータを取得する関数
/// 取得するデータは頂点座標とインデックス及びその個数
/// </summary>
/// <param name="fileName">objファイル名</param>
/// <param name="vertices">頂点座標を格納する配列</param>
/// <param name="numVertices">頂点数</param>
/// <param name="indices">インデックスを格納する配列</param>
/// <param name="numIndices">インデックス数</param>
/// <returns>成功したかどうか</returns>
bool CollisionMesh::LoadCollisionMesh(const char* fileName, float*& vertices, int* numVertices, int*& indices, int* numIndices)
{
	char dir[MAX_PATH];
	strcpy(dir, fileName);
	PathRemoveFileSpec(dir);

	char str[256];		// 読み込むモデルファイルのバッファ
	char* s;
	char c;


	FILE* file;
	if ((file = fopen(fileName, "rt")) == NULL) {
		return false;
	}

	//要素数カウント
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
			// ポリゴンを形成している頂点の数だけループ
			// 例)1/1/1 : 頂点/テクスチャ/法線
			do
			{
				fscanf(file, "%s", str);
				(*numIndices)++;
				c = fgetc(file);
			} while (c != '\n' && c != '\r');
		}
	}


	//カウントした要素数分メモリ確保
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

		// 頂点座標情報を配列に格納
		if (strcmp(str, "v") == 0)
		{
			//頂点座標
			fscanf(file, "%f", &vertices[vc]);
			vc++;
			fscanf(file, "%f", &vertices[vc]);
			vc++;
			fscanf(file, "%f", &vertices[vc]);
			vc++;
		}
		else if (strcmp(str, "f") == 0)
		{
			//面
			do
			{
				fscanf(file, "%s", str);

				// インデックスを取得
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
/// ファイルからメッシュデータを作成する関数
/// </summary>
/// <param name="fileName">メッシュデータを読み込むファイル名</param>
/// <param name="scale">当たり判定用メッシュのスケール</param>
/// <returns>作成できたかどうか</returns>
bool CollisionMesh::CreateCollisionMesh(const char* fileName, const D3DXVECTOR3& scale)
{
	int numVertices = 0;
	int numIndices = 0;
	float* vertices = nullptr;
	int* indices = nullptr;

	// ファイルからメッシュデータを取得(頂点座標とインデックス)
    if (!LoadCollisionMesh(fileName, vertices, &numVertices, indices, &numIndices)) {
        return false;
    }

	// 頂点数やインデックス数が最大値以上なら終了
	if (numVertices > COLLISION_MESH_MAX_VERTICES || numIndices > COLLISION_MESH_MAX_FACES * 3) {
		return false;
	}

	// 頂点バッファ作成
	for (int i = 0; i < numVertices; i++) {
		// 頂点座標を格納
		m_vertices[i].x = vertices[i * 3 + 0];
		m_vertices[i].y = vertices[i * 3 + 1];
		m_vertices[i].z = vertices[i * 3 + 2];
		// スケールを反映
		m_vertices[i] = D3DXVECTOR3(m_vertices[i].x * scale.x, m_vertices[i].y * scale.y, m_vertices[i].z * scale.z);
	}
	m_numVertices = numVertices;

	// 面バッファ作成
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
		// 三角ポリゴンの面積を調べて縮退ポリゴンなら面として扱わない
		float areaSqr = D3DXVec3LengthSq(&normal);
		// 縮退ポリゴンじゃないなら
		if (areaSqr > EPSILON * EPSILON) {
			m_faces[nf].vertexId[0] = indices[i * 3 + 0];
			m_faces[nf].vertexId[1] = indices[i * 3 + 1];
			m_faces[nf].vertexId[2] = indices[i * 3 + 2];
			m_faces[nf].normal = normal / sqrtf(areaSqr);
			nf++;
		}
	}
	m_numFaces = nf;

	// エッジバッファ作成
	// 
	// 同じ頂点の組み合わせの辺を保存しておくテーブル
	int edgeIdTable[COLLISION_MESH_MAX_VERTICES * COLLISION_MESH_MAX_VERTICES / 2];
	// -1で初期化
	memset(edgeIdTable, -1, sizeof(edgeIdTable));

	// 辺のカウント
	int ec = 0;
	for (int i = 0; i < m_numFaces; i++) {
		Face& faceA = m_faces[i];
		for (int e = 0; e < 3; e++) {
			// IDが小さい方を0番に、大きい方を1番に格納
			int vertexId0 = min(faceA.vertexId[e % 3], faceA.vertexId[(e + 1) % 3]);
			int vertexId1 = max(faceA.vertexId[e % 3], faceA.vertexId[(e + 1) % 3]);
			// 2つの頂点IDから辺テーブルのIDを生成
			int tableId = vertexId1 * (vertexId1 - 1) / 2 + vertexId0;
			// 同じ頂点の組み合わせの辺がまだ登録されていないなら
			if (edgeIdTable[tableId] == -1) {
				// 初回時は登録のみ
				m_edges[ec].faceId[0] = i;
				m_edges[ec].faceId[1] = i;
				m_edges[ec].vertexId[0] = vertexId0;
				m_edges[ec].vertexId[1] = vertexId1;
				m_edges[ec].type = EdgeType::EDGE_TYPE_CONVEX; // 凸エッジで初期化
				faceA.edgeId[e] = ec;	// 面情報に辺情報を登録
				// テーブルに保存
				edgeIdTable[tableId] = ec;
				ec++;
				// 辺の最大値を超えたなら終了
				if (ec > COLLISION_MESH_MAX_EDGES) {
					return false;
				}
			}
			// すでに同じ頂点の組み合わせの辺が登録されていたなら
			else {
				// 共有面を見つけたので、エッジの角度を判定
				// 前回保存したテーブルから同じ頂点の組み合わせの辺を持つ面を取得
				Edge& edge = m_edges[edgeIdTable[tableId]];
				Face& faceB = m_faces[edge.faceId[0]];


				// エッジに含まれないＡ面の頂点がB面の表か裏かで判断
				D3DXVECTOR3 s = m_vertices[faceA.vertexId[(e + 2) % 3]];
				D3DXVECTOR3 q = m_vertices[faceB.vertexId[0]];
				D3DXVECTOR3 vec = s - q;

				// 内積の結果がマイナスなら凸エッジ、プラスなら凹エッジ、0なら平坦エッジになる
				// 計算誤差を考慮してEPSILON定数未満なら平坦エッジにする
				float d = D3DXVec3Dot(&vec, &faceB.normal);

				if (d < -EPSILON) {
					// 凸エッジ
					edge.type = EdgeType::EDGE_TYPE_CONVEX;
				}
				else if (d > EPSILON) {
					// 凹エッジは存在しないので
					// 正常ならここには入らない
					edge.type = EdgeType::EDGE_TYPE_CONCAVE;
				}
				else {
					// 平坦エッジ
					edge.type = EdgeType::EDGE_TYPE_FLAT;
				}

				edge.faceId[1] = i;
				faceA.edgeId[e] = edgeIdTable[tableId];		// 面情報に辺情報を登録
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
