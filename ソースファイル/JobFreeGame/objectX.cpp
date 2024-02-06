//==============================================================
//
//オブジェクトX処理[objectX.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "material.h"
#include "player.h"
#include "debugproc.h"
#include "camera.h"
#include "game.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define PRIORITY		(3)		//優先順位

//==============================================================
//コンストラクタ
//==============================================================
CObjectX::CObjectX()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_type = CObject::TYPE_NONE;		//種類
	m_nIdxModel = -1;

	m_pMesh = NULL;				//メッシュ（頂点情報）へのポインタ
	m_pBuffMat = NULL;			//マテリアルへのポインタ
	m_dwNumMat = 0;					//マテリアルの数

	m_vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);	//頂点数の最小値
	m_vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);	//頂点数の最大値

	m_matColor.MatD3D.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Power = 5.0f;
	m_matColor.MatD3D.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//==============================================================
//デストラクタ
//==============================================================
CObjectX::~CObjectX()
{

}

//==============================================================
//生成処理
//==============================================================
CObjectX *CObjectX::Create(void)
{
	CObjectX *pObjectX = NULL;

	if (pObjectX == NULL)
	{//メモリが使用されてなかったら

		//オブジェクトXの生成
		pObjectX = new CObjectX;
	}

	//初期化処理
	pObjectX->Init();

	return pObjectX;
}

//==============================================================
//マテリアル情報割り当て処理
//==============================================================
void CObjectX::BindMaterial(int nIdx)
{
	m_nIdxModel = nIdx;
}

//==============================================================
//オブジェクトXの初期化処理
//==============================================================
HRESULT CObjectX::Init(void)
{
	//モデルのサイズ設定
	CObjectX::SetModel();

	return S_OK;
}

//==============================================================
//オブジェクトXの終了処理
//==============================================================
void CObjectX::Uninit(void)
{
	/*for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (m_apTexture[nCntMat] != NULL)
		{
			m_apTexture[nCntMat]->Release();
			m_apTexture[nCntMat] = NULL;
		}
	}

	delete m_apTexture;
	m_apTexture = NULL;*/

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//オブジェクトXの更新処理
//==============================================================
void CObjectX::Update(void)
{

}

//==============================================================
//オブジェクトXの描画処理
//==============================================================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	CMaterial *pMaterial = CManager::GetInstance()->GetMaterial();
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

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)pMaterial->GetMatAddress(m_nIdxModel)->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)pMaterial->GetNumMatAddress(m_nIdxModel); nCntMat++)
	{
		switch (m_state)
		{
		case STATE_DAMAGE:		//ダメージ状態

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
void CObjectX::SetModel(void)
{
	CMaterial *pMaterial = CManager::GetInstance()->GetMaterial();

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
		/*else */if (vtx.y > m_vtxMax.y)
		{
			m_vtxMax.y = vtx.y;
		}
		/*else */if (vtx.z > m_vtxMax.z)
		{
			m_vtxMax.z = vtx.z;
		}

		//全ての頂点を比較してモデルの最小値を抜き出す
		if (vtx.x < m_vtxMin.x)
		{
			m_vtxMin.x = vtx.x;
		}
		/*else */if (vtx.y < m_vtxMin.y)
		{
			m_vtxMin.y = vtx.y;
		}
		/*else */if (vtx.z < m_vtxMin.z)
		{
			m_vtxMin.z = vtx.z;
		}

		pVtxBuff += dwSizeFVF;								//頂点フォーマットのサイズ分ポインタを進める
	}

	//頂点バッファをアンロック
	pMaterial->GetMeshAddress(m_nIdxModel)->UnlockVertexBuffer();
}

//==============================================================
//モデルとの当たり判定処理
//==============================================================
bool CObjectX::Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	bool bLand = false;		//着地したかどうか

	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		//オブジェクトを取得
		CObject *pObj = GetObject(/*PRIORITY, */nCntModel);

		if (pObj != NULL)
		{//メモリが使用されているとき

			//種類を取得
			CObject::TYPE type = pObj->GetType();

			//if (type == TYPE_MODEL || type == TYPE_ENEMY || type == TYPE_ITEM
			//	|| type == TYPE_NEEDLE || type == TYPE_ALPHA_BLOCK || type == TYPE_DOWN_9BLOCK || type == TYPE_DOWN_6BLOCK)
			//{//種類がモデルの場合

			//	//モデルの位置取得
			//	D3DXVECTOR3 posModel = pObj->GetPosition();
			//	D3DXVECTOR3 minModel = pObj->GetSizeMin();
			//	D3DXVECTOR3 maxModel = pObj->GetSizeMax();

			//	//モデルの当たり判定
			//	if (CObjectX::Collision2DModel(pPos, pPosOld, pMove, Min, Max, posModel, minModel, maxModel, type, pObj) == true)
			//	{
			//		bLand = true;		//着地した状態にする
			//	}
			//}
			//else if (type == TYPE_PLAYER)
			//{//プレイヤーの時

			//	//モデルの位置取得
			//	D3DXVECTOR3 posModel = pObj->GetPosition();
			//	D3DXVECTOR3 minModel = pObj->GetSizeMin();
			//	D3DXVECTOR3 maxModel = pObj->GetSizeMax();

			//	//当たり判定
			//	CObjectX::CollisionEnemy2DModel(pPos, pPosOld, pMove, Min, Max, posModel, minModel, maxModel, type, pObj);

			//}
		}
	}

	return bLand;
}

