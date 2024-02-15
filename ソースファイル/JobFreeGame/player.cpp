//==============================================================
//
//プレイヤー処理[playerModel.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "modelHierarchy.h"
#include "input.h"
#include "motion.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "model.h"
#include "lucmin.h"
#include "whistle.h"
#include "point.h"
//#include "particle.h"

//マクロ定義
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define MAX_STR				(128)		//文字の最大数
#define MOVE				(0.6f)		//手前の時の移動量
#define STOP_MOVE			(0.8f)		//止まる判定の移動量
#define FILE_HUMAN			"data\\TXT\\motion_player.txt"		//プレイヤーモデルのテキスト

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間
#define STEP_CNT			(10)		//歩く音のカウンター

//静的メンバ変数宣言
char *CPlayer::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\player\\00_body.x",
	"data\\MODEL\\player\\01_head.x",
	"data\\MODEL\\player\\02_hair.x",
	"data\\MODEL\\player\\03_LU_arm.x",
	"data\\MODEL\\player\\04_LD_arm.x",
	"data\\MODEL\\player\\05_L_hand.x",
	"data\\MODEL\\player\\06_RU_arm.x",
	"data\\MODEL\\player\\07_RD_arm.x",
	"data\\MODEL\\player\\08_R_arm.x",
	"data\\MODEL\\player\\09_waist.x",
	"data\\MODEL\\player\\10_LU_leg.x",
	"data\\MODEL\\player\\11_LD_leg.x",
	"data\\MODEL\\player\\12_L_shoe.x",
	"data\\MODEL\\player\\13_RU_leg.x",
	"data\\MODEL\\player\\14_RD_leg.x",
	"data\\MODEL\\player\\15_R_shoe.x",

};

//==============================================================
//コンストラクタ
//==============================================================
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量保存用

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = nullptr;		//プレイヤー(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//プレイヤー(パーツ)の総数
	m_pMotion = nullptr;

	m_fRotDest = 0.0f;		//目標
	m_fRotDiff = 0.0f;		//差分

	m_nDashCounter = 0;		//ダッシュした回数

	m_particleType = PARTICLETYPE_NONE;	//パーティクルの種類
	m_nType = 1;		//何番目のパーティクルか
	m_nParticleLife = 50;			//パーティクルの寿命
	m_state = STATE_NONE;			//状態
	m_nCntHit = HIT_CNT;			//攻撃あたるまでのカウンター
	m_bPad = false;					//パッドのスティックを倒してるか
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CPlayer::CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;						//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量保存用

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値
	m_rot = rot;		//向き

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = nullptr;		//プレイヤー(パーツ)へのポインタ
	}

	m_pMotion = nullptr;		//モーションの情報
	m_nNumModel = 0;		//プレイヤー(パーツ)の総数

	//m_bJump = false;		//ジャンプしたか
	//m_bMove = false;		//歩いてるか
	//m_bLand = true;			//着地した
	//m_bDash = false;		//ダッシュしたか

	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分

	m_nDashCounter = 0;		//ダッシュした回数

	m_particleType = PARTICLETYPE_NONE;	//パーティクルの種類
	m_nType = 1;				//何番目のパーティクルか
	m_nParticleLife = 50;		//パーティクルの寿命
	m_state = STATE_NONE;		//状態
	m_nCntHit = HIT_CNT;		//攻撃あたるまでのカウンター
	m_bPad = false;				//パッドのスティックを倒してるか

}

//==============================================================
//デストラクタ
//==============================================================
CPlayer::~CPlayer()
{

}

//==============================================================
//プレイヤーの生成処理
//==============================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{//メモリが使用されてなかったら

		//プレイヤーの生成
		pPlayer = new CPlayer(pos, rot);
	}

	//初期化処理
	pPlayer->Init();

	return pPlayer;
}

