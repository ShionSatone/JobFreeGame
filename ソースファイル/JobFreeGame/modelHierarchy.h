//==============================================================
//
//モデルの階層構造処理[modelHierarchy.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MODELHIERARCHY_H_		//このマクロ定義がされていなかったら
#define _MODELHIERARCHY_H_		//2重インクルード防止のマクロを定義する

#include "objectX.h"

//**************************************************************
//モデルの階層構造クラスの定義
//**************************************************************
class CModelHier
{
public:
	CModelHier();		//コンストラクタ
	CModelHier(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CModelHier();		//デストラクタ

	HRESULT Load(char *pFileName);		//テクスチャの読み込み
	static CModelHier *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName);		//生成処理

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, char *pFileName);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void SetParent(CModelHier *pModelHier) { m_pParent = pModelHier; }		//親モデルの設定
	D3DXMATRIX GetMtxWorld(void) { return m_mtxWorld; }	//ワールドマトリックス取得
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }					//位置設定
	void SetDefaultPos(D3DXVECTOR3 pos) { m_posDefault = pos; }	//初期位置設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }					//向き設定
	void SetDefaultRot(D3DXVECTOR3 rot) { m_rotDefault = rot; }	//初期向き設定
	void SetColor(D3DXCOLOR col);										//色設定
	void SetDisp(void) { m_bDisp = m_bDisp ? false : true; }			//表示設定
	void SetState(CObjectX::STATE state) { m_state = state; }			//敵の状態設定

	D3DXVECTOR3 GetPos(void) { return m_pos; }						//位置取得
	D3DXVECTOR3 GetDefaultPos(void) { return m_posDefault; }		//初期位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }						//向き取得
	D3DXVECTOR3 GetDefaultRot(void) { return m_rotDefault; }		//初期向き取得
	D3DXVECTOR3 GetSizeMax(void) { return m_vtxMax; }			//モデルの最大の大きさ取得
	D3DXVECTOR3 GetSizeMin(void) { return m_vtxMin; }			//モデルの最小の大きさ取得
	CObjectX::STATE GetState(void) { return m_state; }			//敵の状態取得

private:
	void SetModel(void);		//モデルの設定処理

	LPD3DXMESH m_pMesh;					//メッシュ（頂点情報）へのポインタ
	LPD3DXBUFFER m_pBuffMat;			//マテリアルへのポインタ
	DWORD m_dwNumMat;					//マテリアルの数

	D3DXVECTOR3 m_vtxMin;				//モデルの最小値
	D3DXVECTOR3 m_vtxMax;				//モデルの最大値

	int m_nIdxModel;		//モデルの番号
	int m_nAppear;			//点滅カウント

	bool m_bDisp;			//表示するかしないか

	D3DXVECTOR3 m_pos;				//位置
	D3DXVECTOR3 m_rot;				//向き
	D3DXVECTOR3 m_posDefault;		//初期位置
	D3DXVECTOR3 m_rotDefault;		//初期向き
	D3DXMATRIX m_mtxWorld;			//ワールドマトリックス

	CObjectX::STATE m_state;		//状態
	D3DXMATERIAL m_matColor;		//マテリアルデータへのポインタ

	CModelHier *m_pParent;		//親モデルへのポインタ
};

#endif
