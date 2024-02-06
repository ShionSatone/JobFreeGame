//==============================================================
//
//ゴール処理[goal.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _GOAL_H_		//このマクロ定義がされていなかったら
#define _GOAL_H_		//2重インクルード防止のマクロを定義する

#include "objectX.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define MAX_MODEL			(1160)		//ゴールの数

//**************************************************************
//前方宣言
//**************************************************************
class CPlayer;		//プレイヤー

//**************************************************************
//ゴールクラスの定義
//**************************************************************
class CGoal : public CObjectX
{
public:
	CGoal();		//コンストラクタ
	CGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CGoal();		//デストラクタ

	static HRESULT Load(const char* pFilename);		//テクスチャの読み込み
	static void Unload(void);		//テクスチャの破棄
	static CGoal* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetType(TYPE type);		//オブジェクトの種類設定
	void Hit(void);					//ヒット処理

private:

	static LPD3DXMESH m_pMesh;						//メッシュ（頂点情報）へのポインタ
	static LPD3DXBUFFER m_pBuffMat;					//マテリアルへのポインタ
	static DWORD m_dwNumMat;						//マテリアルの数
	static const char* m_pFilename;					//ファイルの名前

	int m_nIdxGoal;		//ゴールの番号

	bool m_bActionDown;		//ブロックを落とすか
	bool m_bDown;			//落とされてるか
	bool m_bCollision;		//他のブロックと当たってるか

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_move;		//前回の位置
	D3DXVECTOR3 m_rot;		//向き
	TYPE m_type;
};

#endif
