//==============================================================
//
//ルクミンの処理[lucmin.h]
//Author:佐藤根詩音
//
//==============================================================
#include "lucmin.h"
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
#include "player.h"
#include "whistle.h"
#include "point.h"
//#include "effect.h"

//マクロ定義
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度

#define GRAVITY				(-1.0f)		//重力
#define THROW_HEIGHT		(15.0f)		//投げられる高さ
#define MAX_STR				(128)		//文字の最大数
#define MOVE				(6.0f)		//移動量
#define MOVE_DISTANCE		(200.0f)	//移動距離
#define FILE_ENEMY			"data\\TXT\\motion_player.txt"		//ルクミンモデルのテキスト

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間
#define GIMMICK_LENGTH		(1000.0f)	//ギミックとルクミンの距離
#define WHISTLE_REACTION_TIME		(30)		// 集合時のリアクション時間
#define ATTACK_INTERVAL		(60 * 1)	// 攻撃の間隔

//静的メンバ変数宣言
int CLucmin::m_nNumAll = 0;						//ルクミンの総数
char *CLucmin::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\enemy\\00_body.x",
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
	"data\\MODEL\\enemy\\15_R_shoe.x",

};

//==============================================================
//コンストラクタ
//==============================================================
CLucmin::CLucmin()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の位置
	m_posDestSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の位置保存用
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最小値
	
	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//ルクミン(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//ルクミン(パーツ)の総数
	m_pMotion = NULL;

	m_nReactionCounter = 0;	// 集合のリアクションカウンター
	m_nAttackCounter = 0;	// 攻撃の間隔カウンター

	m_fRotDest = 0.0f;		//目標
	m_fRotDiff = 0.0f;		//差分
	m_fGimmickRadius = 0.0f;	// ギミックの半径

	m_state = STATE_FOLLOW;			//状態
	m_whistleState = WHISTLESTATE_NONE;	//呼びかけ状態
	m_throwState = THROWSTATE_NONE;		// 投げられ状態
	m_searchState = SEARCHSTATE_NONE;	// 探し状態
	m_nCntDamage = 0;				//ダメージカウンター

	m_pObject = nullptr;		// オブジェクトの情報

	m_nIndex = m_nNumAll;

	m_nNumAll++;

}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CLucmin::CLucmin(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;									// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の位置
	m_posDestSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	// 目的の位置保存用
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最小値

	m_rot = rot;		//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//ルクミン(パーツ)へのポインタ
	}

	m_pMotion = NULL;		//モーションの情報
	m_nNumModel = 0;		//ルクミン(パーツ)の総数

	m_nReactionCounter = 0;	// 集合のリアクションカウンター
	m_nAttackCounter = 0;	// 攻撃の間隔カウンター

	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分
	m_fGimmickRadius = 0.0f;	// ギミックの半径

	m_state = STATE_FOLLOW;		//状態
	m_whistleState = WHISTLESTATE_NONE;	//呼びかけ状態
	m_throwState = THROWSTATE_NONE;		// 投げられ状態
	m_searchState = SEARCHSTATE_NONE;	// 探し状態

	m_nCntDamage = 0;			//ダメージカウンター

	m_pObject = nullptr;		// オブジェクトの情報

	m_nIndex = m_nNumAll;

	m_nNumAll++;
}

//==============================================================
//デストラクタ
//==============================================================
CLucmin::~CLucmin()
{
	m_nNumAll--;
}

//==============================================================
//ルクミンの生成処理
//==============================================================
CLucmin *CLucmin::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLucmin *pLucmin = NULL;

	if (pLucmin == NULL)
	{//メモリが使用されてなかったら

		//ルクミンの生成
		pLucmin = new CLucmin(pos, rot);
	}

	//初期化処理
	pLucmin->Init();

	return pLucmin;
}

//==============================================================
//ルクミンの初期化処理
//==============================================================
HRESULT CLucmin::Init(void)
{
	m_fRotDest = m_rot.y;

	//ルクミンの生成（全パーツ分）
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//モーションの初期化・生成
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//モデルのファイル読み込み
	CLucmin::LoadFile();

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

	CObject::SetType(CObject::TYPE_ENEMY);

	return S_OK;
}

