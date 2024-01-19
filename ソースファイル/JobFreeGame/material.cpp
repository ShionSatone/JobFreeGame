//==============================================================
//
//マテリアルの処理[texture.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "material.h"
#include "manager.h"
#include "renderer.h"

//**************************************************************
// 静的メンバ変数宣言
//**************************************************************
int CMaterial::m_nNumAll = -1;										//モデル番号
CMaterial::Material CMaterial::m_Material[MAX_TEX] = {};			//マテリアルの情報
const char *CMaterial::m_apFilename[MAX_TEX] = 
{
	/*"data\\MODEL\\enemy\\00_body.x",
	"data\\MODEL\\enemy\\01_head.x",
	"data\\MODEL\\enemy\\02_hair.x",
	"data\\MODEL\\enemy\\03_LU_arm.x",
	"data\\MODEL\\enemy\\04_LD_arm.x",
	"data\\MODEL\\enemy\\05_L_hand.x",
	"data\\MODEL\\enemy\\06_RU_arm.x",
	"data\\MODEL\\enemy\\07_RD_arm.x",
	"data\\MODEL\\enemy\\08_R_arm.x",
	"data\\MODEL\\enemy\\09_waist.x",
	"data\\MODEL\\enemy\\10_LU_leg.x",
	"data\\MODEL\\enemy\\11_LD_leg.x",
	"data\\MODEL\\enemy\\12_L_shoe.x",
	"data\\MODEL\\enemy\\13_RU_leg.x",
	"data\\MODEL\\enemy\\14_RD_leg.x",
	"data\\MODEL\\enemy\\15_R_shoe.x",*/
};

//==============================================================
//コンストラクタ
//==============================================================
CMaterial::CMaterial()
{
	//マテリアル初期化
	for (int nCntModel = 0; nCntModel < MAX_TEX; nCntModel++)
	{
		m_Material[nCntModel].pBuffMat = NULL;		//マテリアルへのポインタ
		m_Material[nCntModel].pMesh = NULL;			//メッシュへのポインタ
		m_Material[nCntModel].dwNumMat = NULL;		//マテリアルの数
		m_Material[nCntModel].pTexture = NULL;		//テクスチャ
	}
}

//==============================================================
//デストラクタ
//==============================================================
CMaterial::~CMaterial()
{

}

//==============================================================
//マテリアル読み込み処理
//==============================================================
HRESULT CMaterial::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	for (int nCntModel = 0; nCntModel < MAT_MAX; nCntModel++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(m_apFilename[nCntModel],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_Material[nCntModel].pBuffMat,
			NULL,
			&m_Material[nCntModel].dwNumMat,
			&m_Material[nCntModel].pMesh);

		D3DXMATERIAL *pMat;		//マテリアルへのポインタ

		m_Material[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[m_Material[nCntModel].dwNumMat];

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_Material[nCntModel].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_Material[nCntModel].dwNumMat; nCntMat++)
		{
			if (pMat[nCntMat].pTextureFilename != NULL)
			{//テクスチャが存在する
				D3DXCreateTextureFromFile(pDevice,
					pMat[nCntMat].pTextureFilename,
					&m_Material[nCntModel].pTexture[nCntMat]);
			}
			else
			{//テクスチャが存在しない
				m_Material[nCntModel].pTexture[nCntMat] = NULL;
			}
		}

		m_nNumAll++;		//モデルの数加算
	}

	return S_OK;
}

//==============================================================
//マテリアル破棄処理
//==============================================================
void CMaterial::Unload(void)
{
	for (int nCntModel = 0; nCntModel < MAX_TEX; nCntModel++)
	{
		//テクスチャの破棄
		for (int nCntMat = 0; nCntMat < (int)m_Material[nCntModel].dwNumMat; nCntMat++)
		{
			if (m_Material[nCntModel].pTexture != NULL)
			{
				delete m_Material[nCntModel].pTexture;
				m_Material[nCntModel].pTexture = NULL;
			}
		}

		//マテリアルの破棄
		if (m_Material[nCntModel].pBuffMat != NULL)
		{
			m_Material[nCntModel].pBuffMat->Release();
			m_Material[nCntModel].pBuffMat = NULL;
		}

		//メッシュの破棄
		if (m_Material[nCntModel].pMesh != NULL)
		{
			m_Material[nCntModel].pMesh->Release();
			m_Material[nCntModel].pMesh = NULL;
		}
	}
}

//==============================================================
//モデル番号指定処理
//==============================================================
int CMaterial::Regist(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	for (int nCntModel = 0; nCntModel < MAX_TEX; nCntModel++)
	{
		if (m_apFilename[nCntModel] == pFilename)
		{//ファイル名が同じとき

			return nCntModel;		//番号
		}
		else if (m_apFilename[nCntModel] != pFilename && nCntModel > m_nNumAll)
		{//ファイル名が違う

			m_apFilename[nCntModel] = pFilename;

			//Xファイルの読み込み
			D3DXLoadMeshFromX(m_apFilename[nCntModel],
				D3DXMESH_SYSTEMMEM,
				pDevice,
				NULL,
				&m_Material[nCntModel].pBuffMat,
				NULL,
				&m_Material[nCntModel].dwNumMat,
				&m_Material[nCntModel].pMesh);

			D3DXMATERIAL *pMat;		//マテリアルへのポインタ

			m_Material[nCntModel].pTexture = new LPDIRECT3DTEXTURE9[m_Material[nCntModel].dwNumMat];

			//マテリアル情報に対するポインタを取得
			pMat = (D3DXMATERIAL*)m_Material[nCntModel].pBuffMat->GetBufferPointer();

			for (int nCntMat = 0; nCntMat < (int)m_Material[nCntModel].dwNumMat; nCntMat++)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//テクスチャが存在する
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&m_Material[nCntModel].pTexture[nCntMat]);
				}
				else
				{//テクスチャが存在しない
					m_Material[nCntModel].pTexture[nCntMat] = NULL;
				}
			}

			m_nNumAll++;

			return m_nNumAll;		//マテリアルの番号を返す
		}
	}

	return m_nNumAll;		//マテリアルの番号を返す
}

//==============================================================
//テクスチャの情報取得
//==============================================================
LPDIRECT3DTEXTURE9 CMaterial::GetTexAddress(int nIdx, int nMat)
{
	return m_Material[nIdx].pTexture[nMat];		//指定のテクスチャを返す
}

//==============================================================
//マテリアルの情報取得
//==============================================================
LPD3DXBUFFER CMaterial::GetMatAddress(int nIdx)
{
	return m_Material[nIdx].pBuffMat;
}

//==============================================================
//マテリアルの数の情報取得
//==============================================================
DWORD CMaterial::GetNumMatAddress(int nIdx)
{
	return m_Material[nIdx].dwNumMat;
}

//==============================================================
//メッシュ情報取得
//==============================================================
LPD3DXMESH CMaterial::GetMeshAddress(int nIdx)
{
	return m_Material[nIdx].pMesh;
}