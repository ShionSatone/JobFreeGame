//==============================================================
//
//カメラ処理[camera.h]
//Author:佐藤根詩音
//
//==============================================================
#include "camera.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define MAX_CAMERA			(1)			//カメラの最大数
#define CAMERA_LEN			(10.0f)		//距離
#define CAMERA_PLEN			(0.0f)		//カメラの視点と注視点の距離
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define LENGRH				(1000.0f)	//視点と注視点の長さ
#define HEIGHT				(LENGRH - 100.0f)	//視点の高さ
#define FOLLOW				(0.5f)		//カメラの移動量

//==============================================================
//コンストラクタ
//==============================================================
CCamera::CCamera()
{
	m_posV = D3DXVECTOR3(0.0f, 189.0f, 0.0f);	//視点の初期化
	m_posR = D3DXVECTOR3(0.0f, 168.0f, 0.0f);	//注視点の初期化
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向ベクトルの初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の視点の初期化
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の注視点の初期化
	m_moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点の移動量の初期化
	m_moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点の移動量の初期化
	m_fRotMove = 0.0f;								//現在の方向(角度)
	m_fRotDest = 0.0f;								//目標の方向(角度)
	m_fRotDiff = 0.0f;								//目標の方向までの差分
	//m_fLength = LENGRH;								//長さ
	m_bProject = true;			//透視投影の状態にする
}

//==============================================================
//デストラクタ
//==============================================================
CCamera::~CCamera()
{

}

//==============================================================
//カメラの初期化処理
//==============================================================
HRESULT CCamera::Init(void)
{
	m_posV = D3DXVECTOR3(0.0f, 189.0f, 0.0f);	//視点の初期化
	m_posR = D3DXVECTOR3(0.0f, 168.0f, 0.0f);	//注視点の初期化
	
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);			//上方向ベクトルの初期化
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向きの初期化
	m_posVDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の視点の初期化
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//目的の注視点の初期化
	m_moveV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//視点の移動量の初期化
	m_moveR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//注視点の移動量の初期化
	m_fRotMove = 0.0f;								//現在の方向(角度)
	m_fRotDest = 0.0f;								//目標の方向(角度)
	m_fRotDiff = 0.0f;								//目標の方向までの差分
	//m_fLength = LENGRH;								//長さ
	m_bProject = true;			//透視投影の状態にする

	//初期地点の初期化
	m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * LENGRH;
	m_posV.y = (m_posR.y + CAMERA_PLEN) + sinf(m_rot.x) * LENGRH;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * LENGRH;

	m_posR.x = m_posV.x + sinf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;
	m_posR.y = (m_posV.y - CAMERA_PLEN) + sinf(m_rot.x) * -LENGRH;
	m_posR.z = m_posV.z + cosf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;

	return S_OK;
}

//==============================================================
//カメラの終了処理
//==============================================================
void CCamera::Uninit(void)
{
	
}

