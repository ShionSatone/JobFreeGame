//==============================================================
//
//ゴール処理[goal.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "goal.h"
#include "manager.h"
#include "renderer.h"
#include "material.h"
#include "player.h"
#include "game.h"
#include "sound.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define NUM_MODEL			(2)			//ゴールの種類
#define POS					(10.0f)		//pos初期値
#define MODEL_LEN			(3.0f)		//距離
#define MOVE_DOWN			(-10.0f)		//落下移動量

//**************************************************************
//静的メンバ変数宣言
//**************************************************************
LPD3DXMESH CGoal::m_pMesh = NULL;						//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER CGoal::m_pBuffMat = NULL;					//マテリアルへのポインタ
DWORD CGoal::m_dwNumMat = NULL;
const char* CGoal::m_pFilename = NULL;					//ファイルの名前

//==============================================================
//コンストラクタ
//==============================================================
CGoal::CGoal()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//前回の位置
	m_move = D3DXVECTOR3(0.0f, MOVE_DOWN, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き

	m_bActionDown = false;		//ブロックを落とすか
	m_bDown = false;			//落とされてるか
	m_bCollision = false;		//他のブロックと当たってるか
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CGoal::CGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//位置
	m_posOld = m_pos;	//前回の位置
	m_move = D3DXVECTOR3(0.0f, MOVE_DOWN, 0.0f);		//移動量
	m_rot = rot;		//向き

	m_bActionDown = false;		//ブロックを落とすか
	m_bDown = false;			//落とされてるか
	m_bCollision = false;		//他のブロックと当たってるか

}

//==============================================================
//デストラクタ
//==============================================================
CGoal::~CGoal()
{

}

//==============================================================
//Xファイルの読み込み
//==============================================================
HRESULT CGoal::Load(const char* pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	return S_OK;
}

//==============================================================
//マテリアル・メッシュの破棄
//==============================================================
void CGoal::Unload(void)
{
	//メッシュの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//マテリアルの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//==============================================================
//ゴールの生成処理
//==============================================================
CGoal* CGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename)
{
	CGoal* pGoal = NULL;

	if (pGoal == NULL)
	{//メモリが使用されてなかったら

		//ゴールの生成
		pGoal = new CGoal(pos, rot);
	}

	//ファイル名代入
	m_pFilename = pFilename;

	//初期化処理
	pGoal->Init();

	return pGoal;
}

//==============================================================
//ゴールの初期化処理
//==============================================================
HRESULT CGoal::Init(void)
{
	CMaterial* pMaterial = CManager::GetInstance()->GetMaterial();

	//ゴールの読み込み
	m_nIdxGoal = pMaterial->Regist(m_pFilename);

	//マテリアルの割り当て
	CObjectX::BindMaterial(m_nIdxGoal);

	//オブジェクトXの初期化処理
	CObjectX::Init();

	//位置設定処理
	CObjectX::SetPos(m_pos);

	//種類の設定
	m_type = CObject::TYPE_GOAL;

	CObject::SetType(m_type);

	return S_OK;
}

//==============================================================
//ゴールの終了処理
//==============================================================
void CGoal::Uninit(void)
{
	//オブジェクトXの終了処理
	CObjectX::Uninit();
}

//==============================================================
//ゴールの更新処理
//==============================================================
void CGoal::Update(void)
{

	//オブジェクトXの更新処理
	CObjectX::Update();
}

//==============================================================
//ゴールの描画処理
//==============================================================
void CGoal::Draw(void)
{
	//オブジェクトXの描画処理
	CObjectX::Draw();
}

//==============================================================
//オブジェクトの種類設定処理
//==============================================================
void CGoal::SetType(TYPE type)
{
	m_type = type;

	//種類の設定
	CObject::SetType(m_type);
}

//==============================================================
//ゴールのヒット処理
//==============================================================
void CGoal::Hit(void)
{
	/*CPlayer* pPlayer = CGame::GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();*/

	
}