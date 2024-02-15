//==============================================================
//
//プレイヤー処理[playerModel.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "playerModel.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "modelHierarchy.h"
#include "input.h"
#include "motion.h"
#include "camera.h"
#include "debugproc.h"

//マクロ定義
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define MOVE_Y				(1.0f)		//移動量Y
#define JUMP_HEIGHT			(15.0f)		//ジャンプの高さ
#define MAX_STR				(128)		//文字の最大数
#define PLAYER_MOVE			(0.5f)		//プレイヤーの移動量
#define FILE_HUMAN			"data\\TEXT\\motion_weapon.txt"		//プレイヤーモデルのテキスト

//静的メンバ変数宣言
bool CPlayerModel::m_bStart = false;		//実行してるかどうか
char *CPlayerModel::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\player\\00_body.x",
	"data\\MODEL\\player\\01_head.x",
	"data\\MODEL\\player\\02_hair.x",
	"data\\MODEL\\player\\03_LU_arm.x",
	"data\\MODEL\\player\\04_LD_arm.x",
	"data\\MODEL\\player\\05_L_hand.x",
	"data\\MODEL\\player\\06_RU_arm.x",
	"data\\MODEL\\player\\07_RD_arm.x",
	"data\\MODEL\\player\\08_R_hand.x",
	"data\\MODEL\\player\\09_waist.x",
	"data\\MODEL\\player\\10_skirt.x",
	"data\\MODEL\\player\\11_LU_leg.x",
	"data\\MODEL\\player\\12_LD_leg.x",
	"data\\MODEL\\player\\13_L_shoe.x",
	"data\\MODEL\\player\\14_RU_leg.x",
	"data\\MODEL\\player\\15_RD_leg.x",
	"data\\MODEL\\player\\16_R_shoe.x",
	"data\\MODEL\\player\\17_fish.x",
};

//==============================================================
//コンストラクタ
//==============================================================
CPlayerModel::CPlayerModel()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
	m_posKeepHuman = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//人間の位置保存用
	m_posKeepFish = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//金魚の位置保存用
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = NULL;		//プレイヤー(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//プレイヤー(パーツ)の総数
	m_bJump = false;		//ジャンプしたか
	m_pMotion = NULL;
	m_bMove = false;		//歩いてるか
	m_bLand = true;			//着地した
	m_bAction = false;		//攻撃してるかの判定
	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分
	m_Type = PLAYERTYPE_HUMAN;		//人間の状態にする
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CPlayerModel::CPlayerModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
	m_posKeepHuman = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//人間の位置保存用
	m_posKeepFish = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//金魚の位置保存用
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値
	m_rot = rot;		//向き

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = NULL;		//プレイヤー(パーツ)へのポインタ
	}

	m_nNumModel = 0;		//プレイヤー(パーツ)の総数
	m_bJump = false;		//ジャンプしたか
	m_pMotion = NULL;
	m_bMove = false;		//歩いてるか
	m_bLand = true;			//着地した
	m_bAction = false;		//攻撃してるかの判定
	m_fRotDest = 0.0f;	//目標
	m_fRotDiff = 0.0f;	//差分

	m_Type = PLAYERTYPE_HUMAN;		//人間の状態にする
}

//==============================================================
//デストラクタ
//==============================================================
CPlayerModel::~CPlayerModel()
{

}

//==============================================================
//プレイヤーの生成処理
//==============================================================
CPlayerModel *CPlayerModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayerModel *pPlayerModel = NULL;

	if (pPlayerModel == NULL)
	{//メモリが使用されてなかったら

		//プレイヤーの生成
		pPlayerModel = new CPlayerModel(pos, rot);
	}

	//初期化処理
	pPlayerModel->Init(pos, rot);

	//種類の設定
	pPlayerModel->SetType(CObject::TYPE_PLAYER);

	return pPlayerModel;
}

//==============================================================
//プレイヤーの初期化処理
//==============================================================
HRESULT CPlayerModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	//位置の設定
	m_pos = pos;

	//向きの設定
	m_rot = rot;

	m_posKeepFish = D3DXVECTOR3(0.0f, -5000.0f, 0.0f);		//金魚の初期位置設定

	//プレイヤーの生成（全パーツ分）
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//モーションの初期化・生成
	m_pMotion = m_pMotion->Create();
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//モデルのファイル読み込み
	CPlayerModel::LoadFile();

	//最大値・最小値代入
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//最大値Y
		if ((nCntPlayer <= PARTS_BODY && nCntPlayer <= PARTS_HEAD) ||
			(nCntPlayer <= PARTS_LEG_R && nCntPlayer <= PARTS_FOOT_R))
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

	return S_OK;
}

