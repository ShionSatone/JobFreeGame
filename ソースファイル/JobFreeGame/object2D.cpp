//==============================================================
//
//オブジェクト2D処理[object2D.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "object2D.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//マクロ定義
#define PRIORITY		(3)					//優先順位
#define ANGLE_UP		(0.75f)				//上の角度
#define ANGLE_DOWN		(0.25f)				//下の角度

//==============================================================
//コンストラクタ
//==============================================================
CObject2D::CObject2D()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_type = CObject::TYPE_NONE;		//種類
	m_fHeight = 0.0f;		//縦幅
	m_fWidth = 0.0f;		//横幅
}

//==============================================================
//デストラクタ
//==============================================================
CObject2D::~CObject2D()
{

}

//==============================================================
//生成処理
//==============================================================
CObject2D *CObject2D::Create(void)
{
	CObject2D *pObject2D = NULL;

	if (pObject2D == NULL)
	{//メモリが使用されてなかったら

		//オブジェクト2Dの生成
		pObject2D = new CObject2D;
	}

	//初期化処理
	pObject2D->Init();

	return pObject2D;
}

//==============================================================
//オブジェクト2Dの初期化処理
//==============================================================
HRESULT CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fWidth;
	pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fHeight;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fWidth;
	pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fHeight;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fWidth;
	pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fHeight;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fWidth;
	pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fHeight;
	pVtx[3].pos.z = 0.0f;

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	//種類設定
	CObject::SetType(CObject::TYPE_NONE);

	return S_OK;
}

//==============================================================
//オブジェクト2Dの終了処理
//==============================================================
void CObject2D::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//オブジェクト2Dの更新処理
//==============================================================
void CObject2D::Update(void)
{
	
}

//==============================================================
//オブジェクト2Dの描画処理
//==============================================================
void CObject2D::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//テクスチャの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//2Dオブジェクトの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//プリミティブ（プレイヤーの数）
		2);												//描画するプリミティブ数
}

//==============================================================
//テクスチャ割り当て処理
//==============================================================
void CObject2D::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//テクスチャ割り当て
}

//==============================================================
//オブジェクト2Dの位置設定処理
//==============================================================
void CObject2D::SetPos(D3DXVECTOR3 pos)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ
	m_pos = pos;			//位置

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case CObject::TYPE_NONE:

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

		break;

	case CObject::TYPE_PLAYER:		//プレイヤー

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);

		break;

	case CObject::TYPE_FADE:		//フェード

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	case CObject::TYPE_EFFECT:		//エフェクト

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	case CObject::TYPE_SCORE:		//スコア

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y - m_fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth, m_pos.y + m_fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

		break;

	//case CObject::TYPE_BLOCK:		//ブロック

	//	//頂点座標の設定
	//	pVtx[0].pos = D3DXVECTOR3(m_pos.x, m_pos.y, 0.0f);
	//	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y, 0.0f);
	//	pVtx[2].pos = D3DXVECTOR3(m_pos.x, m_pos.y + m_fHeight, 0.0f);
	//	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth, m_pos.y + m_fHeight, 0.0f);

	//	break;

	default:

		//頂点座標の設定
		pVtx[0].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fWidth;
		pVtx[0].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_UP) * m_fHeight;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fWidth;
		pVtx[1].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_UP) * m_fHeight;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = m_pos.x + sinf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fWidth;
		pVtx[2].pos.y = m_pos.y + cosf(m_rot.z + -D3DX_PI * ANGLE_DOWN) * m_fHeight;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = m_pos.x + sinf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fWidth;
		pVtx[3].pos.y = m_pos.y + cosf(m_rot.z + D3DX_PI * ANGLE_DOWN) * m_fHeight;
		pVtx[3].pos.z = 0.0f;

		break;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//オブジェクト2Dの色設定処理
//==============================================================
void CObject2D::SetColor(D3DXCOLOR col)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	m_col = col;		//色

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case CObject::TYPE_EFFECT:		//エフェクト

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

		break;

	default:

		//頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
		pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

		break;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//アニメーション設定処理
//==============================================================
void CObject2D::SetAnim(int nPatternAnim)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (m_type)
	{
	case TYPE_EXPLOSION:

		//テクスチャ座標の再設定
		pVtx[0].tex = D3DXVECTOR2(nPatternAnim * 0.125f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nPatternAnim * 0.125f + 0.125f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nPatternAnim * 0.125f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nPatternAnim * 0.125f + 0.125f, 1.0f);

		break;
	}
	
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//背景設定処理
//==============================================================
void CObject2D::SetbgMulti(float fTexU, float fTexV)
{
	VERTEX_2D * pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の更新
	pVtx[0].tex = D3DXVECTOR2(fTexU, fTexV);
	pVtx[1].tex = D3DXVECTOR2(fTexU + 1.0f, fTexV);
	pVtx[2].tex = D3DXVECTOR2(fTexU, fTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(fTexU + 1.0f, fTexV + 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//オブジェクト2Dの描画設定
//==============================================================
void CObject2D::SetDraw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	CTexture *pTexture = CManager::GetInstance()->GetTexture();		//テクスチャの取得

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	//テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//2Dオブジェクトの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//プリミティブの種類
		0,												//プリミティブ（プレイヤーの数）
		2);												//描画するプリミティブ数

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//==============================================================
//オブジェクト2Dの種類設定設定
//==============================================================
void CObject2D::SetType(CObject::TYPE type)
{
	m_type = type;

	//種類設定
	CObject::SetType(m_type);
}