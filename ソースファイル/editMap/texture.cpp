//==============================================================
//
//テクスチャの処理[texture.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"

//静的メンバ変数宣言
int CTexture::m_nNumAll = -1;
const char *CTexture::m_apFilename[MAX_TEX] =
{
	"data\\TEXTURE\\player000.png",		//プレイヤー
	"data\\TEXTURE\\enemy000.png",		//敵
	"data\\TEXTURE\\000.jpg",			//床
	//"data\\TEXTURE\\bullet000.png",		//壁
	"data\\TEXTURE\\block000.jpg",		//ブロック
	"data\\TEXTURE\\bullet000.png",		//弾
	"data\\TEXTURE\\effect000.jpg",		//エフェクト
	"data\\TEXTURE\\score002.png",		//スコア
};

//==============================================================
//コンストラクタ
//==============================================================
CTexture::CTexture()
{
	//テクスチャ初期化
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		m_apTexture[nCntTex] = NULL;
	}
}

//==============================================================
//デストラクタ
//==============================================================
CTexture::~CTexture()
{

}

//==============================================================
//テクスチャ読み込み処理
//==============================================================
HRESULT CTexture::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得

	for (int nCntTex = 0; nCntTex < TEX_MAX; nCntTex++)
	{
		if (m_apTexture[nCntTex] == NULL)
		{//テクスチャ設定されてないとき

			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				m_apFilename[nCntTex],
				&m_apTexture[nCntTex]);

			m_nNumAll++;		//テクスチャの数加算
		}
	}

	return S_OK;
}

//==============================================================
//テクスチャ破棄処理
//==============================================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		//テクスチャの破棄
		if (m_apTexture[nCntTex] != NULL)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = NULL;
		}
	}
}

//==============================================================
//テクスチャ番号指定処理
//==============================================================
int CTexture::Regit(const char *pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得

	for (int nCntTex = 0; nCntTex < MAX_TEX; nCntTex++)
	{
		if (m_apFilename[nCntTex] == pFilename)
		{//ファイル名が同じとき

			return nCntTex;		//番号
		}
		else if (m_apFilename[nCntTex] != pFilename && nCntTex > m_nNumAll)
		{//ファイル名が違う

			m_apFilename[nCntTex] = pFilename;

			//テクスチャの読み込み
			D3DXCreateTextureFromFile(pDevice,
				m_apFilename[nCntTex],
				&m_apTexture[nCntTex]);

			m_nNumAll++;

			return m_nNumAll;		//テクスチャの番号を返す
		}
	}

	return m_nNumAll;		//テクスチャの番号を返す
}

//==============================================================
//テクスチャの情報取得
//==============================================================
LPDIRECT3DTEXTURE9 CTexture::GetAddress(int nIdx)
{
	return m_apTexture[nIdx];		//指定のテクスチャを返す
}