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

//マクロ定義
#define NUM_MODEL			(2)			//モデルの種類
#define POS					(10.0f)		//pos初期値
#define COL_R				(1.0f)		//赤色の数値
#define COL_G				(1.0f)		//緑色の数値
#define COL_B				(1.0f)		//青色の数値
#define COL_A				(1.0f)		//不透明度の数値
#define TEX_LEFT_X			(0.0f)		//テクスチャの左xの初期値
#define TEX_RIGHT_X			(1.0f)		//テクスチャの右xの初期値
#define TEX_UP_Y			(0.0f)		//テクスチャの上yの初期値
#define TEX_DOWN_Y			(1.0f)		//テクスチャの下yの初期値
#define MODEL_LEN			(3.0f)		//距離
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度

//静的メンバ変数宣言
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
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CModel::CModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//位置
	m_rot = rot;		//向き

	//CModel::Load(pFilename);
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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得

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

////==============================================================
////マテリアル・メッシュの破棄
////==============================================================
//void CModel::Unload(void)
//{
//	//メッシュの破棄
//	if (m_pMesh != NULL)
//	{
//		m_pMesh->Release();
//		m_pMesh = NULL;
//	}
//
//	//マテリアルの破棄
//	if (m_pBuffMat != NULL)
//	{
//		m_pBuffMat->Release();
//		m_pBuffMat = NULL;
//	}
//}

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
	pModel->Init(pos, rot);

	return pModel;
}

//==============================================================
//モデルの初期化処理
//==============================================================
HRESULT CModel::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CMaterial *pMaterial = CManager::GetMaterial();

	//モデルの読み込み
	m_nIdxModel = pMaterial->Regit(m_pFilename);

	//マテリアルの割り当て
	CObjectX::BindMaterial(m_nIdxModel);

	//オブジェクトXの初期化処理
	CObjectX::Init(pos, rot);

	//種類の設定
	CObject::SetType(CObject::TYPE_MODEL);

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