//==============================================================
//プレイヤーの初期化処理
//==============================================================
HRESULT CPlayer::Init(void)
{
	//CLife *pLife = CGame::GetLife();

	m_fRotDest = m_rot.y;

	//プレイヤーの生成（全パーツ分）
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//モーションの初期化・生成
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//モデルのファイル読み込み
	CPlayer::LoadFile();

	//最大値・最小値代入
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//最大値Y
		if ((nCntPlayer <= PARTS_HEAD) || (nCntPlayer >= PARTS_WAIST && nCntPlayer <= PARTS_L_SHOE))
		{
			m_max.y += m_apModel[nCntPlayer]->GetSizeMax().y;		//最大値加算
		}

		//最大値入れ替え
		if (m_max.x < m_apModel[nCntPlayer]->GetSizeMax().x)
		{
			m_max.x = m_apModel[nCntPlayer]->GetSizeMax().x;		//最小値X
		}
		if (m_max.z < m_apModel[nCntPlayer]->GetSizeMax().z)
		{
			m_max.z = m_apModel[nCntPlayer]->GetSizeMax().z;		//最小値Z

		}

		//最小値入れ替え
		if (m_min.x > m_apModel[nCntPlayer]->GetSizeMin().x)
		{
			m_min.x = m_apModel[nCntPlayer]->GetSizeMin().x;		//最小値X
		}
		if (m_min.y > m_apModel[nCntPlayer]->GetSizeMin().y)
		{
			m_min.y = m_apModel[nCntPlayer]->GetSizeMin().y;		//最小値Y
		}
		if (m_min.z > m_apModel[nCntPlayer]->GetSizeMin().z)
		{
			m_min.z = m_apModel[nCntPlayer]->GetSizeMin().z;		//最小値Z

		}
	}

	m_max.y += 40.0f;

	//for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	//{
	//	//プレイヤーの色設定
	//	m_apModel[2]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	//	//	//状態設定
	//	m_apModel[2]->SetState(CObjectX::STATE_DAMAGE);		//ダメージ状態にする
	//}

	SetType(CObject::TYPE_PLAYER);

	return S_OK;
}

//==============================================================
//プレイヤーの終了処理
//==============================================================
void CPlayer::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{//使用されてるとき

			//終了処理
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];
			m_apModel[nCnt] = NULL;
		}
	}

	if (m_pMotion != NULL)
	{//使用されてるとき

		//モーションの破棄
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//オブジェクト（自分自身の破棄）
	Release();
}

//==============================================================
//プレイヤーの更新処理
//==============================================================
void CPlayer::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CFade* pFade = CManager::GetInstance()->GetFade();
	//CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	//CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//前回の位置更新
	m_posOld = m_pos;

	if (pFade->GetFade() != CFade::FADE_OUT)
	{//自動ダッシュしてないとき

		//プレイヤーの操作
		CPlayer::ControlKeyboard();
		//CPlayer::ControltJoyPad();
	}

	//足音鳴らす処理
	CPlayer::SEStep();

	//当たった時の行動処理
	CPlayer::CollisionAction();

	//状態更新
	CPlayer::UpdateState();

	//向きの補正
	CPlayer::RotNormalize();

	//画面外処理
	CPlayer::Screen();

	//モーション管理
	CPlayer::MotionManager();

	if (m_pos.x >= 350.0f &&
		m_pos.x <= 450.0f &&
		m_pos.z >= 1650.0f &&
		m_pos.z <= 1750.0f)
	{
		// リザルトに遷移
		pFade->SetFade(CScene::MODE_RESULT);
	}

	//モーションの更新処理
	m_pMotion->Update();

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//階層構造の更新処理
		m_apModel[nCntPlayer]->Update();
	}

	//デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("プレイヤーの移動量    (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("プレイヤーの向き      (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);

}

//==============================================================
//状態更新処理
//==============================================================
void CPlayer::UpdateState(void)
{
	//CFade *pFade = CManager::GetInstance()->GetFade();

	switch (m_state)
	{
	case STATE_NONE:		// 通常状態

		break;

	case STATE_THROW:		// 投げる状態

		break;

	case STATE_WHISTLE:		// 呼びかけ状態

		break;

	case STATE_DAMAGE:		// ダメージ状態

		break;

	case STATE_APPEAR:		// 点滅状態

		break;

	case STATE_DEATH:		// 死亡状態

		break;

	default:

		// 停止する
		assert(false);

		break;
	}
}

