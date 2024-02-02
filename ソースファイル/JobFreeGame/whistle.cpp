//==============================================================
//
// 笛の範囲処理[whistle.h]
// Author:佐藤根詩音
//
//==============================================================
#include "whistle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "texture.h"
#include "point.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度

#define MOVE				(6.0f)		//移動量
#define DISTANCE			(500.0f)	//距離

#define SIZE_PLANE			(D3DXVECTOR3(2.0f,0.0f,2.0f))		// 平面範囲の初期値

#define TIME_PLANE			(30.0f)		// 平面呼びかけ時間
#define TIME_CYLINDER		(100.0f)	// 円柱呼びかけ時間
#define SCALE_PLANE			(6.0f)		// 平面の拡大率


//**************************************************************
// 静的メンバ変数宣言
//**************************************************************
CObject3D* CWhistle::m_pObject3D[TYPE_MAX] = {};	// 3Dオブジェクト

//==============================================================
// コンストラクタ
//==============================================================
CWhistle::CWhistle()
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;		// 集合の情報
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_size = SIZE_PLANE;		// 大きさ
	m_nWhistleCounter = 0;		// 笛を鳴らす時間
	m_fscale = 0.0f;			// 拡大率

	m_state = STATE_NONE;		// 状態
	m_type = TYPE_PLANE;		// 種類
}

//==============================================================
// コンストラクタ(オーバーロード)
//==============================================================
CWhistle::CWhistle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;		// 集合の情報
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_size = SIZE_PLANE;		// 大きさ
	m_nWhistleCounter = 0;		// 笛を鳴らす時間
	m_fscale = 0.0f;			// 拡大率

	m_state = STATE_NONE;		// 状態
	m_type = TYPE_PLANE;		// 種類
}

//==============================================================
// デストラクタ
//==============================================================
CWhistle::~CWhistle()
{

}

//==============================================================
// 笛の範囲の生成処理
//==============================================================
CWhistle* CWhistle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CWhistle* pWhistle = NULL;

	if (pWhistle == NULL)
	{//メモリが使用されてなかったら

		//笛の範囲の生成
		pWhistle = new CWhistle(pos, rot);
	}

	//初期化処理
	pWhistle->Init();

	return pWhistle;
}

//==============================================================
// 笛の範囲の初期化処理
//==============================================================
HRESULT CWhistle::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	m_nIdxTexture[TYPE_PLANE] = pTexture->Regist("data\\TEXTURE\\whistle00.png");
	//m_nIdxTexture[TYPE_CYLINDER] = pTexture->Regist("data\\TEXTURE\\whistle01.png");

	m_fscale = SCALE_PLANE;			// 拡大率

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_pObject3D[nCnt] = CObject3D::Create();

		if (m_pObject3D[nCnt] != nullptr)
		{ // オブジェクト3Dが NULL じゃない場合

			// テクスチャの割り当て
			m_pObject3D[nCnt]->BindTexture(m_nIdxTexture[nCnt]);

			// オブジェクト3Dの初期化処理
			m_pObject3D[nCnt]->Init();

			// 種類の設定
			m_pObject3D[nCnt]->SetType(CObject::TYPE_POINT);

			// 位置の設定
			m_pObject3D[nCnt]->SetPos(m_pos);

			// 大きさ設定
			m_pObject3D[nCnt]->SetSize(m_size);
			
		}
		else
		{
			// 停止する
			assert(false);
		}
	}

	return S_OK;
}

//==============================================================
// 笛の範囲の終了処理
//==============================================================
void CWhistle::Uninit(void)
{
	//オブジェクト3Dの終了処理
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 位置設定
		m_pObject3D[nCnt]->Uninit();
	}
}

//==============================================================
// 笛の範囲の更新処理
//==============================================================
void CWhistle::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//前回の位置更新
	m_posOld = m_pos;

	// 追尾処理
	Follow();

	// 状態の更新
	UpdateState();

	//当たり判定
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//デバッグ表示
	pDebugProc->Print("\n笛の範囲の位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("\n笛の範囲の大きさ (%f, %f, %f)\n", m_size.x, m_size.y, m_size.z);

}

//==============================================================
// 状態の更新処理
//==============================================================
void CWhistle::UpdateState(void)
{
	float fSize = (m_nWhistleCounter / m_fscale);

	switch (m_state)
	{
	case CWhistle::STATE_NONE:	// 何もしない状態
		break;
	case CWhistle::STATE_PLANE:	// 平面状態

		if (m_nWhistleCounter >= TIME_PLANE)
		{ // 一定時間経ったら

			// 範囲を円柱にする
			m_state = STATE_CYLINDER;
		}
		else
		{
			// 範囲を大きくする
			m_size = D3DXVECTOR3(m_size.x + SIZE_PLANE.x * m_fscale, m_size.y, m_size.z + SIZE_PLANE.z * m_fscale);

			// 位置の設定
			m_pObject3D[TYPE_PLANE]->SetSize(m_size);
		}

		// 笛のカウンター
		m_nWhistleCounter++;

		break;
	case CWhistle::STATE_CYLINDER:	// 円柱状態
		break;
	
	default:

		// 停止する
		assert(false);

		break;
	}
}

//==============================================================
// 追尾処理
//==============================================================
void CWhistle::Follow(void)
{
	CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();

	m_pos = pPoint->GetPos();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// 位置設定
		m_pObject3D[nCnt]->SetPos(m_pos);
	}
}

//==============================================================
// 笛の範囲の描画処理
//==============================================================
void CWhistle::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// オブジェクト3Dの描画処理
		m_pObject3D[nCnt]->Draw();
	}
}

//==============================================================
// 位置設定
//==============================================================
void CWhistle::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_pObject3D[nCnt]->SetPos(m_pos);
	}
}

//==============================================================
// 位置取得
//==============================================================
D3DXVECTOR3 CWhistle::GetPos(void)
{
	return m_pos;
}