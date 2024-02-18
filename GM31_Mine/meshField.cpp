
#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "transform.h"
#include "meshField.h"
#include "dispInspector.h"

#include "manager.h"
#include "scene.h"
#include "player.h"

float fieldHeight[21][21] =
{
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,2.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,4.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,2.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,4.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,5.0f,5.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,3.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,3.0f,1.0f,2.0f,4.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,3.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,2.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,3.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,2.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,4.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
	{0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,0.0f,1.0f,1.0f,1.0f,1.0f,2.0f,2.0f,3.0f,2.0f,3.0f,4.0f},
};

void MeshField::Init(std::string filePath)
{
	m_texturePath = filePath;
	{
		// 頂点情報をセット
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			for (int z = 0; z < STAGE_HEIGHT; z++)
			{
				// 二次元配列のパラメータからステージの高低差を決める
				m_vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, fieldHeight[z][x], (z - 10) * -5.0f);
				m_vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		// 法線ベクトルの算出
		for (int x = 1; x < (STAGE_WIDTH - 1); x++) {
			for (int z = 1; z < (STAGE_HEIGHT - 1); z++) {
				D3DXVECTOR3 vx, vz, vn;
				vx = m_vertex[x + 1][z].Position - m_vertex[x - 1][z].Position;
				vz = m_vertex[x][z - 1].Position - m_vertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);
				D3DXVec3Normalize(&vn, &vn);
				m_vertex[x][z].Normal = vn;
			}
		}

		// 頂点バッファ作成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * STAGE_WIDTH * STAGE_WIDTH;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);
	}

	{
		// インデックスバッファ生成
		unsigned int index[(22 * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++) {
			for (int z = 0; z < 21; z++) {
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}

			if (x == 19)
				break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_indexBuffer);

		// テクスチャ読み込み
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			filePath.c_str(),
			NULL,
			NULL,
			&m_texture,
			NULL);
		assert(m_texture);
	}
}

void MeshField::Load()
{
	// IDからターゲットを探す
	if (m_targetID != -1) {
		m_target = Manager::GetInstance()->GetScene()->GetGameObjectWithID(m_targetID);
	}
	{
		// 頂点情報をセット
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			for (int z = 0; z < STAGE_HEIGHT; z++)
			{
				// 二次元配列のパラメータからステージの高低差を決める
				m_vertex[x][z].Position = D3DXVECTOR3((x - 10) * 5.0f, fieldHeight[z][x], (z - 10) * -5.0f);
				m_vertex[x][z].Normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
				m_vertex[x][z].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
				m_vertex[x][z].TexCoord = D3DXVECTOR2(x * 0.5f, z * 0.5f);
			}
		}

		// 法線ベクトルの算出
		for (int x = 1; x < (STAGE_WIDTH - 1); x++) {
			for (int z = 1; z < (STAGE_HEIGHT - 1); z++) {
				D3DXVECTOR3 vx, vz, vn;
				vx = m_vertex[x + 1][z].Position - m_vertex[x - 1][z].Position;
				vz = m_vertex[x][z - 1].Position - m_vertex[x][z + 1].Position;

				D3DXVec3Cross(&vn, &vz, &vx);
				D3DXVec3Normalize(&vn, &vn);
				m_vertex[x][z].Normal = vn;
			}
		}


		// 頂点バッファ作成
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * STAGE_WIDTH * STAGE_WIDTH;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = m_vertex;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_vertexBuffer);

	}

	{
		// インデックスバッファ生成
		unsigned int index[(22 * 2) * 20 - 2];

		int i = 0;
		for (int x = 0; x < 20; x++)
		{
			for (int z = 0; z < 21; z++)
			{
				index[i] = x * 21 + z;
				i++;

				index[i] = (x + 1) * 21 + z;
				i++;
			}

			if (x == 19)
				break;

			index[i] = (x + 1) * 21 + 20;
			i++;

			index[i] = (x + 1) * 21;
			i++;
		}

		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * ((22 * 2) * 20 - 2);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = index;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_indexBuffer);

		// テクスチャ読み込み
		D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
			m_texturePath.c_str(),
			NULL,
			NULL,
			&m_texture,
			NULL);
		assert(m_texture);
	}
}