//==============================================================
//当たった時の行動処理
//==============================================================
void CPlayer::CollisionAction(void)
{
	//CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	//CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//パッドの情報取得

	//bool bLand = CObjectX::Collision(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//当たり判定
	//if (bLand == true)
	//{//着地したら

	//	m_nDashCounter = 0;		//ダッシュ数リセット

	//	if (m_bLand == false)
	//	{
	//		//パーティクルの生成
	//		//CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(0.1f, 0.2f, 0.4f, 0.8f), PARTICLETYPE_MOVE, 20, 10.0f);
	//	}

	//	m_bJump = false;	//ジャンプしてない状態にする
	//	m_bLand = true;		//着地した状態にする

	//}
	//else if (bLand == false &&
	//	pInputKeyboard->GetPress(DIK_SPACE) == false && pInputJoyPad->GetPress(pInputJoyPad->BUTTON_A, 0) == false)
	//{//地面についてない && ジャンプボタン押してない

	//	m_bJump = true;		//ジャンプしてる状態にする
	//	m_bLand = false;	//着地してない状態にする
	//}
}

//==============================================================
//モーション管理処理
//==============================================================
void CPlayer::MotionManager(void)
{
	//if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	//{//歩いてる && 待機状態

	//	//歩かせる
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	//}
	////else if (m_pMotion->IsFinish() == true && m_bAction == true)
	////{//攻撃が終わったら

	////	m_bAction = false;		//攻撃してない状態にする
	////}
	//else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
	//	(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_bMove == false && m_bJump == false && m_bLand == true))
	//{//モーションが終了したら

	//	//待機状態に戻す
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	//}
}

//==============================================================
//向きの補正処理
//==============================================================
void CPlayer::RotNormalize(void)
{
	//向きの差分を求める
	m_fRotDiff = m_fRotDest - m_rot.y;

	//目標の方向までの差分を修正
	if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * 2;
	}
	else if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff -= D3DX_PI * 2;
	}

	//差分足す
	m_rot.y += m_fRotDiff * 0.1f;

	//現在の方向修正
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================================
//足音鳴らす処理
//==============================================================
void CPlayer::SEStep(void)
{
	//CSound *pSound = CManager::GetInstance()->GetSound();

	//足音鳴らす
	//if (m_bMove == true && m_bJump == false)
	//{//歩いてるとき && ジャンプしてない

	//	if ((m_nCntMove >= STEP_CNT))
	//	{//一定時間たったら

	//		//BGM再生
	//		//pSound->Play(pSound->SOUND_LABEL_SE_STEP);

	//		m_nCntMove = 0;
	//	}

	//	m_nCntMove++;		//足音鳴らすカウンター加算
	//}
	//else if(m_bMove == false)
	//{//歩いてないとき

	//	m_nCntMove = STEP_CNT;
	//}

}

//==============================================================
//プレイヤーのキーボード操作処理
//==============================================================
void CPlayer::ControlKeyboard(void)
{
	//移動処理
	ControlKeyboardMove();

	// ルクミン命令処理
	ControlKeyboardCommand();

	// 移動量加算
	m_pos += m_move;

	// 慣性付与
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;
}

//==============================================================
//プレイヤーのキーボードの移動操作処理
//==============================================================
void CPlayer::ControlKeyboardMove(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//移動
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//右
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//奥
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//手前
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.25f;

		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
		}

		//m_bMove = true;		//歩かせる
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//左

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//奥
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//手前
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.25f;

		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
		}

		//m_bMove = true;		//歩かせる
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//奥
		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_DOWN;

		//m_bMove = true;		//歩かせる
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//手前
		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
		m_fRotDest = pCamera->GetRotation().y;

		//m_bMove = true;		//歩かせる
	}

	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
	{//歩いてないとき

		//m_bMove = false;		//歩いてない状態にする
	}
}

