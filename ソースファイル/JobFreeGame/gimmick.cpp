//==============================================================
//
// ギミックの処理[gimmick.cpp]
// Author:佐藤根詩音
//
//==============================================================
#include "gimmick.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "material.h"
#include "debugproc.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define PRIORITY		(3)		// 優先順位
#define LIFE			(10)	// ギミックの寿命

//**************************************************************
//静的メンバ変数宣言
//**************************************************************
int CGimmick::m_nNumAll = 0;			// ギミックの総数

//==============================================================
// コンストラクタ
//==============================================================
CGimmick::CGimmick()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//向き
	m_nIdxModel = -1;						// モデルの番号
	m_nIndex = m_nNumAll;					// ギミックの番号
	m_nNumAll++;							// ギミックの総数加算
	m_type = TYPE_NONE;						// ギミックの種類
	m_nLife = LIFE;			// 寿命

}

//==============================================================
// コンストラクタ
//==============================================================
CGimmick::CGimmick(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量
	m_rot = rot;		//向き
	m_nIdxModel = -1;						// モデルの番号
	m_nIndex = m_nNumAll;					// ギミックの番号
	m_nNumAll++;							// ギミックの総数加算
	m_type = type;							// ギミックの種類
	m_nLife = LIFE;			// 寿命

}

//==============================================================
// デストラクタ
//==============================================================
CGimmick::~CGimmick()
{

}

//==============================================================
// 生成処理
//==============================================================
CGimmick* CGimmick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CGimmick* pGimmick = NULL;

	if (pGimmick == NULL)
	{//メモリが使用されてなかったら

		//ギミックの生成
		pGimmick = new CGimmick(pos, rot, type);
	}

	//初期化処理
	pGimmick->Init();

	return pGimmick;
}

//==============================================================
// ギミックの初期化処理
//==============================================================
HRESULT CGimmick::Init(void)
{
	CMaterial* pMaterial = CManager::GetInstance()->GetMaterial();

	//モデルの読み込み
	m_nIdxModel = pMaterial->Regist("data\\MODEL\\item_star.x");

	//マテリアルの割り当て
	CObjectX::BindMaterial(m_nIdxModel);

	//オブジェクトXの初期化処理
	CObjectX::Init();

	//位置設定処理
	CObjectX::SetPos(m_pos);

	// 種類の設定
	CObject::SetType(CObject::TYPE_GIMMICK);

	return S_OK;
}

//==============================================================
// ギミックの終了処理
//==============================================================
void CGimmick::Uninit(void)
{
	//オブジェクトXの終了処理
	CObjectX::Uninit();
}

//==============================================================
// ギミックの更新処理
//==============================================================
void CGimmick::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	pDebugProc->Print("\nギミックの位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);


	//オブジェクトXの更新処理
	CObjectX::Update();
}

//==============================================================
// ギミックの描画処理
//==============================================================
void CGimmick::Draw(void)
{
	//オブジェクトXの描画処理
	CObjectX::Draw();
}

//==============================================================
// ヒット処理
//==============================================================
void CGimmick::Hit(void)
{

}