void MeshField::Uninit()
{

	m_vertexBuffer->Release();
	m_indexBuffer->Release();
	m_texture->Release();

}

void MeshField::Update()
{
	if (m_target) {
		auto targetTransform = m_target->GetTransform();
		targetTransform->m_localPosition.y = GetHeight(targetTransform->m_localPosition);
	}
}

void MeshField::Draw()
{
	//// 入力レイアウト設定
	//Renderer::GetDeviceContext()->IASetInputLayout(m_vertexLayout);

	//// シェーダ設定
	//Renderer::GetDeviceContext()->VSSetShader(m_vertexShader, NULL, 0);
	//Renderer::GetDeviceContext()->PSSetShader(m_pixelShader, NULL, 0);


	//// マトリクス設定
	//D3DXMATRIX world, scale, rot, trans;
	//D3DXMatrixScaling(&scale, m_transform->m_localScale.x, m_transform->m_localScale.y, m_transform->m_localScale.z);
	//D3DXMatrixRotationYawPitchRoll(&rot, m_transform->m_localRotation.y, m_transform->m_localRotation.x, m_transform->m_localRotation.z);
	//D3DXMatrixTranslation(&trans, m_transform->m_localPosition.x, m_transform->m_localPosition.y, m_transform->m_localPosition.z);
	//world = scale * rot * trans;
	//Renderer::SetWorldMatrix(&world);


	// 頂点バッファ設定
	UINT stride = sizeof( VERTEX_3D );
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );

	// インデックスバッファ設定
	Renderer::GetDeviceContext()->IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

	// マテリアル設定
	MATERIAL material;
	ZeroMemory( &material, sizeof(material) );
	material.Diffuse = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f );
	material.TextureEnable = true;
	Renderer::SetMaterial( material );

	// テクスチャ設定
	Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_texture);

	// プリミティブトポロジ設定
	Renderer::GetDeviceContext()->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// ポリゴン描画
	Renderer::GetDeviceContext()->DrawIndexed((22 * 2) * 20 - 2, 0, 0);

}

void MeshField::DispInspector()
{
	DispComponent(this);
}


/// <summary>
/// テクスチャを変更する関数
/// </summary>
/// <param name="texturePath"></param>
void MeshField::ChangeTexture(std::string texturePath)
{
	// 同じファイルなら読み込まない
	if (m_texturePath == texturePath) {
		return;
	}
	m_texturePath = texturePath;
	if (m_texture) {
		m_texture->Release();
	}
	// テクスチャ読み込み
	D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
		texturePath.c_str(),
		NULL,
		NULL,
		&m_texture,
		NULL);
	assert(m_texture);

}

float MeshField::GetHeight(D3DXVECTOR3 pos)
{
	int x, z;

	// ブロック番号算出
	x = pos.x / 5.0f + 10.0f;
	z = pos.z / -5.0f + 10.0f;

	D3DXVECTOR3 pos0, pos1, pos2, pos3;

	pos0 = m_vertex[x][z].Position;
	pos1 = m_vertex[x + 1][z].Position;
	pos2 = m_vertex[x][z + 1].Position;
	pos3 = m_vertex[x + 1][z + 1].Position;

	D3DXVECTOR3 v12, v1p, c;
	v12 = pos2 - pos1;
	v1p = pos - pos1;

	D3DXVec3Cross(&c, &v12, &v1p);
	
	float py;
	D3DXVECTOR3 n;
	if (c.y > 0.0f) {
		// 左上ポリゴン
		D3DXVECTOR3 v10;
		v10 = pos0 - pos1;
		D3DXVec3Cross(&n, &v10, &v12);
	}
	else {
		// 右下ポリゴン
		D3DXVECTOR3 v13;
		v13 = pos3 - pos1;
		D3DXVec3Cross(&n, &v12, &v13);
	}

	// 高さ取得
	py = -((pos.x - pos1.x) * n.x
		+ (pos.z - pos1.z) * n.z) / n.y + pos1.y;

	return py;
}

