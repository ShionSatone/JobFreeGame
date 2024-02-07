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
#include "lucmin.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define PRIORITY		(3)		// 優先順位
#define LIFE			(5)		// ギミックの寿命
#define DAMAGE_TIME		(10)	// ダメージ継続時間

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
	m_state = STATE_NONE;					// ギミックの状態
	m_nLife = LIFE;			// 寿命
	m_nDamageCounter = 0;	// ダメージカウンター
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
	m_state = STATE_NONE;					// ギミックの状態
	m_nLife = LIFE;			// 寿命
	m_nDamageCounter = 0;	// ダメージカウンター

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

	// 状態更新
	UpdateState();

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
// 状態の更新処理
//==============================================================
void CGimmick::UpdateState(void)
{
	switch (m_state)
	{
	case CGimmick::STATE_NONE:		// 通常状態
		break;
	case CGimmick::STATE_DAMAGE:	// ダメージ状態

		if (m_nDamageCounter >= DAMAGE_TIME)
		{ // 一定時間経過したら

			// 通常状態にする
			m_state = STATE_NONE;
			CObjectX::SetState(CObject::STATE_NONE);

			m_nDamageCounter = 0;
		}
		else
		{
			// ダメージカウンター加算
			m_nDamageCounter++;
		}

		break;
	case CGimmick::STATE_DEATH:		// 死亡状態

		// ギミックの存在確認する
		CheckExist();

		// 終了処理
		Uninit();

		break;

	default:

		// 停止する
		assert(false);

		break;
	}
}

//==============================================================
// ヒット処理
//==============================================================
void CGimmick::Hit(void)
{
	m_nLife--;		// 寿命減らす

	if (m_nLife > 0)
	{ // 寿命がまだあるとき

		//状態設定
		CObjectX::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		// ダメージ状態にする
		m_state = STATE_DAMAGE;
		CObjectX::SetState(CObject::STATE_DAMAGE);
	}
	else if(m_nLife <= 0)
	{ // 寿命がないとき

		// 死亡状態にする
		m_state = STATE_DEATH;
	}
}

//==============================================================
// ギミックの存在確認処理
//==============================================================
void CGimmick::CheckExist(void)
{
	CLucmin* pLucmin = nullptr;

	for (int nCnt = 0; nCnt < MAX_LUCMIN; nCnt++)
	{
		// ルクミンの情報取得
		pLucmin = CManager::GetInstance()->GetScene()->GetGame()->GetLucmin(nCnt);

		if (pLucmin != nullptr)
		{ // ルクミンが NULL じゃない場合

			if (pLucmin->GetGimmickObj() != nullptr && pLucmin->GetState() == CLucmin::STATE_ATTACK)
			{ // ギミックのオブジェクトが NULL じゃない && 攻撃状態の場合

				// NULL の情報を渡す
				pLucmin->SetGimmickObj(nullptr);

				// NULL の情報を渡す
				pLucmin = nullptr;
			}
		}

	}
}
