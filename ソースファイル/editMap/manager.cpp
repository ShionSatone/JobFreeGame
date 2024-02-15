//==============================================================
//
//マネージャ処理[manager.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "manager.h"
#include "object.h"
#include "renderer.h"
#include "debugproc.h"
#include "input.h"
#include "object3D.h"
#include "camera.h"
#include "light.h"
#include "field.h"
#include "objectX.h"
#include "model.h"
#include "texture.h"
#include "edit.h"
#include "playerModel.h"
#include "material.h"

//静的メンバ変数宣言
CRenderer *CManager::m_pRenderer = NULL;		//レンダラーの情報
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	//キーボードの情報
CInputJoyPad *CManager::m_pInputJoyPad = NULL;		//ゲームパッドの情報

CDebugProc *CManager::m_pDebugProc = NULL;		//デバッグ表示の情報

CCamera *CManager::m_pCamera = NULL;			//カメラの情報
CLight *CManager::m_pLight = NULL;				//ライトの情報
CTexture *CManager::m_pTexture = NULL;			//テクスチャの情報
CEdit *CManager::m_pEdit = NULL;				//モデル配置の情報
CPlayerModel *CManager::m_pPlayerModel = NULL;	//プレイヤーモデル配置の情報
CMaterial *CManager::m_pMaterial = NULL;		//マテリアルの情報

//==============================================================
//コンストラクタ
//==============================================================
CManager::CManager()
{
	m_hInstance = NULL;
	m_hWnd = NULL;
	m_bWindow = false;
}

//==============================================================
//デストラクタ
//==============================================================
CManager::~CManager()
{

}

