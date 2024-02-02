//==============================================================
//
//数字処理[number.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "number.h"
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//マクロ定義
#define PRIORITY		(4)				//優先順位
#define ANGLE_UP		(0.75f)			//上の角度
#define ANGLE_DOWN		(0.25f)			//下の角度

//==============================================================
//コンストラクタ
//==============================================================
CNumber::CNumber()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_type = CObject::TYPE_NONE;		//種類
	m_fHeight = 0.0f;		//縦幅
	m_fWidth = 0.0f;		//横幅
}

//==============================================================
//デストラクタ
//==============================================================
CNumber::~CNumber()
{

}

//==============================================================
//生成処理
//==============================================================
CNumber *CNumber::Create(void)
{
	CNumber *pNumber = NULL;

	if (pNumber == NULL)
	{//メモリが使用されてなかったら

		//数字の生成
		pNumber = new CNumber;
	}

	//初期化処理
	pNumber->Init();

	return pNumber;
}

//==============================================================
//数字の初期化処理
//==============================================================
HRESULT CNumber::Init(void)
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

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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

	return S_OK;
}

//==============================================================
//数字の終了処理
//==============================================================
void CNumber::Uninit(void)
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
//数字の更新処理
//==============================================================
void CNumber::Update(void)
{
	
}

//==============================================================
//数字の描画処理
//==============================================================
void CNumber::Draw(void)
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
void CNumber::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//テクスチャ割り当て
}

//==============================================================
//数字の位置設定処理
//==============================================================
void CNumber::SetPosition(TYPE type, D3DXVECTOR3 pos, float fWidth, float fHeight)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	m_pos = pos;		//位置

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	switch (type)
	{
	case CObject::TYPE_SCORE:		//スコア

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y - fHeight, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y - fHeight, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - fWidth, m_pos.y + fHeight, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + fWidth, m_pos.y + fHeight, 0.0f);

		break;

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
//数字の色設定処理
//==============================================================
void CNumber::SetColor(TYPE type, D3DXCOLOR col)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	m_col = col;		//色

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[1].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[2].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);
	pVtx[3].col = D3DXCOLOR(m_col.r, m_col.g, m_col.b, m_col.a);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==============================================================
//アニメーションの設定処理
//==============================================================
void CNumber::SetAnim(int nPatternAnim, float fDigitX)
{
	VERTEX_2D *pVtx;		//頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	/*switch (m_type)
	{
	case TYPE_SCORE:*/

		//テクスチャ座標の再設定
		pVtx[0].tex = D3DXVECTOR2(nPatternAnim * fDigitX, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(nPatternAnim * fDigitX + fDigitX, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(nPatternAnim * fDigitX, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(nPatternAnim * fDigitX + fDigitX, 1.0f);

		/*break;
	}*/
	
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}