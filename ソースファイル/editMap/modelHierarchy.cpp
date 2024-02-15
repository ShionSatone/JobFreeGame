//==============================================================
//
//モデルの階層構造処理[modelHierarchy.h]
//Author:佐藤根詩音
//
//==============================================================
#include "modelHierarchy.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "objectX.h"
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

//==============================================================
//コンストラクタ
//==============================================================
CModelHier::CModelHier()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_posDefault = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//初期位置
	m_rotDefault = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//初期向き
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_pParent = NULL;		//親モデルへのポインタ
	m_pBuffMat = NULL;
	m_dwNumMat = NULL;
	m_pMesh = NULL;

	ZeroMemory(&m_pTexture, sizeof m_pTexture);
	//m_pTexture = NULL;

	m_state = CObjectX::STATE_NONE;		//通常状態
	m_matColor.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Power = 5.0f;
	m_matColor.MatD3D.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_nIdxModel = -1;
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CModelHier::CModelHier(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//位置
	m_rot = rot;		//向き
	m_posDefault = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//初期位置
	m_rotDefault = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//初期向き
	m_vtxMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最小値
	m_vtxMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//モデルの最大値
	m_pParent = NULL;	//親モデルへのポインタ
	m_pBuffMat = NULL;
	m_dwNumMat = NULL;
	m_pMesh = NULL;

	ZeroMemory(&m_pTexture, sizeof m_pTexture);
	//m_pTexture = NULL;

	m_state = CObjectX::STATE_NONE;		//通常状態
	m_matColor.MatD3D.Diffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Ambient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Emissive = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Power = 5.0f;
	m_matColor.MatD3D.Specular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	m_nIdxModel = -1;
}

//==============================================================
//デストラクタ
//==============================================================
CModelHier::~CModelHier()
{

}

//==============================================================
//Xファイルの読み込み
//==============================================================
HRESULT CModelHier::Load(char *pFileName)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得

	//Xファイルの読み込み
	D3DXLoadMeshFromX(pFileName,
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
//モデルの生成処理
//==============================================================
CModelHier *CModelHier::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName)
{
	CModelHier *pModelHier = NULL;

	if (pModelHier == NULL)
	{//メモリが使用されてなかったら

		//モデルの生成
		pModelHier = new CModelHier(pos, rot);
	}

	//初期化処理
	pModelHier->Init(pos, rot, pFileName);

	return pModelHier;
}

//==============================================================
//モデルの初期化処理
//==============================================================
HRESULT CModelHier::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName)
{
	CMaterial *pMaterial = CManager::GetMaterial();

	//モデルの読み込み
	m_nIdxModel = pMaterial->Regit(pFileName);

	//位置の設定
	m_pos = pos;

	//向きの設定
	m_rot = rot;

	//モデルの設定処理
	CModelHier::SetModel();

	return S_OK;
}

//==============================================================
//モデルの終了処理
//==============================================================
void CModelHier::Uninit(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		delete[] m_pTexture;
		m_pTexture = NULL;
	}

#if 0 // リークしてるぞ。
	delete this;
#endif
}

//==============================================================
//モデルの更新処理
//==============================================================
void CModelHier::Update(void)
{
	
}

//==============================================================
//モデルの描画処理
//==============================================================
void CModelHier::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	CMaterial *pMaterial = CManager::GetMaterial();
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;

	//ワールドマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	D3DXMATRIX mtxParent;		//親モデルのマトリックス

	if (m_pParent != NULL)
	{//親が存在する

		//親モデルのマトリックスを取得する
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{//親が存在しない

		//現在（最新）のマトリックスを取得する
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);

	}

	//親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	//パーツのワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pMaterial->GetMatAddress(m_nIdxModel)->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pMaterial->GetNumMatAddress(m_nIdxModel); nCntMat++)
	{
		switch (m_state)
		{
		case CObjectX::STATE_DAMAGE:		//ダメージ状態

			//マテリアルの設定
			pDevice->SetMaterial(&m_matColor.MatD3D);

			break;

		default:

			//マテリアルの設定
			pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

			break;
		}

		//テクスチャの設定
		pDevice->SetTexture(0, pMaterial->GetTexAddress(m_nIdxModel, nCntMat));

		//モデル（パーツ）の描画
		pMaterial->GetMeshAddress(m_nIdxModel)->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==============================================================
//モデルの設定処理
//==============================================================
void CModelHier::SetModel(void)
{
	CMaterial *pMaterial = CManager::GetMaterial();

	int nNumVtx;					//頂点数
	DWORD dwSizeFVF;				//頂点フォーマットのサイズ
	BYTE *pVtxBuff;					//頂点バッファへのポインタ

	//頂点数を取得
	nNumVtx = pMaterial->GetMeshAddress(m_nIdxModel)->GetNumVertices();

	//頂点フォーマットのサイズを取得
	dwSizeFVF = D3DXGetFVFVertexSize(pMaterial->GetMeshAddress(m_nIdxModel)->GetFVF());

	//頂点バッファをロック
	pMaterial->GetMeshAddress(m_nIdxModel)->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//頂点座標の代入

		//全ての頂点を比較してモデルの最大値を抜き出す
		if (vtx.x > m_vtxMax.x)
		{
			m_vtxMax.x = vtx.x;
		}
		else if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		else if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		//全ての頂点を比較してモデルの最小値を抜き出す
		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		else if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		else if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;								//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	pMaterial->GetMeshAddress(m_nIdxModel)->UnlockVertexBuffer();
}