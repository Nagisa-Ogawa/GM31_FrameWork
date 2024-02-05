
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#include "main.h"
#include "renderer.h"
#include "gameObject.h"
#include "model.h"
#include "polygonCollision.h"
#include "dispInspector.h"


void Model::Init(const char* FileName, bool hasColl)
{
	if (hasColl) {
		// ���̃��f���p�����蔻����쐬
		m_PolyColl = m_gameObject->AddComponent<PolygonCollision>();
	}
	// �I�u�W�F�N�g��.obj�t�@�C������ǂݍ���
	MODEL model;
	LoadObj(FileName, &model);

	m_fileName = FileName;

	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = new SUBSET[model.SubsetNum];
		m_SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				model.SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			if (m_SubsetArray[i].Material.Texture)
				m_SubsetArray[i].Material.Material.TextureEnable = true;
			else
				m_SubsetArray[i].Material.Material.TextureEnable = false;

		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}

// ���łɃ��f�����p�ӂ���Ă���ꍇ�̏���������
void Model::Init(MODEL* pModel)
{
	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * pModel->VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem =pModel->VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * pModel->IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = pModel->IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = new SUBSET[pModel->SubsetNum];
		m_SubsetNum = pModel->SubsetNum;

		for (unsigned int i = 0; i < pModel->SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = pModel->SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = pModel->SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = pModel->SubsetArray[i].Material.Material;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				pModel->SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			if (m_SubsetArray[i].Material.Texture)
				m_SubsetArray[i].Material.Material.TextureEnable = true;
			else
				m_SubsetArray[i].Material.Material.TextureEnable = false;

		}
	}
}

void Model::Load()
{
	m_PolyColl = m_gameObject->GetComponent<PolygonCollision>();
	MODEL model;
	LoadObj(m_fileName.c_str(), &model);


	// ���_�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(VERTEX_3D) * model.VertexNum;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.VertexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_VertexBuffer);
	}


	// �C���f�b�N�X�o�b�t�@����
	{
		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(unsigned int) * model.IndexNum;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = model.IndexArray;

		Renderer::GetDevice()->CreateBuffer(&bd, &sd, &m_IndexBuffer);
	}

	// �T�u�Z�b�g�ݒ�
	{
		m_SubsetArray = new SUBSET[model.SubsetNum];
		m_SubsetNum = model.SubsetNum;

		for (unsigned int i = 0; i < model.SubsetNum; i++)
		{
			m_SubsetArray[i].StartIndex = model.SubsetArray[i].StartIndex;
			m_SubsetArray[i].IndexNum = model.SubsetArray[i].IndexNum;

			m_SubsetArray[i].Material.Material = model.SubsetArray[i].Material.Material;

			m_SubsetArray[i].Material.Texture = NULL;

			D3DX11CreateShaderResourceViewFromFile(Renderer::GetDevice(),
				model.SubsetArray[i].Material.TextureName,
				NULL,
				NULL,
				&m_SubsetArray[i].Material.Texture,
				NULL);

			if (m_SubsetArray[i].Material.Texture)
				m_SubsetArray[i].Material.Material.TextureEnable = true;
			else
				m_SubsetArray[i].Material.Material.TextureEnable = false;

		}
	}

	delete[] model.VertexArray;
	delete[] model.IndexArray;
	delete[] model.SubsetArray;

}

void Model::Uninit()
{
	m_VertexBuffer->Release();
	m_IndexBuffer->Release();

	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		if (m_SubsetArray[i].Material.Texture)
			m_SubsetArray[i].Material.Texture->Release();
	}

	delete[] m_SubsetArray;
}

void Model::Update()
{
}

void Model::Draw()
{

	// ���_�o�b�t�@�ݒ�
	UINT stride = sizeof(VERTEX_3D);
	UINT offset = 0;
	Renderer::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// �C���f�b�N�X�o�b�t�@�ݒ�
	Renderer::GetDeviceContext()->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// �v���~�e�B�u�g�|���W�ݒ�
	Renderer::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (unsigned int i = 0; i < m_SubsetNum; i++)
	{
		// �}�e���A���ݒ�
		Renderer::SetMaterial(m_SubsetArray[i].Material.Material);

		// �e�N�X�`���ݒ�
		if (m_SubsetArray[i].Material.Texture)
			Renderer::GetDeviceContext()->PSSetShaderResources(0, 1, &m_SubsetArray[i].Material.Texture);

		// �|���S���`��
		Renderer::GetDeviceContext()->DrawIndexed(m_SubsetArray[i].IndexNum, m_SubsetArray[i].StartIndex, 0);
	}

}

void Model::DispInspector()
{
	DispComponent(this);
}