//==============================================================
//モデルの2Dの当たり判定処理
//==============================================================
bool CObjectX::Collision2DModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max, D3DXVECTOR3 posModel, D3DXVECTOR3 minModel, D3DXVECTOR3 maxModel, TYPE type, CObject *pObj)
{
	//CPlayer *pPlayer = CGame::GetPlayer();
	bool bLand = false;		//着地したかどうか

	//if (pPos->y + Max.y > posModel.y + minModel.y
	//	&& pPos->y < posModel.y + maxModel.y)
	//{
	//	if (pPosOld->x - Min.z <= posModel.x + minModel.x
	//		&& pPos->x - Min.z > posModel.x + minModel.x
	//		/*&& pPos->y + Max.y > posModel.y + minModel.y
	//		&& pPos->y < posModel.y + maxModel.y*/)
	//	{//左にめり込んだ場合

	//		if (pPosOld->y + Max.y <= posModel.y + minModel.y
	//			&& pPos->y + Max.y > posModel.y + minModel.y)
	//		{//底にめり込んだ場合

	//			pPos->y = posModel.y + minModel.y - Max.y;		//ブロックの底下に立たせる
	//			pMove->y = 0.0f;		//移動量を０にする
	//		}
	//		else if (pPosOld->y >= posModel.y + maxModel.y
	//			&& pPos->y < posModel.y + maxModel.y)
	//		{//上にめり込んだ場合

	//			pPos->y = posModel.y + maxModel.y;		//ブロックの上に立たせる
	//			pMove->y = 0.0f;		//移動量を０にする

	//			bLand = true;		//着地した状態にする
	//		}
	//		else
	//		{
	//			CPlayer *pPlayer = CGame::GetPlayer();

	//			if (type != TYPE_ITEM && pPlayer->GetIsDashAuto() == false)
	//			{//アイテム以外のとき

	//				pPos->x = posModel.x + minModel.x + Min.z;		//ブロックの左に立たせる
	//				pMove->x = 0.0f;		//移動量を０にする
	//			}
	//		}

	//		CPlayer *pPlayer = CGame::GetPlayer();

	//		if (type != TYPE_ALPHA_BLOCK && pPlayer->GetIsDashAuto() == true)
	//		{
	//			pPlayer->SetDashAuto(false);		//透明ブロックに当たってない状態にする
	//		}

	//		if (type == TYPE_ITEM || type == TYPE_ALPHA_BLOCK || type == TYPE_DOWN_9BLOCK || type == TYPE_DOWN_6BLOCK)
	//		{//アイテム || 透明ブロックに当たった時

	//			//アイテムのヒット処理
	//			pObj->Hit();
	//		}
	//		else if (type == TYPE_ENEMY || type == TYPE_PLAYER || type == TYPE_NEEDLE)
	//		{//敵に当たった時

	//			//プレイヤーのヒット処理
	//			pPlayer->Hit();
	//		}
	//	}

	//	if (pPosOld->x + Min.z >= posModel.x + maxModel.x
	//		&& pPos->x + Min.z < posModel.x + maxModel.x
	//		/*&& pPos->y + Max.y > posModel.y + minModel.y
	//		&& pPos->y < posModel.y + maxModel.y*/)
	//	{//右にめり込んだ場合

	//		if (pPosOld->y + Max.y <= posModel.y + minModel.y
	//			&& pPos->y + Max.y > posModel.y + minModel.y)
	//		{//底にめり込んだ場合

	//			pPos->y = posModel.y + minModel.y - Max.y;		//ブロックの底下に立たせる
	//			pMove->y = 0.0f;		//移動量を０にする
	//		}
	//		else if (pPosOld->y >= posModel.y + maxModel.y
	//			&& pPos->y < posModel.y + maxModel.y)
	//		{//上にめり込んだ場合

	//			pPos->y = posModel.y + maxModel.y;		//ブロックの上に立たせる
	//			pMove->y = 0.0f;		//移動量を０にする

	//			bLand = true;		//着地した状態にする
	//		}
	//		else
	//		{
	//			CPlayer *pPlayer = CGame::GetPlayer();

	//			if (type != TYPE_ITEM && pPlayer->GetIsDashAuto() == false)
	//			{//アイテム以外のとき

	//				pPos->x = posModel.x + maxModel.x - Min.z;		//ブロックの右に立たせる
	//				pMove->x = 0.0f;		//移動量を０にする
	//			}

	//		}

	//		CPlayer *pPlayer = CGame::GetPlayer();

	//		if (type != TYPE_ALPHA_BLOCK && pPlayer->GetIsDashAuto() == true)
	//		{
	//			pPlayer->SetDashAuto(false);		//透明ブロックに当たってない状態にする
	//		}

	//		if (type == TYPE_ITEM || type == TYPE_ALPHA_BLOCK || type == TYPE_DOWN_9BLOCK || type == TYPE_DOWN_6BLOCK)
	//		{//アイテム || 透明ブロックに当たった時

	//			//アイテムのヒット処理
	//			pObj->Hit();
	//		}
	//		else if (type == TYPE_ENEMY || type == TYPE_PLAYER || type == TYPE_NEEDLE)
	//		{//敵に当たった時

	//			//プレイヤーのヒット処理
	//			pPlayer->Hit();
	//		}
	//	}

	//	if (pPosOld->y + Max.y <= posModel.y + minModel.y
	//		&& pPos->y + Max.y > posModel.y + minModel.y
	//		&& pPos->x - Min.z > posModel.x + minModel.x
	//		&& pPos->x + Min.z < posModel.x + maxModel.x)
	//	{//底にめり込んだ場合

	//		if (pPosOld->x - Min.z <= posModel.x + minModel.x
	//			&& pPos->x - Min.z > posModel.x + minModel.x)
	//		{

	//		}
	//		else if (pPosOld->x + Min.z >= posModel.x + maxModel.x
	//			&& pPos->x + Min.z < posModel.x + maxModel.x)
	//		{

	//		}
	//		else
	//		{
	//			CPlayer *pPlayer = CGame::GetPlayer();

	//			if (type != TYPE_ITEM && pPlayer->GetIsDashAuto() == false)
	//			{//アイテム以外のとき

	//				pPos->y = posModel.y + minModel.y - Max.y;		//ブロックの底下に立たせる
	//				pMove->y = 0.0f;		//移動量を０にする
	//			}
	//		}

	//		CPlayer *pPlayer = CGame::GetPlayer();

	//		if (type != TYPE_ALPHA_BLOCK && pPlayer->GetIsDashAuto() == true)
	//		{
	//			pPlayer->SetDashAuto(false);		//透明ブロックに当たってない状態にする
	//		}

	//		if (type == TYPE_ITEM || type == TYPE_ALPHA_BLOCK || type == TYPE_DOWN_9BLOCK || type == TYPE_DOWN_6BLOCK)
	//		{//アイテム || 透明ブロックに当たった時

	//			//アイテムのヒット処理
	//			pObj->Hit();
	//		}
	//		else if (type == TYPE_ENEMY || type == TYPE_PLAYER || type == TYPE_NEEDLE)
	//		{//敵に当たった時

	//			//プレイヤーのヒット処理
	//			pPlayer->Hit();
	//		}
	//	}

	//	if (pPosOld->y >= posModel.y + maxModel.y
	//		&& pPos->y < posModel.y + maxModel.y
	//		&& pPos->x - Min.z > posModel.x + minModel.x
	//		&& pPos->x + Min.z < posModel.x + maxModel.x)
	//	{//上にめり込んだ場合

	//		if (pPosOld->x - Min.z <= posModel.x + minModel.x
	//			&& pPos->x - Min.z > posModel.x + minModel.x)
	//		{

	//		}
	//		else if (pPosOld->x + Min.z >= posModel.x + maxModel.x
	//			&& pPos->x + Min.z < posModel.x + maxModel.x)
	//		{

	//		}
	//		else
	//		{
	//			CPlayer *pPlayer = CGame::GetPlayer();

	//			if (type != TYPE_ITEM && pPlayer->GetIsDashAuto() == false)
	//			{//アイテム以外のとき

	//				pPos->y = posModel.y + maxModel.y;		//ブロックの上に立たせる
	//				pMove->y = 0.0f;		//移動量を０にする

	//				bLand = true;		//着地した状態にする
	//			}
	//		}

	//		CPlayer *pPlayer = CGame::GetPlayer();

	//		if (type != TYPE_ALPHA_BLOCK && pPlayer->GetIsDashAuto() == true)
	//		{
	//			pPlayer->SetDashAuto(false);		//透明ブロックに当たってない状態にする
	//		}

	//		if (type == TYPE_ITEM || type == TYPE_ALPHA_BLOCK || type == TYPE_DOWN_9BLOCK || type == TYPE_DOWN_6BLOCK)
	//		{//アイテム || 透明ブロックに当たった時

	//			//アイテムのヒット処理
	//			pObj->Hit();
	//		}
	//		else if (type == TYPE_ENEMY || type == TYPE_PLAYER || type == TYPE_NEEDLE)
	//		{//敵に当たった時

	//			//プレイヤーのヒット処理
	//			pPlayer->Hit();
	//		}
	//	}
	//}

	return bLand;
}