//==============================================================
//ルクミンの終了処理
//==============================================================
void CLucmin::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		if (m_apModel[nCnt] != nullptr)
		{//使用されてるとき

			//終了処理
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];
			m_apModel[nCnt] = nullptr;
		}
	}

	if (m_pMotion != nullptr)
	{//使用されてるとき

		//モーションの破棄
		delete m_pMotion;
		m_pMotion = nullptr;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//ルクミンの更新処理
//==============================================================
void CLucmin::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();
	CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();

	//前回の位置更新
	m_posOld = m_pos;

	//当たり判定
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	// 位置加算
	m_pos += m_move;

	// 画面外処理
	Screen();

	// 慣性付与
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	if (pPoint->CircleCollision(pPoint->GetPos(), m_pos, 1000.0f, 50.0f) == true &&
		pPoint->GetState() == CPoint::STATE_WHISTLE &&
		m_state != STATE_WHISTLE && m_state != STATE_FOLLOW && m_state != STATE_THROW)
	{
		if (m_state = STATE_ATTACK)
		{ // 攻撃してるとき

			// オブジェクトを NULL にする
			m_pObject = nullptr;

			if (m_searchState != SEARCHSTATE_NONE)
			{ // 何かしてたら

				// 何もしてない状態にする
				m_searchState = SEARCHSTATE_NONE;
			}
		}

		m_state = STATE_WHISTLE;	// 集合してる状態にする
	}

	//状態更新
	UpdateState();

	//向きの補正
	RotNormalize();

	//モーション管理
	MotionManager();

	//モーションの更新処理
	m_pMotion->Update();

	//デバッグ表示
	//pDebugProc->Print("\nルクミンの位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	/*pDebugProc->Print("ルクミンの移動量 (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("ルクミンの向き   (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);*/

	//pDebugProc->Print("目的の位置   (%f, %f, %f)\n", m_posDest.x, m_posDest.y, m_posDest.z);

}