//==============================================================
//プレイヤーの終了処理
//==============================================================
void CPlayerModel::Uninit(void)
{
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		if (m_apModel[nCntPlayer] != NULL)
		{//使用されてるとき

			//終了処理
			m_apModel[nCntPlayer]->Uninit();
			m_apModel[nCntPlayer] = NULL;
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
//プレイヤーの更新処理
//==============================================================
void CPlayerModel::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetDebugProc();
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetCamera();		//カメラの情報取得

	if (pInputKeyboard->GetTrigger(DIK_F4) == true)
	{//F4が押されたとき

		//プレイヤーの種類入れ替え
		m_Type = m_Type ? PLAYERTYPE_HUMAN : PLAYERTYPE_GOLDFISH;

		if (m_Type == PLAYERTYPE_HUMAN)
		{//人間になったら

			m_posKeepFish = m_pos;		//金魚の位置保存しておく
			m_pos = m_posKeepHuman;		//地上の位置に戻る
			m_rot = D3DXVECTOR3(0.0f, 0.5f * -D3DX_PI, 0.0f);
			m_fRotDest = 0.5f * -D3DX_PI;
			pCamera->SetPositionVY(m_pos.y + 189.0f + sinf(m_rot.x) * -300.0f);		//カメラの位置設定
			pCamera->SetPositionRY(m_pos.y + 168.0f + sinf(m_rot.x) * -10.0f);		//カメラの位置設定

		}
		else if(m_Type == PLAYERTYPE_GOLDFISH)
		{//金魚になったら

			m_posKeepHuman = m_pos;		//人間の位置保存しておく
			m_pos = m_posKeepFish;		//水中の位置に戻る
			m_rot = D3DXVECTOR3(0.0f, 1.0f * D3DX_PI, 0.0f);
			m_fRotDest = 1.0f * D3DX_PI;
			pCamera->SetPositionVY(m_pos.y + 50.0f + sinf(m_rot.x) * 300.0f);		//カメラの位置設定
			pCamera->SetPositionRY(m_pos.y + 50.0f + sinf(m_rot.x) * 300.0f);		//カメラの位置設定
		}
	}

	//前回の位置更新
	m_posOld = m_pos;

	if (pInputKeyboard->GetTrigger(DIK_F5) == true)
	{//F5キーが押されたら

		m_bStart = m_bStart ? false : true;
	}

	if(m_bStart == true)
	{//実行してるとき

		switch (m_Type)
		{
		case PLAYERTYPE_HUMAN:		//人間

			//人間の更新処理
			CPlayerModel::UpdateHuman();

			pDebugProc->Print("\n-----[人間の姿]-----\n");
			pDebugProc->Print("\n～～[地上エリア]～～\n");

			break;

		case PLAYERTYPE_GOLDFISH:	//金魚

			//金魚の更新処理
			CPlayerModel::UpdataFish();

			pDebugProc->Print("\n-----[金魚の姿]-----\n");
			pDebugProc->Print("\n～～[水中エリア]～～\n");

			break;
		}
	}

	//モーションの更新処理
	m_pMotion->Update();

	//デバッグ表示
	pDebugProc->Print("\nプレイヤーの位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	/*pDebugProc->Print("プレイヤーの移動量 (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("プレイヤーの向き   (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);*/
}

//==============================================================
//人間の更新処理
//==============================================================
void CPlayerModel::UpdateHuman(void)
{
	//プレイヤーの操作
	CPlayerModel::ControlHuman();

	//移動量加算
	m_move.y -= MOVE_Y;

	//位置更新
	m_pos += m_move;

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	//当たり判定
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		CObjectX::Collision(&m_pos, &m_posOld, &m_move, m_min, m_max);
	}

	//向きの補正
	CPlayerModel::RotCorrection();

	//画面外処理
	CPlayerModel::Screen();

	if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	{//歩いてる && 待機状態

		//歩かせる
		m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	}
	else if (m_pMotion->IsFinish() == true && m_bAction == true)
	{//攻撃が終わったら

		m_bAction = false;		//攻撃してない状態にする
	}
	else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
		(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_bMove == false && m_bJump == false && m_bLand == true && m_bAction == false))
	{//モーションが終了したら

		//待機状態に戻す
		m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	}
}

//==============================================================
//金魚の更新処理
//==============================================================
void CPlayerModel::UpdataFish(void)
{
	//プレイヤーの操作
	CPlayerModel::ControlGoldFish();

	//位置更新
	m_pos += m_move;

	//移動量を更新
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	//当たり判定
	/*for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		CObjectX::Collision(&m_pos, &m_posOld, &m_move, m_apModel[nCntPlayer]->GetSizeMin(), m_apModel[nCntPlayer]->GetSizeMax());
	}*/

	//向きの補正
	CPlayerModel::RotCorrection();

	//画面外処理
	CPlayerModel::Screen();
}

//==============================================================
//向きの補正処理
//==============================================================
void CPlayerModel::RotCorrection(void)
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
//プレイヤーの操作処理(人間)
//==============================================================
void CPlayerModel::ControlHuman(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetCamera();		//カメラの情報取得

	//移動
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//右

#if _DEBUG

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//奥
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//手前
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.25f;

		}
		else
#endif
		{
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
		}

		m_bMove = true;		//歩かせる
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//左

#if _DEBUG

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//奥
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//手前
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.25f;

		}
		else
#endif
		{
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
		}

		m_bMove = true;		//歩かせる
	}