//==============================================================
//カメラの更新処理
//==============================================================
void CCamera::Update(void)
{
	//CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();		//デバッグ表示の情報取得

	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{//ゲーム画面のとき

		//カメラの操作
		CCamera::Control();

	}
	//else
	//{//それ以外(リザルト)のとき

	//	m_posV = D3DXVECTOR3(-4680, 1271.0f, 300.0f);	//視点の初期化
	//	m_posR = D3DXVECTOR3(0.0f, 1250.0f, 300.0f);	//注視点の初期化
	//}

	//移動処理
	m_posV.x = m_posR.x + sinf(m_rot.y) * -cosf(m_rot.x) * LENGRH;
	m_posV.y = m_posR.y + sinf(m_rot.x) * LENGRH;
	m_posV.z = m_posR.z + cosf(m_rot.y) * -cosf(m_rot.x) * LENGRH;

	m_posR.x = m_posV.x + sinf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;
	m_posR.y = m_posV.y + sinf(m_rot.x) * -LENGRH;
	m_posR.z = m_posV.z + cosf(m_rot.y) * -cosf(m_rot.x) * -LENGRH;

	//カメラの回り込み
	/*if (pPlayer->move.x <= 0.0001f && pPlayer->move.x >= -0.0001f && pPlayer->move.z <= 0.0001f && pPlayer->move.z >= -0.0001f)
	{
		m_fRotDest = pPlayer->rot.y + D3DX_PI;
	}*/

	//向きの補正
	CCamera::RotCorrection();

	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{//ゲーム画面のとき

		//自動追従の慣性
		m_posR.x += (m_posRDest.x - m_posR.x) * FOLLOW;
		m_posV.x += (m_posVDest.x - m_posV.x) * FOLLOW;
		m_posR.y += (m_posRDest.y - m_posR.y) * FOLLOW;
		m_posV.y += (m_posVDest.y - m_posV.y) * FOLLOW;
		m_posR.z += (m_posRDest.z - m_posR.z) * FOLLOW;
		m_posV.z += (m_posVDest.z - m_posV.z) * FOLLOW;
	}

	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	pDebugProc->Print("[視点]  (%f,%f,%f)\n", m_posV.x, m_posV.y, m_posV.z);
	pDebugProc->Print("[注視点](%f,%f,%f)\n", m_posR.x, m_posR.y, m_posR.z);
}

//==============================================================
//向きの補正処理
//==============================================================
void CCamera::RotCorrection(void)
{
	//差分を計算
	m_fRotDiff = m_fRotDest - m_fRotMove;

	//目標の方向までの差分を修正
	if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * 2;
	}
	else if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff -= D3DX_PI * 2;
	}

	//現在の移動方向に差分を足す
	m_fRotMove += m_fRotDiff * 0.1f;

	//現在の方向修正
	if (m_fRotMove > D3DX_PI)
	{
		m_fRotMove -= D3DX_PI * 2;
	}
	else if (m_fRotMove < -D3DX_PI)
	{
		m_fRotMove += D3DX_PI * 2;
	}
}

//==============================================================
//カメラの操作
//==============================================================
void CCamera::Control(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	//CPlayer *pPlayer = CGame::GetPlayer();			//プレイヤーの情報取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();

#if 1

	//カメラ
	if (pInputKeyboard->GetPress(DIK_A) == true)
	{//カメラ左
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//カメラ左上
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.25f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.25f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.25f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//カメラ左下
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.75f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * 0.75f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.75f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * 0.75f);
		}
		else
		{
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * CURVE_RL);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + -D3DX_PI * CURVE_RL);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * CURVE_RL);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + -D3DX_PI * CURVE_RL);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_D) == true)
	{//カメラ右
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//カメラ左上
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.25f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.25f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.25f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.25f);
		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//カメラ左下
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.75f);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * 0.75f);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.75f);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * 0.75f);
		}
		else
		{
			m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_RL);
			m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_RL);
			m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_RL);
			m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_RL);
		}
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//カメラ上
		m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_UP);
		m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_UP);
		m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_UP);
		m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_UP);
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//カメラ下
		m_posR.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_DOWN);
		m_posV.x += CAMERA_LEN * sinf(m_rot.y + D3DX_PI * CURVE_DOWN);
		m_posR.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_DOWN);
		m_posV.z += CAMERA_LEN * cosf(m_rot.y + D3DX_PI * CURVE_DOWN);
	}
#endif

	//目的の注視点を設定
	m_posRDest.x = pPlayer->GetPos().x + sinf(pPlayer->GetRot().y + D3DX_PI) * 0.0f;
	m_posRDest.y = pPlayer->GetPos().y;
	m_posRDest.z = pPlayer->GetPos().z + cosf(pPlayer->GetRot().y + D3DX_PI) * 0.0f;

	//目的の視点を設定
	m_posVDest.x = pPlayer->GetPos().x + sinf(GetRotation().y + D3DX_PI) * LENGRH;
	m_posVDest.y = pPlayer->GetPos().y + HEIGHT;
	m_posVDest.z = pPlayer->GetPos().z + cosf(GetRotation().y + D3DX_PI) * LENGRH;

