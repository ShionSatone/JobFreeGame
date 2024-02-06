//==============================================================
//
//オブジェクトX処理[objectX.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _OBJECTX_H_		//このマクロ定義がされていなかったら
#define _OBJECTX_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define MAX_TEX		(128)		//テクスチャの最大数

//**************************************************************
//オブジェクトXクラス
//**************************************************************
class CObjectX : public CObject
{
public:
	CObjectX();	//コンストラクタ
	~CObjectX();	//デストラクタ

	static CObjectX *Create(void);		//生成処理

	virtual HRESULT Init(void);			//初期化処理
	virtual void Uninit(void);			//終了処理
	virtual void Update(void);			//更新処理
	virtual void Draw(void);			//描画処理

	void BindMaterial(int nIdx);		//外部からマテリアル指定

	//void BindXFile(LPD3DXMESH pMesh, LPD3DXBUFFER pBuffMat, DWORD dwNumMat);

	// セット・ゲット関数
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }			//位置設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }			//向き設定
	void SetSize(D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax) { m_vtxMin = vtxMin;  m_vtxMax = vtxMax; }		//モデルの大きさ設定
	void SetState(STATE state) { m_state = state; }				//敵の状態設定
	void SetColor(D3DXCOLOR col);								//色設定

	D3DXVECTOR3 GetPos(void) { return m_pos; }			//位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }			//向きの取得
	D3DXVECTOR3 GetSizeMin(void) { return m_vtxMin; }		//最小の大きさ取得
	D3DXVECTOR3 GetSizeMax(void) { return m_vtxMax; }		//最大の大きさ取得
	STATE GetState(void) { return m_state; }				//敵の状態取得

	void SetModel(void);		//モデルの設定処理
	static bool Collision(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max);			//モデルの当たり判定
	static bool CollisionEnemy(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max);		//モデルの当たり判定
	static bool CollisionGimmick(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max);	//モデルの当たり判定
	static int CollisionEdit(D3DXVECTOR3 pos, D3DXVECTOR3 Min, D3DXVECTOR3 Max);		//エディットのモデルとの当たり判定
	static void CollisionRockOn(D3DXVECTOR3* pPos, D3DXVECTOR3* pPosOld, D3DXVECTOR3* pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max);	// ロックオンの当たり判定

private:

	static bool Collision2DModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max, D3DXVECTOR3 posModel, D3DXVECTOR3 minModel, D3DXVECTOR3 maxModel, TYPE type, CObject *pObj);		//モデルの2Dの当たり判定
	static bool CollisionEnemy2DModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max, D3DXVECTOR3 posModel, D3DXVECTOR3 minModel, D3DXVECTOR3 maxModel, TYPE type, CObject *pObj);		//敵モデルの2Dの当たり判定
	static bool Collision3DModel(D3DXVECTOR3 *pPos, D3DXVECTOR3 *pPosOld, D3DXVECTOR3 *pMove, D3DXVECTOR3 Min, D3DXVECTOR3 Max, D3DXVECTOR3 posModel, D3DXVECTOR3 minModel, D3DXVECTOR3 maxModel, TYPE type, CObject *pObj);		//モデルの3Dの当たり判定

	LPD3DXMESH m_pMesh;							//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;					//マテリアルへのポインタ
	DWORD m_dwNumMat;							//マテリアルの数

	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 m_vtxMin;						//モデルの最小値
	D3DXVECTOR3 m_vtxMax;						//モデルの最大値

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き
	D3DXCOLOR m_col;		//色
	TYPE m_type;			//種類
	STATE m_state;			//状態

	int m_nIdxParent;				//親モデルのインデックス
	D3DXMATERIAL m_matColor;		//マテリアルデータへのポインタ

	int m_nIdxModel;		//モデルの番号

};

#endif