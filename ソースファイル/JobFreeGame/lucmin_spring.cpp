//==============================================================
//
//春ルクミンの処理[lucmin_spring.h]
//Author:佐藤根詩音
//
//==============================================================
#include "lucmin_spring.h"
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
#define FILE_ENEMY			"data\\TXT\\motion_player.txt"		//春ルクミンモデルのテキスト

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間

//静的メンバ変数宣言
int CLucminSpring::m_nNumAll = 0;						//春ルクミンの総数
char *CLucminSpring::m_apFileName[PARTS_MAX] =
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
CLucminSpring::CLucminSpring()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最小値

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//春ルクミン(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//春ルクミン(パーツ)の総数
	m_pMotion = NULL;

	m_fRotDest = 0.0f;		//目標
	m_fRotDiff = 0.0f;		//差分

	m_nCntDamage = 0;				//ダメージカウンター

	m_nIndex = m_nNumAll;

	m_nNumAll++;

}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CLucminSpring::CLucminSpring(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;									// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 移動量
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// モデルの最小値
	m_rot = rot;		//向き
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 目的の向き

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//春ルクミン(パーツ)へのポインタ
	}

	m_pMotion = NULL;		//モーションの情報
	m_nNumModel = 0;		//春ルクミン(パーツ)の総数

	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分

	m_nCntDamage = 0;			//ダメージカウンター

	m_nIndex = m_nNumAll;

	m_nNumAll++;
}

//==============================================================
//デストラクタ
//==============================================================
CLucminSpring::~CLucminSpring()
{
	m_nNumAll--;
}

//==============================================================
//春ルクミンの生成処理
//==============================================================
CLucminSpring *CLucminSpring::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLucminSpring *pEnemyModel = NULL;

	if (pEnemyModel == NULL)
	{//メモリが使用されてなかったら

		//春ルクミンの生成
		pEnemyModel = new CLucminSpring(pos, rot);
	}

	//初期化処理
	pEnemyModel->Init();

	return pEnemyModel;
}

//==============================================================
//春ルクミンの初期化処理
//==============================================================
HRESULT CLucminSpring::Init(void)
{
	m_fRotDest = m_rot.y;

	//春ルクミンの生成（全パーツ分）
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//モーションの初期化・生成
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//モデルのファイル読み込み
	CLucminSpring::LoadFile();

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
//春ルクミンの終了処理
//==============================================================
void CLucminSpring::Uninit(void)
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
//春ルクミンの更新処理
//==============================================================
void CLucminSpring::Update(void)
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

	// 慣性付与
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	/*if (pPoint->CircleCollision(pPoint->GetPos(), m_pos, 1000.0f, 50.0f) == true &&
		pPoint->GetState() == CPoint::STATE_WHISTLE &&
		m_state != STATE_WHISTLE && m_state != STATE_FOLLOW && m_state != STATE_THROW)
	{
		m_state = STATE_WHISTLE;
	}*/

	//モーション管理
	MotionManager();

	//モーションの更新処理
	m_pMotion->Update();

	//状態設定
	/*for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy]->SetState(m_state);

	}*/

	//デバッグ表示
	pDebugProc->Print("\n春ルクミンの位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("春ルクミンの移動量 (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("春ルクミンの向き   (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);
}

//==============================================================
//モーション管理処理
//==============================================================
void CLucminSpring::MotionManager(void)
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
//春ルクミンの描画処理
//==============================================================
void CLucminSpring::Draw(void)
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
		//春ルクミンの描画
		m_apModel[nCntEnemy]->Draw();
	}
}

//==============================================================
//モデルファイル読み込み処理
//==============================================================
void CLucminSpring::LoadFile(void)
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