//==============================================================
// 命令処理
//==============================================================
void CPlayer::ControlKeyboardCommand(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	if (pInputKeyboard->GetTrigger(DIK_L) == true)
	{ // Lキーを押したとき

		// 投げる状態にする
		m_state = STATE_THROW;

		// 状態設定
		SetStateThrow();
	}
	//else if (pInputKeyboard->GetPress(DIK_RETURN) == true && m_state != STATE_WHISTLE)
	//{ // ENTERキーを押した && 集合状態じゃないとき

	//	// 集合状態にする
	//	m_state = STATE_WHISTLE;

	//	// 状態設定
	//	//SetStateWhistle();
	//}
}

//==============================================================
// 投げる状態設定
//==============================================================
void CPlayer::SetStateThrow(void)
{
	CLucmin* pLucmin = nullptr;		// ルクミンの情報

	for (int nCnt = 0; nCnt < MAX_LUCMIN; nCnt++)
	{
		// ルクミンの情報取得
		pLucmin = CManager::GetInstance()->GetScene()->GetGame()->GetLucmin(nCnt);

		if (pLucmin != nullptr)
		{ // ルクミンが NULL じゃないとき

			if (pLucmin->GetState() == CLucmin::STATE_FOLLOW)
			{ // ルクミンが投げられ状態 && 呼びかけ && 探し状態じゃないとき

				pLucmin->SetState(CLucmin::STATE_THROW);		// 投げられ状態にする

				break;

			}
		}
	}
}

//==============================================================
// 集合状態設定
//==============================================================
void CPlayer::SetStateWhistle(void)
{
	//CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();		// ポイントの情報取得

	//pPoint->Set
}

//==============================================================
//プレイヤーのパッド操作処理
//==============================================================
//void CPlayer::ControlFrontJoyPad(void)
//{
//	CSound *pSound = CManager::GetInstance()->GetSound();
//
//	//移動処理
//	CPlayer::ControlFrontJoyPadMove();
//}
//
////==============================================================
////プレイヤーのパッドの移動操作処理
////==============================================================
//void CPlayer::ControlFrontJoyPadMove(void)
//{
//	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//パッドの情報取得
//	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
//
//	//移動
//	if (pInputJoyPad->GetPressLX(0).x > 0.0f)
//	{//右
//		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
//		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
//		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
//
//		m_bMove = true;		//歩かせる
//	}
//	else if (pInputJoyPad->GetPressLX(0).x < 0.0f)
//	{//左
//		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
//		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
//		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
//
//		m_bMove = true;		//歩かせる
//	}
//
//	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
//	{//歩いてないとき
//
//		m_bMove = false;		//歩いてない状態にする
//	}
//}

//==============================================================
//画面外判定処理
//==============================================================
void CPlayer::Screen(void)
{
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	//if (m_pos.y <= -6000.0f)
	//{//一番下まで行ったら

	//	CFade *pFade = CManager::GetInstance()->GetFade();			//フェードの情報取得
	//	/*CDeathUI *pDeathUI = CGame::GetDeathUI();
	//	CItemUI *pItemUI = CGame::GetItemUI();*/

	//	//スコア保存
	//	/*CManager::GetInstance()->SetNumDeath(pDeathUI->Get());
	//	CManager::GetInstance()->SetNumItem(pItemUI->Get());*/

	//	//リザルトの判定
	//	CManager::GetInstance()->SetResult(true);

	//	//ゲーム画面
	//	pFade->SetFade(CScene::MODE_RESULT);
	//}
}

//==============================================================
//プレイヤーの描画処理
//==============================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
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

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//プレイヤーの描画
		m_apModel[nCntPlayer]->Draw();
	}
}