//���f���Ǎ�////////////////////////////////////////////
void Model::LoadObj( const char *FileName, MODEL *Model )
{

	char dir[MAX_PATH];
	strcpy (dir, FileName );
	PathRemoveFileSpec(dir);

	POLYGON_POSITION* polyPosArray;		// �|���S�����Ƃ̒��_�z��

	D3DXVECTOR3* positionArray;	// ���_�z��
	D3DXVECTOR3* normalArray;	// �@���z��
	D3DXVECTOR2* texcoordArray;	// �e�N�X�`�����W�z��

	unsigned int	positionNum = 0;	// ���_�̌�
	unsigned int	normalNum = 0;		// �@���̌�
	unsigned int	texcoordNum = 0;	// �e�N�X�`�����W�̌�
	unsigned int	vertexNum = 0;		// ���_���̌�
	unsigned int	indexNum = 0;		// �C���f�b�N�X�̌�
	unsigned int	in = 0;				// �|���S���̒��_��
	unsigned int	subsetNum = 0;		// �}�e���A���t�@�C���̌�
	unsigned int	polygonNum = 0;		// �|���S���̐�

	MODEL_MATERIAL	*materialArray = NULL;	// �}�e���A���̔z��
	unsigned int	materialNum = 0;		// �}�e���A���̌�

	char str[256];		// �ǂݍ��ރ��f���t�@�C���̃o�b�t�@
	char *s;
	char c;


	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);



	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "v" ) == 0 )
		{
			positionNum++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			normalNum++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			texcoordNum++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			subsetNum++;
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			in = 0;

			// �|���S�����`�����Ă��钸�_�̐��������[�v
			// ��)1/1/1 : ���_/�e�N�X�`��/�@��
			do
			{
				fscanf( file, "%s", str );
				vertexNum++;
				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c!= '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
				in = 6;
			indexNum += in;
			polygonNum++;
		}
	}


	//�J�E���g�����v�f�����������m��
	positionArray = new D3DXVECTOR3[ positionNum ];
	normalArray = new D3DXVECTOR3[ normalNum ];
	texcoordArray = new D3DXVECTOR2[ texcoordNum ];
	polyPosArray = new POLYGON_POSITION[polygonNum];

	Model->VertexArray = new VERTEX_3D[ vertexNum ];
	Model->VertexNum = vertexNum;

	Model->IndexArray = new unsigned int[ indexNum ];
	Model->IndexNum = indexNum;

	Model->SubsetArray = new SUBSET[ subsetNum ];
	Model->SubsetNum = subsetNum;

	if (m_PolyColl != nullptr) {
		// �|���S���p�����蔻��R���|�[�l���g�փ|���S���z����Z�b�g
		m_PolyColl->SetPolygonArray(polyPosArray);
	}


	//�v�f�Ǎ�
	D3DXVECTOR3 *position = positionArray;
	D3DXVECTOR3 *normal = normalArray;
	D3DXVECTOR2 *texcoord = texcoordArray;

	unsigned int vc = 0;
	unsigned int ic = 0;
	unsigned int sc = 0;
	unsigned int pc = 0;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;

		if( strcmp( str, "mtllib" ) == 0 )
		{
			//�}�e���A���t�@�C��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			LoadMaterial( path, &materialArray, &materialNum );
		}
		else if( strcmp( str, "o" ) == 0 )
		{
			//�I�u�W�F�N�g��
			fscanf( file, "%s", str );
		}
		else if( strcmp( str, "v" ) == 0 )
		{
			//���_���W
			fscanf( file, "%f", &position->x );
			fscanf( file, "%f", &position->y );
			fscanf( file, "%f", &position->z );
			position++;
		}
		else if( strcmp( str, "vn" ) == 0 )
		{
			//�@��
			fscanf( file, "%f", &normal->x );
			fscanf( file, "%f", &normal->y );
			fscanf( file, "%f", &normal->z );
			normal++;
		}
		else if( strcmp( str, "vt" ) == 0 )
		{
			//�e�N�X�`�����W
			fscanf( file, "%f", &texcoord->x );
			fscanf( file, "%f", &texcoord->y );
			texcoord->y = 1.0f - texcoord->y;
			texcoord++;
		}
		else if( strcmp( str, "usemtl" ) == 0 )
		{
			//�}�e���A��
			fscanf( file, "%s", str );

			if( sc != 0 )
				Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;

			Model->SubsetArray[ sc ].StartIndex = ic;


			for( unsigned int i = 0; i < materialNum; i++ )
			{
				if( strcmp( str, materialArray[i].Name ) == 0 )
				{
					Model->SubsetArray[ sc ].Material.Material = materialArray[i].Material;
					strcpy( Model->SubsetArray[ sc ].Material.TextureName, materialArray[i].TextureName );
					strcpy( Model->SubsetArray[ sc ].Material.Name, materialArray[i].Name );

					break;
				}
			}

			sc++;
			
		}
		else if( strcmp( str, "f" ) == 0 )
		{
			//��
			in = 0;

			do
			{
				fscanf( file, "%s", str );

				// �ʒu�����擾
				s = strtok( str, "/" );	
				Model->VertexArray[vc].Position = positionArray[ atoi( s ) - 1 ];
				polyPosArray[pc].position[in] = positionArray[atoi(s) - 1];
				if( s[ strlen( s ) + 1 ] != '/' )
				{
					//�e�N�X�`�����W�����݂���Ȃ�e�N�X�`�����W���擾
					s = strtok( NULL, "/" );
					Model->VertexArray[vc].TexCoord = texcoordArray[ atoi( s ) - 1 ];
				}
				// �@�������擾
				s = strtok( NULL, "/" );	
				Model->VertexArray[vc].Normal = normalArray[ atoi( s ) - 1 ];

				Model->VertexArray[vc].Diffuse = D3DXVECTOR4( 1.0f, 1.0f, 1.0f, 1.0f );

				Model->IndexArray[ic] = vc;
				ic++;
				vc++;

				in++;
				c = fgetc( file );
			}
			while( c != '\n' && c != '\r' );

			//�l�p�͎O�p�ɕ���
			if( in == 4 )
			{
				Model->IndexArray[ic] = vc - 4;
				ic++;
				Model->IndexArray[ic] = vc - 2;
				ic++;
			}
			pc++;
		}
	}
	if (m_PolyColl != nullptr) {
		m_PolyColl->SetArrayCount(pc);
	}

	if( sc != 0 )
		Model->SubsetArray[ sc - 1 ].IndexNum = ic - Model->SubsetArray[ sc - 1 ].StartIndex;


	fclose( file );


	delete[] positionArray;
	delete[] normalArray;
	delete[] texcoordArray;
	delete[] materialArray;
}