//==============================================================
//状態更新処理
//==============================================================
void CLucmin::UpdateState(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// プレイヤーの情報取得
	CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();		// ポイントの情報取得

	switch (m_state)
	{
	case STATE_NONE:		// 何もしてない状態
		break;

	case STATE_FOLLOW:		// 追尾状態

		m_posDestSave = pPlayer->GetPos();		// 目的の位置

		// 追尾処理
		FollowMove(m_posDestSave);

		break;

	case STATE_THROW:		// 投げられてる状態

		if (m_throwState == THROWSTATE_NONE)
		{ // 何もしてない状態の場合

			m_throwState = THROWSTATE_STANDBY;		// 投げられ準備の状態にする
		}

		// 投げられ状態の更新
		UpdateThrowState();

		break;

	case STATE_SEARCH:		// 探す状態

		if (m_searchState == SEARCHSTATE_NONE)
		{ // 何もしてない状態の場合

			m_searchState = SEARCHSTATE_SEARCH;		// 探し状態にする
		}

		// 探し状態の更新
		UpdateSearchState();

		break;

	case STATE_ATTACK:		// 攻撃状態

		// 攻撃処理
		Attack();

		break;

	case STATE_WHISTLE:		// 呼び戻される状態

		if (m_whistleState == WHISTLESTATE_NONE)
		{ // 何もしてない状態だったら

			m_whistleState = WHISTLESTATE_REACTION;		// リアクション状態にする
		}

		// 集合状態の更新
		UpdateWhistleState();

		break;

	case STATE_DAMAGE:		// ダメージ状態
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
// 集合状態の更新処理
//==============================================================
void CLucmin::UpdateWhistleState(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// プレイヤーの情報取得

	switch (m_whistleState)
	{
	case CLucmin::WHISTLESTATE_NONE:		// 何もしてない状態
		break;

	case CLucmin::WHISTLESTATE_REACTION:	// リアクション状態

		if (m_nReactionCounter >= WHISTLE_REACTION_TIME)
		{
			m_whistleState = WHISTLESTATE_FOLLOW;

			m_nReactionCounter = 0;	// リアクション時間初期化
		}

		m_nReactionCounter++;		// リアクション時間加算

		break;

	case CLucmin::WHISTLESTATE_FOLLOW:		// 追尾状態

		m_posDestSave = pPlayer->GetPos();		// 目的の位置

		// 追尾処理
		FollowMove(m_posDestSave);

		break;

	default:

		// 停止する
		assert(false);

		break;
	}
}

//==============================================================
// 投げられ状態の更新処理
//==============================================================
void CLucmin::UpdateThrowState(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// プレイヤーの情報取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();
	D3DXVECTOR3 posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の位置
	float fMoveSpeedX = 0.12f;
	float fMoveSpeedZ = 0.12f;

	switch (m_throwState)
	{
	case CLucmin::THROWSTATE_NONE:		// 何もしてない状態

		break;

	case CLucmin::THROWSTATE_STANDBY:	// 準備状態

		// プレイヤーの位置から投げられるようにする
		m_pos.x = posPlayer.x;
		m_pos.z = posPlayer.z;

		// ルクミン投げられる高さ加算
		m_move.y += THROW_HEIGHT;

		// 目的の位置設定
		m_posDest.x = posPlayer.x - (posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * 100.0f);
		m_posDest.z = posPlayer.z - (posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * 100.0f);

		m_rotDest.y = atan2f(m_posDest.x, m_posDest.z);

		// 投げられる状態にする
		m_throwState = THROWSTATE_THROW;

		break;

	case CLucmin::THROWSTATE_THROW:		// 投げられる状態

		if (m_posDest.x < 0.0f)
		{
			fMoveSpeedX *= -1.0f;
		}

		if (m_posDest.z < 0.0f)
		{
			fMoveSpeedZ *= -1.0f;
		}


		// 移動量加算
		m_move.x = sinf(m_rotDest.y + D3DX_PI) * (MOVE + (m_posDest.x * fMoveSpeedX));
		m_move.z = cosf(m_rotDest.y + D3DX_PI) * (MOVE + (m_posDest.z * fMoveSpeedZ));

		// 重力
		m_move.y += GRAVITY;

		break;

	default:

		// 停止する
		assert(false);

		break;
	}


}

//==============================================================
// 探し状態の更新処理
//==============================================================
void CLucmin::UpdateSearchState(void)
{
	switch (m_searchState)
	{
	case CLucmin::SEARCHSTATE_NONE:		// 何もしてない状態
		break;

	case CLucmin::SEARCHSTATE_SEARCH:	// 探し状態

		// 探す処理
		Search();

		break;

	case CLucmin::SEARCHSTATE_FIND:		// 見つけ状態

		// 追尾処理
		FollowMove(m_posDestSave);

		break;

	default:

		// 停止する
		assert(false);

		break;
	}
}

//==============================================================
//モーション管理処理
//==============================================================
void CLucmin::MotionManager(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	//switch (m_MotionState)
	//{
	//case MOTIONSTATE_NONE:		//何もしてない(止まってる状態)

	//	break;

	//case MOTIONSTATE_MOVE:		//歩いてる状態

	//	break;

	//case MOTIONSTATE_JUMP:		//ジャンプしてる状態

	//	break;

	//case MOTIONSTATE_LAND:		//着地した状態

	//	break;

	//default:

	//	// 停止する
	//	assert(false);

	//	break;
	//}

	//if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	//{//歩いてる && 待機状態

	//	//歩かせる
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	//}
	//else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
	//	(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && 
	//		m_bMove == false && m_bJump == false && m_bLand == true))
	//{//モーションが終了したら

	//	//待機状態に戻す
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	//}
}

//==============================================================
//向きの補正処理
//==============================================================
void CLucmin::RotNormalize(void)
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
//画面外判定処理
//==============================================================
void CLucmin::Screen(void)
{
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;

		if (m_throwState == THROWSTATE_THROW)
		{ // 投げられ状態の時

			m_move.x = 0.0f;
			m_move.z = 0.0f;

			// 通常状態に戻す
			m_throwState = THROWSTATE_NONE;

			// 探す状態にする
			m_state = STATE_SEARCH;
		}
	}
}

//==============================================================
//ルクミンの描画処理
//==============================================================
void CLucmin::Draw(void)
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

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		//ルクミンの描画
		m_apModel[nCntEnemy]->Draw();
	}
}

