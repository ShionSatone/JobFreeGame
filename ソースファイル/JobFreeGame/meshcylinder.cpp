//==============================================================
//
// メッシュの円柱の処理[meshcylinder.cpp]
// Author:佐藤根詩音
//
//==============================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define NUM_VEC		(18)		// 頂点数
#define LENGTH		(500.0f)	//長さ

//**************************************************************
// 静的メンバ変数宣言
//**************************************************************

//==============================================================
// コンストラクタ
//==============================================================
CMesuCylinder::CMesuCylinder()
{
	m_nIdxTexture = -1;		// テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
}
//
////==============================================================
//// コンストラクタ
////==============================================================
//CMesuCylinder::CMesuCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
//{
//	m_nIdxTexture = -1;		// テクスチャの番号
//	m_pos = pos;		// 位置
//	m_rot = rot;		// 向き
//}

//==============================================================
// デストラクタ
//==============================================================
CMesuCylinder::~CMesuCylinder()
{

}

//==============================================================
//テクスチャ割り当て処理
//==============================================================
void CMesuCylinder::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//テクスチャ割り当て
}

//==============================================================
// メッシュシリンダーの生成処理
//==============================================================
CMesuCylinder* CMesuCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CMesuCylinder* pMesuCylinder = NULL;

	if (pMesuCylinder == NULL)
	{//メモリが使用されてなかったら

		//メッシュシリンダーの生成
		pMesuCylinder = new CMesuCylinder;
	}

	//初期化処理
	pMesuCylinder->Init();

	return pMesuCylinder;
}

//==============================================================
// メッシュシリンダーの初期化処理
//==============================================================
HRESULT CMesuCylinder::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VEC,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * NUM_VEC,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx;				// 頂点情報へのポインタ
	int nCntAngle = 0;				// 角度のカウンター
	int nCntTex = 0;				// テクスチャのカウンター
	float fAngle = 0.0f;			// 角度
	float fPosY = 200.0f;			// 高さ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_VEC; nCnt++)
	{
		nCntTex = (nCnt % NUM_VEC) * (1.0f / (NUM_VEC * 0.5f));

		// 頂点座標の設定
		pVtx[nCnt].pos.x = m_pos.x + sinf(m_rot.y + fAngle * D3DX_PI) * LENGTH;
		pVtx[nCnt].pos.y = fPosY;
		pVtx[nCnt].pos.z = m_pos.z + cosf(m_rot.y + fAngle * D3DX_PI) * LENGTH;

		// 法線ベクトルの設定
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラーの設定
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//テクスチャ座標の設定
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + nCntTex, 1.0f);

		if (fAngle >= 1.0f)
		{
			fAngle *= -1.0f;
		}
		
		if (fAngle != 0.0f)
		{
			fAngle += 0.25f;
		}
		else
		{
			if (2 <= nCntAngle)
			{
				fAngle += 0.25f;
			}
			else
			{
				nCntAngle++;

			}
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================
// メッシュシリンダーの終了処理
//==============================================================
void CMesuCylinder::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファ破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
// メッシュシリンダーの更新処理
//==============================================================
void CMesuCylinder::Update(void)
{

}

//==============================================================
// メッシュシリンダーの描画処理
//==============================================================
void CMesuCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	CTexture* pTexture = CManager::GetInstance()->GetTexture();		//テクスチャの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//オブジェクト3Dの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//プリミティブ（オブジェクト3Dの数）
		2);												//描画するプリミティブ数
}