//�}�e���A���ǂݍ���///////////////////////////////////////////////////////////////////
void Model::LoadMaterial( const char *FileName, MODEL_MATERIAL **MaterialArray, unsigned int *MaterialNum )
{

	char dir[MAX_PATH];
	strcpy(dir, FileName);
	PathRemoveFileSpec(dir);



	char str[256];

	FILE *file;
	file = fopen( FileName, "rt" );
	assert(file);

	MODEL_MATERIAL *materialArray;
	unsigned int materialNum = 0;

	//�v�f���J�E���g
	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			materialNum++;
		}
	}


	//�������m��
	materialArray = new MODEL_MATERIAL[ materialNum ];


	//�v�f�Ǎ�
	int mc = -1;

	fseek( file, 0, SEEK_SET );

	while( true )
	{
		fscanf( file, "%s", str );

		if( feof( file ) != 0 )
			break;


		if( strcmp( str, "newmtl" ) == 0 )
		{
			//�}�e���A����
			mc++;
			fscanf( file, "%s", materialArray[ mc ].Name );
			strcpy( materialArray[ mc ].TextureName, "" );

			materialArray[mc].Material.Emission.r = 0.0f;
			materialArray[mc].Material.Emission.g = 0.0f;
			materialArray[mc].Material.Emission.b = 0.0f;
			materialArray[mc].Material.Emission.a = 0.0f;
		}
		else if( strcmp( str, "Ka" ) == 0 )
		{
			//�A���r�G���g
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Ambient.b );
			materialArray[ mc ].Material.Ambient.a = 1.0f;
		}
		else if( strcmp( str, "Kd" ) == 0 )
		{
			//�f�B�t���[�Y
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.b );
			materialArray[ mc ].Material.Diffuse.a = 1.0f;
		}
		else if( strcmp( str, "Ks" ) == 0 )
		{
			//�X�y�L����
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.r );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.g );
			fscanf( file, "%f", &materialArray[ mc ].Material.Specular.b );
			materialArray[ mc ].Material.Specular.a = 1.0f;
		}
		else if( strcmp( str, "Ns" ) == 0 )
		{
			//�X�y�L�������x
			fscanf( file, "%f", &materialArray[ mc ].Material.Shininess );
		}
		else if( strcmp( str, "d" ) == 0 )
		{
			//�A���t�@
			fscanf( file, "%f", &materialArray[ mc ].Material.Diffuse.a );
		}
		else if( strcmp( str, "map_Kd" ) == 0 )
		{
			//�e�N�X�`��
			fscanf( file, "%s", str );

			char path[256];
			strcpy( path, dir );
			strcat( path, "\\" );
			strcat( path, str );

			strcat( materialArray[ mc ].TextureName, path );
		}
	}

	fclose(file);

	*MaterialArray = materialArray;
	*MaterialNum = materialNum;
}