#if _DEBUG
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//奥
		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_UP) * PLAYER_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_UP) * PLAYER_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 1.0f;

		m_bMove = true;		//歩かせる

	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//手前
		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_DOWN) * PLAYER_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_DOWN) * PLAYER_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.0f;

		m_bMove = true;		//歩かせる

	}
#endif

	if ((m_move.x <= 0.8f && m_move.x >= -0.8f) && (m_move.z <= 0.8f && m_move.z >= -0.8f))
	{//歩いてないとき

		m_bMove = false;		//歩いてない状態にする
	}

	//ジャンプ
	if (pInputKeyboard->GetTrigger(DIK_SPACE) == true && m_bJump == false)
	{//SPACEキーを押してたら && ジャンプしてなかったら

		//ジャンプする
		m_move.y = JUMP_HEIGHT;

		//ジャンプした状態にする
		//m_bJump = true;
		//m_bLand = false;

		//モーションの設定
		//m_pMotion->Set(m_pMotion->MOTIONTYPE_JUMP);
	}
}

//==============================================================
//プレイヤーの操作処理(金魚)
//==============================================================
void CPlayerModel::ControlGoldFish(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();		//キーボードの情報取得
	CCamera *pCamera = CManager::GetCamera();		//カメラの情報取得

	//移動
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//右

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//奥
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * 0.25f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//手前
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * 0.75f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.25f;

		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
		}

		m_bMove = true;		//歩かせる
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//左

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//奥
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * -0.25f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//手前
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * -0.75f) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.25f;

		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * PLAYER_MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
		}

		m_bMove = true;		//歩かせる
	}

	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//奥
		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_UP) * PLAYER_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_UP) * PLAYER_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 1.0f;

		m_bMove = true;		//歩かせる

	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//手前
		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_DOWN) * PLAYER_MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_DOWN) * PLAYER_MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.0f;

		m_bMove = true;		//歩かせる

	}

	if ((m_move.x <= 0.8f && m_move.x >= -0.8f) && (m_move.z <= 0.8f && m_move.z >= -0.8f))
	{//歩いてないとき

		m_bMove = false;		//歩いてない状態にする
	}
}

//==============================================================
//画面外判定処理
//==============================================================
void CPlayerModel::Screen(void)
{
	//画面外判定(横)
	//if (m_pos.x < WIDTH_PLAYER)
	//{//画面左に出たら

	//	m_pos.x = WIDTH_PLAYER;

	//}
	//else if (m_pos.x > SCREEN_WIDTH - WIDTH_PLAYER)
	//{//画面右に出たら

	//	m_pos.x = SCREEN_WIDTH - WIDTH_PLAYER;

	//}

	//画面外判定(縦)
	//if (m_pos.y > HEIGHT_PLAYER)
	//{//画面上に出たら

	//	m_pos.y = HEIGHT_PLAYER;
	//}
	/*else */
	if (m_pos.y < 0.0f && m_Type == PLAYERTYPE_HUMAN)
	{//画面下に出たら

		if (m_bJump == true)
		{//ジャンプしてたら

			//着地
			m_pMotion->Set(m_pMotion->MOTIONTYPE_LAND);
		}

		if (m_pMotion->IsFinish() == true)
		{//終了したら

			m_bLand = true;		//着地した
		}

		m_pos.y = 0.0f;
		m_move.y = 0.0f;

		m_bJump = false;
	}
}

//==============================================================
//プレイヤーの描画処理
//==============================================================
void CPlayerModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
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
//モデルファイル読み込み処理
//==============================================================
void CPlayerModel::LoadFile(void)
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

							m_apModel[nIndex]->SetPosition(pos);		//位置設定
							m_apModel[nIndex]->SetDefaultPosition(pos);	//初期位置設定

						}
						else if (strcmp(&aString[0], "ROT") == 0)
						{//向き情報

							fscanf(pFile, "%s", &aString[0]);	//文字読み込み

							fscanf(pFile, "%f", &rot.x);		//向き読み込み
							fscanf(pFile, "%f", &rot.y);		//向き読み込み
							fscanf(pFile, "%f", &rot.z);		//向き読み込み

							m_apModel[nIndex]->SetRotation(rot);		//向き設定
							m_apModel[nIndex]->SetDefaultRotation(rot);	//初期向き設定
						}
					}
				}
			}
		}

		//ファイル閉じる
		fclose(pFile);
	}
}