//==============================================================
//敵モデルとの当たり判定処理
//==============================================================
bool CObjectX::CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();

	bool bLand = false;		//着地したかどうか

	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		//オブジェクトを取得
		CObject *pObj = GetObject(/*PRIORITY, */nCntModel);

		if (pObj != NULL)
		{//メモリが使用されているとき

			//種類を取得
			CObject::TYPE type = pObj->GetType();

			if (type == TYPE_PLAYER)
			{//プレイヤーの時

				//モデルの位置取得
				D3DXVECTOR3 posModel = pObj->GetPos();
				D3DXVECTOR3 minModel = pObj->GetSizeMin();
				D3DXVECTOR3 maxModel = pObj->GetSizeMax();

				//当たり判定
				CObjectX::Collision3DModel(pPos, pPosOld, pMove, Min, Max, posModel, minModel, maxModel, type, pObj);

			}
		}
	}

	return bLand;
}

//==============================================================
//敵モデルの2Dの当たり判定処理
//==============================================================
bool CObjectX::CollisionEnemy2DModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max, D3DXVECTOR3 posModel, D3DXVECTOR3 minModel, D3DXVECTOR3 maxModel, TYPE type, CObject *pObj)
{
	//CPlayer *pPlayer = CGame::GetPlayer();
	bool bLand = false;		//着地したかどうか

	/*if (pPos->y + Max.y > posModel.y + minModel.y
	&& pPos->y < posModel.y + maxModel.y)*/
	{
		if (pPosOld->x - Min.z <= posModel.x + minModel.x
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->y + Max.y > posModel.y + minModel.y
			&& pPos->y < posModel.y + maxModel.y)
		{//左にめり込んだ場合

			CPlayer *pPlayer = CGame::GetPlayer();

			//プレイヤーのヒット処理
			pPlayer->Hit();
		}
		else if (pPosOld->x + Min.z >= posModel.x + maxModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x
			&& pPos->y + Max.y > posModel.y + minModel.y
			&& pPos->y < posModel.y + maxModel.y)
		{//右にめり込んだ場合

			CPlayer *pPlayer = CGame::GetPlayer();

			//プレイヤーのヒット処理
			pPlayer->Hit();
		}
		else if (pPosOld->y + Max.y <= posModel.y + minModel.y
			&& pPos->y + Max.y > posModel.y + minModel.y
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x)
		{//底にめり込んだ場合

			CPlayer *pPlayer = CGame::GetPlayer();

			//プレイヤーのヒット処理
			pPlayer->Hit();
		}
		else if (pPosOld->y >= posModel.y + maxModel.y
			&& pPos->y < posModel.y + maxModel.y
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x)
		{//上にめり込んだ場合

			CPlayer *pPlayer = CGame::GetPlayer();

			//プレイヤーのヒット処理
			pPlayer->Hit();
		}
	}

	return bLand;
}