//==============================================================
//プレイヤーのヒット処理
//==============================================================
void CPlayer::Hit(void)
{
	//CSound *pSound = CManager::GetInstance()->GetSound();
	//CDeathUI *pDeathUI = CGame::GetDeathUI();
	CFade *pFade = CManager::GetInstance()->GetFade();

	pFade->SetFade(CScene::MODE_RESULT);

	//if (m_state != CObject::STATE_DAMAGE && m_state != CObject::STATE_APPEAR && m_nCntDamage <= 0 && m_nCntHit <= 0 &&
	//	pFade->GetFade() != CFade::FADE_IN)
	//{
	//	//BGM再生
	//	//pSound->Play(pSound->SOUND_LABEL_SE_DAMAGE001);

	//	m_state = CObject::STATE_APPEAR;

	//	m_nCntDamage = HIT_CNT;				//ダメージ状態を保つ時間設定
	//	m_nCntHit = 60;				//攻撃あたるまでのカウンター

	//	pDeathUI->Add(1);		//死亡数加算

	//	//フェードさせる
	//	pFade->SetNormalFade();

	//	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	//	{
	//		//プレイヤーの色設定
	//		m_apModel[nCntPlayer]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	//	}

	//	//パーティクル生成
	//	//CParticle::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_EXPLOSION, 30, 40);

	//	//状態設定
	//	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	//	{
	//		m_apModel[nCntPlayer]->SetState(m_state);		//ダメージ状態にする
	//	}
	//}
}

//==============================================================
//モデルファイル読み込み処理
//==============================================================
void CPlayer::LoadFile(void)
{
	FILE *pFile;				//ファイルポインタ
	char aString[MAX_STR];		//文字読み込み
	int nIndex = 0, nParent = 0;	//パーツNo.,親番号
	D3DXVECTOR3 pos, rot;

	//ファイル開く
	pFile = fopen(FILE_HUMAN, "r");

	if (pFile != NULL)
	{//ファイルが開けた場合

		while (strcmp(&aString[0], "CHARACTERSET") != 0)
		{//[CHARACTERSET]するまでの間

			fscanf(pFile, "%s", &aString[0]);		//文字読み込み
		}

		if (strcmp(&aString[0], "CHARACTERSET") == 0)
		{//[CHARACTERSET]が来たら

			while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
			{//[END_CHARACTERSET]がくるまでの間

				fscanf(pFile, "%s", &aString[0]);		//文字読み込み

				if (strcmp(&aString[0], "PARTSSET") == 0)
				{//[PARTSSET]が来たら

					while (strcmp(&aString[0], "END_PARTSSET") != 0)
					{//[END_PARTSSET]がくるまでの間

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み

						if (strcmp(&aString[0], "INDEX") == 0)
						{//パーツNo.

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み
							fscanf(pFile, "%d", &nIndex);			//パーツNo.読み込み

						}
						else if (strcmp(&aString[0], "PARENT") == 0)
						{//親情報

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み
							fscanf(pFile, "%d", &nParent);			//親番号読み込み

							if (nParent == -1)
							{//親がいなかったら

								m_apModel[nIndex]->SetParent(NULL);		//NULLを入れる
							}
							else
							{//親がいたら

								m_apModel[nIndex]->SetParent(m_apModel[nParent]);		//親番号入れる
							}
						}
						else if (strcmp(&aString[0], "POS") == 0)
						{//位置情報

							fscanf(pFile, "%s", &aString[0]);	//文字読み込み

							fscanf(pFile, "%f", &pos.x);		//位置読み込み
							fscanf(pFile, "%f", &pos.y);		//位置読み込み
							fscanf(pFile, "%f", &pos.z);		//位置読み込み

							m_apModel[nIndex]->SetPos(pos);		//位置設定
							m_apModel[nIndex]->SetDefaultPos(pos);	//初期位置設定

						}
						else if (strcmp(&aString[0], "ROT") == 0)
						{//向き情報

							fscanf(pFile, "%s", &aString[0]);	//文字読み込み

							fscanf(pFile, "%f", &rot.x);		//向き読み込み
							fscanf(pFile, "%f", &rot.y);		//向き読み込み
							fscanf(pFile, "%f", &rot.z);		//向き読み込み

							m_apModel[nIndex]->SetRot(rot);		//向き設定
							m_apModel[nIndex]->SetDefaultRot(rot);	//初期向き設定
						}
					}
				}
			}
		}

		//ファイル閉じる
		fclose(pFile);
	}
}