#if _DEBUG
	//視点
	if (pInputKeyboard->GetPress(DIK_C) == true)
	{//視点右
		m_rot.y -= 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_Z) == true)
	{//視点左
		m_rot.y += 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_Y) == true)
	{//視点上
		m_rot.x += 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_N) == true)
	{//視点下
		m_rot.x -= 0.01f;
	}

	//注視点
	if (pInputKeyboard->GetPress(DIK_Q) == true)
	{//注視点右
		m_rot.y -= 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_E) == true)
	{//注視点左
		m_rot.y += 0.05f;
	}
	if (pInputKeyboard->GetPress(DIK_T) == true)
	{//視点上
		m_rot.x -= 0.01f;
	}
	if (pInputKeyboard->GetPress(DIK_B) == true)
	{//視点下
		m_rot.x += 0.01f;
	}
#endif
	
#if _DEBUG

	//if (pInputKeyboard->GetTrigger(DIK_F3) == true)
	//{//F3キーが押されたら

	//	//投影切り替え
	//	m_bProject = m_bProject ? false : true;
	//}

#endif
}

//==============================================================
//カメラの設定処理
//==============================================================
void CCamera::SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	//CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	//if (m_bProject == false)
	{
		//プロジェクションマトリックスを作成(透視投影)
		D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
			D3DXToRadian(45.0f),
			(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
			10.0f, 7000.0f);
	}
	//else if (m_bProject == true)
	//{
	//	//プロジェクションマトリックスを作成(並行投影)
	//	D3DXMatrixOrthoLH(&m_mtxProjection,		//プロジェクションマトリックス
	//		(float)SCREEN_WIDTH * 1.2f/* * 0.5f*/,			//画面の幅
	//		(float)SCREEN_HEIGHT * 1.2f/* * 0.5f*/,		//画面の高さ
	//		-1000.0f,					//Z値の最小値
	//		7000.0f);					//Z値の最大値
	//}

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	//ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView, &m_posV, &m_posR, &m_vecU);

	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);
}

////==============================================================
////分割カメラの設定処理
////==============================================================
////void SetCamera(int nIdex)
////{
////	LPDIRECT3DDEVICE9 pDevice = GetDevice();		//デバイスへのポインタ
////
////	//ビューポートの設定
////	pDevice->SetViewport(&g_camera[nIdex].viewport);
////
////	//プロジェクションマトリックスの初期化
////	D3DXMatrixIdentity(&g_camera[nIdex].mtxProjection);
////
////	//プロジェクションマトリックスを作成
////	D3DXMatrixPerspectiveFovLH(&g_camera[nIdex].mtxProjection,
////		D3DXToRadian(45.0f),
////		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,
////		10.0f, 5000.0f);
////
////	//プロジェクションマトリックスの設定
////	pDevice->SetTransform(D3DTS_PROJECTION, &g_camera[nIdex].mtxProjection);
////
////	//ビューマトリックスの初期化
////	D3DXMatrixIdentity(&g_camera[nIdex].mtxView);
////
////	//ビューマトリックスの作成
////	D3DXMatrixLookAtLH(&g_camera[nIdex].mtxView, &g_camera[nIdex].posV, &g_camera[nIdex].posR, &g_camera[nIdex].vecU);
////
////	//ビューマトリックスの設定
////	pDevice->SetTransform(D3DTS_VIEW, &g_camera[nIdex].mtxView);
////}
//
////==============================================================
////カメラの取得
////==============================================================
//Camera *GetCamera(void)
//{
//	return &g_camera;		//カメラの情報の先頭アドレスを渡す
//}