//==============================================================
//モデルの3Dの当たり判定処理
//==============================================================
bool CObjectX::Collision3DModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max, D3DXVECTOR3 posModel, D3DXVECTOR3 minModel, D3DXVECTOR3 maxModel, TYPE type, CObject *pObj)
{
	bool bLand = false;		//着地したかどうか
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	/*if (pPos->y + Max.y > posModel.y + minModel.y
		&& pPos->y < posModel.y + maxModel.y)*/
	{
		if (pPosOld->x - Min.z <= posModel.x + minModel.x
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->z - Min.z > posModel.z + minModel.z
			&& pPos->z + Min.z < posModel.z + maxModel.z)
		{//左にめり込んだ場合

			if (pPosOld->y + Max.y <= posModel.y + minModel.y
				&& pPos->y + Max.y > posModel.y + minModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//底にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else if (pPosOld->y >= posModel.y + maxModel.y
				&& pPos->y < posModel.y + maxModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//上にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else
			{
				//pPos->x = posModel.x + minModel.x + Min.z;		//ブロックの左に立たせる
				//pMove->x = 0.0f;		//移動量を０にする

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}

		}

		if (pPosOld->x + Min.z >= posModel.x + maxModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x
			&& pPos->z - Min.z > posModel.z + minModel.z
			&& pPos->z + Min.z < posModel.z + maxModel.z)
		{//右にめり込んだ場合

			if (pPosOld->y + Max.y <= posModel.y + minModel.y
				&& pPos->y + Max.y > posModel.y + minModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//底にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else if (pPosOld->y >= posModel.y + maxModel.y
				&& pPos->y < posModel.y + maxModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//上にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else
			{

				//pPos->x = posModel.x + maxModel.x - Min.z;		//ブロックの右に立たせる
				//pMove->x = 0.0f;		//移動量を０にする

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}

		}

		if (pPosOld->z - Min.z <= posModel.z + minModel.z
			&& pPos->z - Min.z > posModel.z + minModel.z
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x)
		{//手前にめり込んだ場合

			if (pPosOld->y + Max.y <= posModel.y + minModel.y
				&& pPos->y + Max.y > posModel.y + minModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//底にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else if (pPosOld->y >= posModel.y + maxModel.y
				&& pPos->y < posModel.y + maxModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//上にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else
			{

				//pPos->z = posModel.z + minModel.z + Min.z;		//ブロックの手前に立たせる
				//pMove->z = 0.0f;		//移動量を０にする

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}

		}

		if (pPosOld->z + Min.z >= posModel.z + maxModel.z
			&& pPos->z + Min.z < posModel.z + maxModel.z
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x)
		{//奥にめり込んだ場合

			if (pPosOld->y + Max.y <= posModel.y + minModel.y
				&& pPos->y + Max.y > posModel.y + minModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//底にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else if (pPosOld->y >= posModel.y + maxModel.y
				&& pPos->y < posModel.y + maxModel.y
				&& pPos->x - Min.z > posModel.x + minModel.x
				&& pPos->x + Min.z < posModel.x + maxModel.x
				&& pPos->z - Min.z > posModel.z + minModel.z
				&& pPos->z + Min.z < posModel.z + maxModel.z)
			{//上にめり込んだ場合

				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
			}
			else
			{
				CPlayer* pPlayer = CGame::GetPlayer();

				//プレイヤーのヒット処理
				pPlayer->Hit();
				//pPos->z = posModel.z + maxModel.z - Min.z;		//ブロックの奥に立たせる
				//pMove->z = 0.0f;		//移動量を０にする
			}

		}

		if (pPosOld->y + Max.y <= posModel.y + minModel.y
			&& pPos->y + Max.y > posModel.y + minModel.y
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x
			&& pPos->z - Min.z > posModel.z + minModel.z
			&& pPos->z + Min.z < posModel.z + maxModel.z)
		{//底にめり込んだ場合
			CPlayer* pPlayer = CGame::GetPlayer();

			//プレイヤーのヒット処理
			pPlayer->Hit();
			//pPos->y = posModel.y + minModel.y - Max.y;		//ブロックの底下に立たせる
			//pMove->y = 0.0f;		//移動量を０にする

		}

		if (pPosOld->y >= posModel.y + maxModel.y
			&& pPos->y < posModel.y + maxModel.y
			&& pPos->x - Min.z > posModel.x + minModel.x
			&& pPos->x + Min.z < posModel.x + maxModel.x
			&& pPos->z - Min.z > posModel.z + minModel.z
			&& pPos->z + Min.z < posModel.z + maxModel.z)
		{//上にめり込んだ場合
			CPlayer* pPlayer = CGame::GetPlayer();

			//プレイヤーのヒット処理
			pPlayer->Hit();
			//pPos->y = posModel.y + maxModel.y;		//ブロックの上に立たせる
			//pMove->y = 0.0f;		//移動量を０にする

			//bLand = true;		//着地した状態にする

		}
	}

	return bLand;
}

//==============================================================
//エディットのモデルとの当たり判定処理
//==============================================================
int CObjectX::CollisionEdit(D3DXVECTOR3 pos, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	int nCnt = 0;

	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		//オブジェクトを取得
		CObject *pObj = GetObject(/*PRIORITY, */nCntModel);

		if (pObj != NULL)
		{//メモリが使用されているとき

			//種類を取得
			CObject::TYPE type = pObj->GetType();

			//if (type == pObj->TYPE_MODEL)
			//{//種類がモデルの場合

			//	//モデルの位置取得
			//	D3DXVECTOR3 posModel = pObj->GetPosition();
			//	D3DXVECTOR3 minModel = pObj->GetSizeMin();
			//	D3DXVECTOR3 maxModel = pObj->GetSizeMax();

			//	if (pos.x - Min.z > posModel.x + minModel.x
			//		&& pos.x + Min.z < posModel.x + maxModel.x
			//		&& pos.z - Min.z > posModel.z + minModel.z
			//		&& pos.z + Min.z < posModel.z + maxModel.z)
			//	{//モデルの範囲内の場合

			//		//終了処理
			//		pObj->Uninit();

			//		return nCnt;
			//	}

			//	nCnt++;		//モデル数加算
			//}
		}
	}

	return NULL;
}

//==============================================================
// ロックオンの当たり判定処理
//==============================================================
void CObjectX::CollisionRockOn(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max)
{
	for (int nCntModel = 0; nCntModel < MAX_OBJECT; nCntModel++)
	{
		//オブジェクトを取得
		CObject* pObj = GetObject(nCntModel);

		if (pObj != NULL)
		{//メモリが使用されているとき

			//種類を取得
			CObject::TYPE type = pObj->GetType();

			if (type == TYPE_GIMMICK)
			{//ギミックの時

				//モデルの位置取得
				D3DXVECTOR3 posModel = pObj->GetPos();
				D3DXVECTOR3 minModel = pObj->GetSizeMin();
				D3DXVECTOR3 maxModel = pObj->GetSizeMax();

				CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();



				/*if (pPos->y + Max.y > posModel.y + minModel.y
					&& pPos->y < posModel.y + maxModel.y)*/
				{
					if (pPosOld->x - Min.z <= posModel.x + minModel.x
						&& pPos->x - Min.z > posModel.x + minModel.x
						&& pPos->z - Min.z > posModel.z + minModel.z
						&& pPos->z + Min.z < posModel.z + maxModel.z)
					{//左にめり込んだ場合

						if (pPosOld->y + Max.y <= posModel.y + minModel.y
							&& pPos->y + Max.y > posModel.y + minModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//底にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else if (pPosOld->y >= posModel.y + maxModel.y
							&& pPos->y < posModel.y + maxModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//上にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else
						{
							//pPos->x = posModel.x + minModel.x + Min.z;		//ブロックの左に立たせる
							//pMove->x = 0.0f;		//移動量を０にする

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}

					}

					if (pPosOld->x + Min.z >= posModel.x + maxModel.x
						&& pPos->x + Min.z < posModel.x + maxModel.x
						&& pPos->z - Min.z > posModel.z + minModel.z
						&& pPos->z + Min.z < posModel.z + maxModel.z)
					{//右にめり込んだ場合

						if (pPosOld->y + Max.y <= posModel.y + minModel.y
							&& pPos->y + Max.y > posModel.y + minModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//底にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else if (pPosOld->y >= posModel.y + maxModel.y
							&& pPos->y < posModel.y + maxModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//上にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else
						{

							//pPos->x = posModel.x + maxModel.x - Min.z;		//ブロックの右に立たせる
							//pMove->x = 0.0f;		//移動量を０にする

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}

					}

					if (pPosOld->z - Min.z <= posModel.z + minModel.z
						&& pPos->z - Min.z > posModel.z + minModel.z
						&& pPos->x - Min.z > posModel.x + minModel.x
						&& pPos->x + Min.z < posModel.x + maxModel.x)
					{//手前にめり込んだ場合

						if (pPosOld->y + Max.y <= posModel.y + minModel.y
							&& pPos->y + Max.y > posModel.y + minModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//底にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else if (pPosOld->y >= posModel.y + maxModel.y
							&& pPos->y < posModel.y + maxModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//上にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else
						{

							//pPos->z = posModel.z + minModel.z + Min.z;		//ブロックの手前に立たせる
							//pMove->z = 0.0f;		//移動量を０にする

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}

					}

					if (pPosOld->z + Min.z >= posModel.z + maxModel.z
						&& pPos->z + Min.z < posModel.z + maxModel.z
						&& pPos->x - Min.z > posModel.x + minModel.x
						&& pPos->x + Min.z < posModel.x + maxModel.x)
					{//奥にめり込んだ場合

						if (pPosOld->y + Max.y <= posModel.y + minModel.y
							&& pPos->y + Max.y > posModel.y + minModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//底にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else if (pPosOld->y >= posModel.y + maxModel.y
							&& pPos->y < posModel.y + maxModel.y
							&& pPos->x - Min.z > posModel.x + minModel.x
							&& pPos->x + Min.z < posModel.x + maxModel.x
							&& pPos->z - Min.z > posModel.z + minModel.z
							&& pPos->z + Min.z < posModel.z + maxModel.z)
						{//上にめり込んだ場合

							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
						}
						else
						{
							CPlayer* pPlayer = CGame::GetPlayer();

							//プレイヤーのヒット処理
							pPlayer->Hit();
							//pPos->z = posModel.z + maxModel.z - Min.z;		//ブロックの奥に立たせる
							//pMove->z = 0.0f;		//移動量を０にする
						}

					}

					if (pPosOld->y + Max.y <= posModel.y + minModel.y
						&& pPos->y + Max.y > posModel.y + minModel.y
						&& pPos->x - Min.z > posModel.x + minModel.x
						&& pPos->x + Min.z < posModel.x + maxModel.x
						&& pPos->z - Min.z > posModel.z + minModel.z
						&& pPos->z + Min.z < posModel.z + maxModel.z)
					{//底にめり込んだ場合
						CPlayer* pPlayer = CGame::GetPlayer();

						//プレイヤーのヒット処理
						pPlayer->Hit();
						//pPos->y = posModel.y + minModel.y - Max.y;		//ブロックの底下に立たせる
						//pMove->y = 0.0f;		//移動量を０にする

					}

					if (pPosOld->y >= posModel.y + maxModel.y
						&& pPos->y < posModel.y + maxModel.y
						&& pPos->x - Min.z > posModel.x + minModel.x
						&& pPos->x + Min.z < posModel.x + maxModel.x
						&& pPos->z - Min.z > posModel.z + minModel.z
						&& pPos->z + Min.z < posModel.z + maxModel.z)
					{//上にめり込んだ場合
						CPlayer* pPlayer = CGame::GetPlayer();

						//プレイヤーのヒット処理
						pPlayer->Hit();
						//pPos->y = posModel.y + maxModel.y;		//ブロックの上に立たせる
						//pMove->y = 0.0f;		//移動量を０にする

						//bLand = true;		//着地した状態にする

					}
				}

			}
		}
	}
}

//==============================================================
//モデルの色設定処理
//==============================================================
void CObjectX::SetColor(D3DXCOLOR col)
{
	m_matColor.MatD3D.Diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Ambient = D3DXCOLOR(col.r, col.g, col.b, col.a);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Emissive = D3DXCOLOR(col.r, col.g, col.b, col.a);		//マテリアルデータへのポインタ
	m_matColor.MatD3D.Power = 5.0f;
	m_matColor.MatD3D.Specular = D3DXCOLOR(col.r, col.g, col.b, col.a);
}