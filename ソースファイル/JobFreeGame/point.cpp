//==============================================================
//
//ポイントの処理[point.h]
//Author:佐藤根詩音
//
//==============================================================
#include "point.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "player.h"
#include "texture.h"
#include "whistle.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度

#define MOVE				(6.0f)		//移動量
#define DISTANCE			(500.0f)	//距離

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間

//**************************************************************
//静的メンバ変数宣言
//**************************************************************

//==============================================================
//コンストラクタ
//==============================================================
CPoint::CPoint()
{
	m_nIdxTexture = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置

	m_state = STATE_NONE;			// 状態
	m_color = COLOR_NONE;			// 色
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CPoint::CPoint(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_nIdxTexture = -1;
	m_pos = pos;									// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置

	m_state = STATE_NONE;			// 状態
	m_color = COLOR_NONE;			// 色
}

//==============================================================
//デストラクタ
//==============================================================
CPoint::~CPoint()
{

}

//==============================================================
//ポイントの生成処理
//==============================================================
CPoint* CPoint::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPoint* pPoint = NULL;

	if (pPoint == NULL)
	{//メモリが使用されてなかったら

		//ポイントの生成
		pPoint = new CPoint(pos, rot);
	}

	//初期化処理
	pPoint->Init();

	return pPoint;
}

//==============================================================
//ポイントの初期化処理
//==============================================================
HRESULT CPoint::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//地上テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\point000.png");

	// テクスチャの割り当て
	CObject3D::BindTexture(m_nIdxTexture);

	//オブジェクト3Dの初期化処理
	CObject3D::Init();

	//種類の設定
	CObject3D::SetType(CObject::TYPE_POINT);

	//位置の設定
	CObject3D::SetPos(m_pos);

	//ポイントの大きさ設定
	CObject3D::SetSize(D3DXVECTOR3(100.0f, 0.0f, 100.0f));

	return S_OK;
}

//==============================================================
//ポイントの終了処理
//==============================================================
void CPoint::Uninit(void)
{
	//オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//==============================================================
//ポイントの更新処理
//==============================================================
void CPoint::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();

	//前回の位置更新
	m_posOld = m_pos;

	//当たり判定
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	// 笛の処理
	ContlolWhistle();

	// 状態更新
	UpdateState();

	// 色更新
	UpdateColor();

	// 画面外処理
	Screen();

	//デバッグ表示
	pDebugProc->Print("\nポイントの位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//==============================================================
// 状態更新処理
//==============================================================
void CPoint::UpdateState(void)
{
	switch (m_state)
	{
	case CPoint::STATE_NONE:		// 通常状態

		// 追尾処理
		Follow();

		break;

	case CPoint::STATE_LOCKON:		// ロックオン
		break;
	case CPoint::STATE_THROW:		// 投げる状態

		//操作処理
		ControlKeyboard();

		break;

	case CPoint::STATE_WHISTLE:		// 呼び状態

		// 追尾処理
		Follow();

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
void CPoint::Follow(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// ポイントの情報取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();

	m_pos.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * DISTANCE;
	m_pos.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * DISTANCE;

	// 位置設定
	CObject3D::SetPos(m_pos);
}

//==============================================================
// 色更新処理
//==============================================================
void CPoint::UpdateColor(void)
{
	switch (m_color)
	{
	case CPoint::COLOR_NONE:		// 通常色(灰色)
		break;
	case CPoint::COLOR_SPRING:		// 春(ピンク)
		break;
	case CPoint::COLOR_SUMMER:		// 夏(緑)
		break;
	case CPoint::COLOR_FALL:		// 秋(オレンジ)
		break;
	case CPoint::COLOR_WINTER:		// 冬(白)
		break;
	default:

		// 停止する
		assert(false);

		break;
	}
}

//==============================================================
// ポイントのキーボード操作処理
//==============================================================
void CPoint::ControlKeyboard(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//移動
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{//右
		if (pInputKeyboard->GetPress(DIK_I) == true)
		{//奥
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;

		}
		else if (pInputKeyboard->GetPress(DIK_K) == true)
		{//手前
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;

		}
		else
		{
			m_pos.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_J) == true)
	{//左

		if (pInputKeyboard->GetPress(DIK_I) == true)
		{//奥
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;

		}
		else if (pInputKeyboard->GetPress(DIK_K) == true)
		{//手前
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;

		}
		else
		{
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_I) == true)
	{//奥
		m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
		m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_K) == true)
	{//手前
		m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
		m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
	}

	//位置の設定
	CObject3D::SetPos(m_pos);
}

//==============================================================
// 呼びかけ処理
//==============================================================
void CPoint::ContlolWhistle(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CWhistle* pWhistle = nullptr;

	if (pInputKeyboard->GetPress(DIK_RETURN) == true)
	{ // ENTERを押したとき

		if (m_state != STATE_WHISTLE)
		{ // 集合状態じゃないとき

			if (pWhistle == nullptr)
			{
				// 集合範囲の生成
				pWhistle = CWhistle::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				// 呼びかけ開始状態にする
				pWhistle->SetState(CWhistle::STATE_PLANE);
			}

			// 集合状態にする
			m_state = STATE_WHISTLE;

		}

	}
}

//==============================================================
// 画面外判定処理
//==============================================================
void CPoint::Screen(void)
{
	
}

//==============================================================
// ポイントの描画処理
//==============================================================
void CPoint::Draw(void)
{
	// オブジェクト3Dの描画処理
	CObject3D::Draw();
}