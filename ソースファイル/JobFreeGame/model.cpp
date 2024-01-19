//==============================================================
//
//モデル処理[model.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "model.h"
#include "manager.h"
#include "renderer.h"
#include "material.h"
#include "player.h"
#include "game.h"
#include "sound.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define NUM_MODEL			(2)			//モデルの種類
#define POS					(10.0f)		//pos初期値
#define MODEL_LEN			(3.0f)		//距離
#define MOVE_DOWN			(-10.0f)		//落下移動量

//**************************************************************
//静的メンバ変数宣言
//**************************************************************
LPD3DXMESH CModel::m_pMesh = NULL;						//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER CModel::m_pBuffMat = NULL;					//マテリアルへのポインタ
DWORD CModel::m_dwNumMat = NULL;
const char *CModel::m_pFilename = NULL;					//ファイルの名前

//==============================================================
//コンストラクタ
//==============================================================
CModel::CModel()
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
CModel::CModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
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
CModel::~CModel()
{

}

//==============================================================
//Xファイルの読み込み
//==============================================================
HRESULT CModel::Load(const char *pFilename)
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
void CModel::Unload(void)
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
//モデルの生成処理
//==============================================================
CModel *CModel::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *pFilename)
{
	CModel *pModel = NULL;

	if (pModel == NULL)
	{//メモリが使用されてなかったら

		//モデルの生成
		pModel = new CModel(pos, rot);
	}

	//ファイル名代入
	m_pFilename = pFilename;

	//初期化処理
	pModel->Init();

	return pModel;
}

//==============================================================
//モデルの初期化処理
//==============================================================
HRESULT CModel::Init(void)
{
	CMaterial *pMaterial = CManager::GetInstance()->GetMaterial();

	//モデルの読み込み
	m_nIdxModel = pMaterial->Regist(m_pFilename);

	//マテリアルの割り当て
	CObjectX::BindMaterial(m_nIdxModel);

	//オブジェクトXの初期化処理
	CObjectX::Init();

	//位置設定処理
	CObjectX::SetPos(m_pos);

	//種類の設定
	m_type = CObject::TYPE_MODEL;

	CObject::SetType(m_type);

	return S_OK;
}

//==============================================================
//モデルの終了処理
//==============================================================
void CModel::Uninit(void)
{
	//オブジェクトXの終了処理
	CObjectX::Uninit();
}

//==============================================================
//モデルの更新処理
//==============================================================
void CModel::Update(void)
{
	//CSound *pSound = CManager::GetInstance()->GetSound();

	//if (m_bActionDown == true)
	//{//落下させるとき

	//	m_pos = CObjectX::GetPos();		//位置取得

	//	m_posOld = m_pos;	//前回の位置

	//	m_pos += m_move;	//移動量加算

	//	CObjectX::SetPos(m_pos);

	//	if (m_bCollision == false)
	//	{//他のブロックと当たってないとき

	//		if (CObjectX::Collision(&m_pos, &m_posOld, &m_move, CObjectX::GetSizeMin(), CObjectX::GetSizeMax()) == true)
	//		{//当たったら

	//			//pSound->Play(pSound->SOUND_LABEL_SE_COLLAPSE01);

	//			m_bCollision = true;
	//		}
	//	}
	//}

	//オブジェクトXの更新処理
	CObjectX::Update();
}

//==============================================================
//モデルの描画処理
//==============================================================
void CModel::Draw(void)
{
	//オブジェクトXの描画処理
	CObjectX::Draw();
}

//==============================================================
//オブジェクトの種類設定処理
//==============================================================
void CModel::SetType(TYPE type)
{
	m_type = type;

	//種類の設定
	CObject::SetType(m_type);
}

//==============================================================
//モデルのヒット処理
//==============================================================
void CModel::Hit(void)
{
	CPlayer *pPlayer = CGame::GetPlayer();
	CSound *pSound = CManager::GetInstance()->GetSound();

	//switch (m_type)
	//{
	//case TYPE_ALPHA_BLOCK:		//透明ブロック

	//	//プレイヤー自動ダッシュさせる
	//	pPlayer->SetDashAuto(true);

	//	break;

	//case TYPE_DOWN_6BLOCK:		//落下ブロック

	//	if (m_bDown == false)
	//	{
	//		pSound->Play(pSound->SOUND_LABEL_SE_COLLAPSE00);
	//		m_bDown = true;			//落とした状態にする

	//	}

	//	m_bActionDown = true;	//ブロックを落とす

	//	break;

	//case TYPE_DOWN_9BLOCK:		//落下ブロック

	//	if (m_bDown == false)
	//	{
	//		pSound->Play(pSound->SOUND_LABEL_SE_COLLAPSE00);
	//		m_bDown = true;			//落とした状態にする

	//	}

	//	m_bActionDown = true;	//ブロックを落とす
	//	m_bDown = true;			//落とした状態にする

	//	break;
	//}
}