//==============================================================
// 目標の位置についていく処理
//==============================================================
void CLucmin::FollowMove(D3DXVECTOR3 posDest)
{
	float fRotDest = 0.0f;

	//m_nIndex;

	// 目的の位置
	m_posDest.x = m_pos.x - posDest.x;
	m_posDest.z = m_pos.z - posDest.z;

	// 目的の向き
	m_fRotDest = atan2f(m_posDest.x, m_posDest.z);

	if (((m_pos.x - posDest.x) > MOVE_DISTANCE || (m_pos.x - posDest.x) < -MOVE_DISTANCE) ||
		((m_pos.z - posDest.z) > MOVE_DISTANCE || (m_pos.z - posDest.z) < -MOVE_DISTANCE))
	{ // 一定距離から離れたら

		if (m_state == STATE_WHISTLE)
		{ // 呼び戻し状態の時

			// 移動量加算
			m_move.x = sinf(m_fRotDest + D3DX_PI) * MOVE * 1.5f;
			m_move.z = cosf(m_fRotDest + D3DX_PI) * MOVE * 1.5f;
		}
		else
		{
			// 移動量加算
			m_move.x = sinf(m_fRotDest + D3DX_PI) * MOVE;
			m_move.z = cosf(m_fRotDest + D3DX_PI) * MOVE;
		}
		
	}
	else if (m_searchState == SEARCHSTATE_FIND &&
		(((m_pos.x - posDest.x) > m_fGimmickRadius || (m_pos.x - posDest.x) < -m_fGimmickRadius) ||
		((m_pos.z - posDest.z) > m_fGimmickRadius || (m_pos.z - posDest.z) < -m_fGimmickRadius)))
	{ // 見つけた状態だったら

		// 移動量加算
		m_move.x = sinf(m_fRotDest + D3DX_PI) * MOVE;
		m_move.z = cosf(m_fRotDest + D3DX_PI) * MOVE;


	}
	else if (m_searchState == SEARCHSTATE_FIND)
	{ // 探し状態 && 目的のオブジェクトに到達したら

		m_searchState = SEARCHSTATE_NONE;		// 何もしない状態にする
		m_state = STATE_ATTACK;					// 攻撃状態にする

	}
	else
	{ // 一定距離以内だったら

		if (m_state == STATE_WHISTLE)
		{ // 呼び戻し状態の時

			m_state = STATE_FOLLOW;		// 追尾状態にする
			m_whistleState = WHISTLESTATE_NONE;		// 何もない状態にする
		}

		
	}
}

//==============================================================
// 探す処理
//==============================================================
void CLucmin::Search(void)
{
	float fMinLength = 1000000.0f;		// 最短の長さ
	float fLength = 0.0f;			// 対象の長さ

	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		//オブジェクトを取得
		CObject* pObj = GetObject(nCntModel);

		if (pObj != nullptr)
		{//メモリが使用されているとき

			//種類を取得
			CObject::TYPE type = pObj->GetType();

			if (type == TYPE_GIMMICK)
			{//ギミックの時

				//モデルの位置取得
				D3DXVECTOR3 posModel = pObj->GetPos();
				D3DXVECTOR3 minModel = pObj->GetSizeMin();
				D3DXVECTOR3 maxModel = pObj->GetSizeMax();

				m_fGimmickRadius = maxModel.x * 2.0f;

				//当たり判定
				if (CircleCollision(posModel, m_pos, m_fGimmickRadius, GIMMICK_LENGTH * 0.5f) == true)
				{ // 円の当たり判定

					// ルクミンとギミックの距離を求める
					fLength = (posModel.x - m_pos.x) * (posModel.x - m_pos.x) + (posModel.z - m_pos.z) * (posModel.z - m_pos.z);

					if (fMinLength > fLength)
					{ // 距離が最短より小さい場合

						fMinLength = fLength;		// 最短距離更新

						// オブジェクトの情報保存
						m_pObject = pObj;

						// 目的地保存
						m_posDestSave = posModel;

						// 見つけた状態にする
						m_searchState = SEARCHSTATE_FIND;
					}
				}
			}
		}
	}
}

//==============================================================
// 攻撃処理
//==============================================================
void CLucmin::Attack(void)
{
	if (m_nAttackCounter >= ATTACK_INTERVAL)
	{ // 一定時間経ったら

		if (m_pObject != nullptr)
		{ // オブジェクトが NULL じゃなかったら

			// ヒット処理
			m_pObject->Hit();

			m_nAttackCounter = 0;	// カウンター初期化

		}
		else if(m_pObject == nullptr)
		{ // オブジェクトが NULL だったら

			if (m_state == STATE_ATTACK)
			{ // 攻撃してたら

				// 探している状態にする
				m_state = STATE_SEARCH;
			}

			m_nAttackCounter = 0;	// カウンター初期化
		}
	}
	else
	{ // 時間経ってなかったら

		// カウンター加算
		m_nAttackCounter++;
	}
}

//==============================================================
// 円の当たり判定
//==============================================================
bool CLucmin::CircleCollision(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, float fRadius0, float fRadius1)
{
	float fLength = 0.0f;

	fLength = (pos0.x - pos1.x) * (pos0.x - pos1.x) + (pos0.z - pos1.z) * (pos0.z - pos1.z);

	if (fLength <= (fRadius0 * fRadius1) + (fRadius0 * fRadius1))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//==============================================================
//モデルファイル読み込み処理
//==============================================================
void CLucmin::LoadFile(void)
{
	FILE *pFile;				//ファイルポインタ
	char aString[MAX_STR];		//文字読み込み
	int nIndex = 0, nParent = 0;	//パーツNo.,親番号
	D3DXVECTOR3 pos, rot;

	//ファイル開く
	pFile = fopen(FILE_ENEMY, "r");

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