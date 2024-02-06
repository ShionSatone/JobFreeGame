//==============================================================
//
//敵の処理[enemy.h]
//Author:佐藤根詩音
//
//==============================================================
#include "enemy.h"
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
//#include "effect.h"

//マクロ定義
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define MOVE_Y				(0.7f)		//移動量Y
#define ADD_MOVE_Y			(1.5f)		//移動量Y加算する数
#define JUMP_HEIGHT			(10.0f)		//ジャンプの高さ
#define MAX_STR				(128)		//文字の最大数
#define FRONT_MOVE			(0.6f)		//手前の時の移動量
#define FRONT_DASH_MOVE		(15.0f)		//手前のダッシュ時の移動量
#define MAX_DASH			(2)			//ダッシュの最大数
#define STOP_MOVE			(0.8f)		//止まる判定の移動量
#define FILE_ENEMY			"data\\TXT\\motion_player.txt"		//敵モデルのテキスト

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間
#define STEP_CNT			(25)		//歩く音のカウンター
#define ACTION_FRAME		(50)		//2体目以降の敵の行動フレーム

#define MOVE				(1.0f)		// 移動量

//静的メンバ変数宣言
int CEnemy::m_nNumAll = 0;						//敵の総数
char *CEnemy::m_apFileName[PARTS_MAX] =
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
CEnemy::CEnemy()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//敵(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//敵(パーツ)の総数
	m_pMotion = NULL;

	m_fRotDest = 0.0f;		//目標
	m_fRotDiff = 0.0f;		//差分

	m_nFrameCounter = 0;	//フレーム数カウンター

	m_state = STATE_NONE;			//状態
	m_enemyState = ENEMYSTATE_NONE;	//止まってる状態
	m_nCntDamage = 0;				//ダメージカウンター

	m_nNum = m_nNumAll;

	m_nNumAll++;

}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CEnemy::CEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;									//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//前回の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//移動量
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値
	m_rot = rot;		//向き

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//敵(パーツ)へのポインタ
	}

	m_pMotion = NULL;		//モーションの情報
	m_nNumModel = 0;		//敵(パーツ)の総数

	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分

	m_nFrameCounter = 0;	//フレーム数カウンター

	m_state = STATE_NONE;		//状態
	m_enemyState = ENEMYSTATE_NONE;	//止まってる状態
	m_nCntDamage = 0;			//ダメージカウンター

	m_nNum = m_nNumAll;

	m_nNumAll++;
}

//==============================================================
//デストラクタ
//==============================================================
CEnemy::~CEnemy()
{
	m_nNumAll--;
}

//==============================================================
//敵の生成処理
//==============================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEnemy *pEnemyModel = NULL;

	if (pEnemyModel == NULL)
	{//メモリが使用されてなかったら

		//敵の生成
		pEnemyModel = new CEnemy(pos, rot);
	}

	//初期化処理
	pEnemyModel->Init();

	return pEnemyModel;
}

//==============================================================
//敵の初期化処理
//==============================================================
HRESULT CEnemy::Init(void)
{
	m_fRotDest = m_rot.y;

	//敵の生成（全パーツ分）
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//モーションの初期化・生成
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//モデルのファイル読み込み
	CEnemy::LoadFile();

	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		m_apModel[nCnt]->SetState(CObjectX::STATE_DAMAGE);		//ダメージ状態にする
	}

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
//敵の終了処理
//==============================================================
void CEnemy::Uninit(void)
{
	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		if (m_apModel[nCntEnemy] != NULL)
		{//使用されてるとき

			//終了処理
			m_apModel[nCntEnemy]->Uninit();
			m_apModel[nCntEnemy] = NULL;
		}
	}

	if (m_pMotion != NULL)
	{//使用されてるとき

		//モーションの破棄
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//敵の更新処理
//==============================================================
void CEnemy::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	//前回の位置更新
	m_posOld = m_pos;

	//手前側の更新処理
	CEnemy::UpdateFront();

	//モーションの更新処理
	m_pMotion->Update();

	//状態設定
	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy]->SetState(m_state);

	}

	//デバッグ表示
	pDebugProc->Print("\n敵の位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("敵の移動量 (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("敵の向き   (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);
}

//==============================================================
//手前の更新処理
//==============================================================
void CEnemy::UpdateFront(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CPlayer *pPlayer = CGame::GetPlayer();		//プレイヤーの情報取得

	//前回の位置更新
	m_posOld = m_pos;

	// 移動処理
	Move();

	// 位置加算
	m_pos += m_move;

	//当たり判定
	CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//状態更新
	CEnemy::UpdateState();

	//向きの補正
	CEnemy::RotCorrection();

	//モーション管理
	CEnemy::MotionManager();
}

//==============================================================
// 移動処理
//==============================================================
void CEnemy::Move(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// プレイヤーの情報取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();
	float fRotDest = 0.0f;

	// 目的の位置
	m_posDest.x = m_pos.x - posPlayer.x;
	m_posDest.z = m_pos.z - posPlayer.z;

	// 目的の向き
	m_fRotDest = atan2f(m_posDest.x, m_posDest.z);

	if (((m_pos.x - posPlayer.x) > 1.0f || (m_pos.x - posPlayer.x) < -1.0f) ||
		((m_pos.z - posPlayer.z) > 1.0f || (m_pos.z - posPlayer.z) < -1.0f))
	{ // 一定距離から離れたら

		// 移動量加算
		m_move.x = sinf(m_fRotDest + D3DX_PI) * MOVE;
		m_move.z = cosf(m_fRotDest + D3DX_PI) * MOVE;

	}
}

//==============================================================
//状態更新処理
//==============================================================
void CEnemy::UpdateState(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得

	switch (m_enemyState)
	{
	case ENEMYSTATE_NONE:		//何もしてない(止まってる状態)


		break;

	case ENEMYSTATE_MOVE:		//歩いてる状態

		break;

	case ENEMYSTATE_JUMP:		//ジャンプしてる状態

		break;

	case ENEMYSTATE_LAND:		//着地した状態

		break;

	case ENEMYSTATE_DASH:		//ダッシュ状態

		break;
	}
}

//==============================================================
//モーション管理処理
//==============================================================
void CEnemy::MotionManager(void)
{
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
void CEnemy::RotCorrection(void)
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
void CEnemy::Screen(void)
{
	//if (m_pos.y < 0.0f)
	//{//画面下に出たら

	//	m_move.y = 0.0f;
	//	m_pos.y = 0.0f;

	//	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//	if (pInputKeyboard->GetPress(DIK_SPACE) == false)
	//	{
	//		m_bLand = true;		//着地した
	//		m_bJump = false;	//ジャンプしてない
	//	}

	//	m_nDashCounter = 0;		//ダッシュ数リセット

	//	//m_state = STATE_NONE;		//通常状態にする
	//}
}

//==============================================================
//敵の描画処理
//==============================================================
void CEnemy::Draw(void)
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
		//敵の描画
		m_apModel[nCntEnemy]->Draw();
	}
}

//==============================================================
//モデルファイル読み込み処理
//==============================================================
void CEnemy::LoadFile(void)
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