//==============================================================
//初期化処理
//==============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	m_hInstance = hInstance;
	m_hWnd = hWnd;
	m_bWindow = bWindow;

	//レンダラーの生成
	if (m_pRenderer == NULL)
	{//メモリが使用されてないとき

		m_pRenderer = new CRenderer;
	}

	//レンダラーの初期化処理
	if (m_pRenderer != NULL)
	{//メモリ確保できてたら

		//レンダラーの初期化
		if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//デバッグ表示の生成
	if (m_pDebugProc == NULL)
	{//メモリが使用されてないとき

		m_pDebugProc = new CDebugProc;
	}

	if (m_pDebugProc != NULL)
	{//メモリが確保されたとき

		//デバッグ表示の初期化処理
		m_pDebugProc->Init();
	}

	//キーボードの生成
	if (m_pInputKeyboard == NULL)
	{//キーボードが使用されてないとき

		m_pInputKeyboard = new CInputKeyboard;
	}

	if (m_pInputKeyboard != NULL)
	{//メモリ確保できてたら

		//キーボードの初期化
		if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//ゲームパッドの生成
	if (m_pInputJoyPad == NULL)
	{
		m_pInputJoyPad = new CInputJoyPad;
	}

	if (m_pInputJoyPad != NULL)
	{//メモリ確保できてたら

		//パッドの初期化
		if (FAILED(m_pInputJoyPad->Init(hInstance, hWnd)))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//カメラの生成
	if (m_pCamera == NULL)
	{
		m_pCamera = new CCamera;
	}

	if (m_pCamera != NULL)
	{//メモリ確保できてたら

		//カメラの初期化
		if (FAILED(m_pCamera->Init()))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//ライトの生成
	if (m_pLight == NULL)
	{
		m_pLight = new CLight;
	}

	if (m_pLight != NULL)
	{//メモリ確保できてたら

		//ライトの初期化
		if (FAILED(m_pLight->Init()))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//テクスチャの生成
	if (m_pTexture == NULL)
	{
		m_pTexture = new CTexture;
	}

	if (m_pTexture != NULL)
	{//メモリ確保できてたら

		//テクスチャの初期化
		if (FAILED(m_pTexture->Load()))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//マテリアルの生成
	if (m_pMaterial == NULL)
	{
		m_pMaterial = new CMaterial;
	}

	if (m_pMaterial != NULL)
	{//メモリ確保できてたら

		//マテリアルの初期化
		if (FAILED(m_pMaterial->Load()))
		{//初期化処理が失敗した場合

			return E_FAIL;
		}
	}

	//全てのテクスチャ3Dの読み込み
	CManager::Load3DAll();

	float posX = -5400.0f;
	float posY = 1000.0f;

	//プレイヤーの生成
	m_pPlayerModel = CPlayerModel::Create(D3DXVECTOR3(posX, posY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//全てのオブジェクト3Dの生成
	CManager::Create3DAll();

	//エディットの生成
	CEdit::Create(D3DXVECTOR3(posX, posY, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	return S_OK;
}

//==============================================================
//オブジェクト3Dの生成処理
//==============================================================
void CManager::Create3DAll(void)
{
	
}

//==============================================================
//終了処理
//==============================================================
void CManager::Uninit(void)
{
	//全てのオブジェクトの破棄
	CObject::ReleaseAll();

	//全てのテクスチャ3Dの破棄
	CManager::UnLoad3DAll();

	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		//テクスチャの破棄
		m_pTexture->Unload();

		//メモリ開放
		delete m_pTexture;
		m_pTexture = NULL;
	}

	//マテリアルの破棄
	if (m_pMaterial != NULL)
	{
		//テクスチャの破棄
		m_pMaterial->Unload();

		//メモリ開放
		delete m_pMaterial;
		m_pMaterial = NULL;
	}

	//キーボードの破棄
	if (m_pInputKeyboard != NULL)
	{
		//キーボードの終了処理
		m_pInputKeyboard->Uninit();

		//メモリ開放
		delete m_pInputKeyboard;
		m_pInputKeyboard = NULL;
	}

	//ゲームパッドの破棄
	if (m_pInputJoyPad != NULL)
	{
		//ゲームパッドの終了処理
		m_pInputJoyPad->Uninit();

		//メモリ開放
		delete m_pInputJoyPad;
		m_pInputJoyPad = NULL;
	}

	//ライトの破棄
	if (m_pLight != NULL)
	{
		//ライトの終了処理
		m_pLight->Uninit();

		//メモリ開放
		delete m_pLight;
		m_pLight = NULL;
	}

	//カメラの破棄
	if (m_pCamera != NULL)
	{
		//カメラの終了処理
		m_pCamera->Uninit();

		//メモリ開放
		delete m_pCamera;
		m_pCamera = NULL;
	}

	//デバッグ表示の破棄
	if (m_pDebugProc != NULL)
	{
		//デバッグ表示の終了処理
		m_pDebugProc->Uninit();

		//メモリ開放
		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}

	//レンダラーの破棄
	if (m_pRenderer != NULL)
	{
		//レンダラーの終了処理
		m_pRenderer->Uninit();

		//メモリ開放
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
}

//==============================================================
//全てのテクスチャ3Dの読み込み
//==============================================================
void CManager::Load3DAll(void)
{
	//モデルの読み込み
	//CModel::Load();

	//敵の読み込み
	//CEnemyModel::Load();
}

//==============================================================
//全てのテクスチャ3Dの破棄
//==============================================================
void CManager::UnLoad3DAll(void)
{
	//モデルの破棄
	//CModel::Unload();

	//敵の破棄
	//CEnemyModel::Unload();
}

//==============================================================
//更新処理
//==============================================================
void CManager::Update(void)
{
	//キーボードの更新処理
	m_pInputKeyboard->Update();

	//ゲームパッドの更新処理
	m_pInputJoyPad->Update();

	//デバッグ表示の更新処理
	m_pDebugProc->Update();

	//マップの再生成
	CManager::ResetMap();

	//レンダラーの更新処理
	m_pRenderer->Update();

	//カメラの更新処理
	m_pCamera->Update();
}

//==============================================================
//描画処理
//==============================================================
void CManager::Draw(void)
{
	//レンダラーの描画処理
	m_pRenderer->Draw();
}

//==============================================================
//マップの配置リセット
//==============================================================
void CManager::ResetMap(void)
{
	//リセット
	//if (m_pInputKeyboard->GetPress(DIK_F2) == true)
	//{//F2を押したとき

	//	//マップのオブジェクトのリセット
	//	CObject::ResetMapAll();

	//	////オブジェクト3Dの生成
	//	CManager::Create3DAll();
	//}
}