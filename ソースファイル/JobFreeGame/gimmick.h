//==============================================================
//
// ギミックの処理[gimmick.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _GIMMICK_H_		// このマクロ定義がされていなかったら
#define _GIMMICK_H_		// 2重インクルード防止のマクロを定義する

#include "objectX.h"

//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 前方宣言
//**************************************************************

//**************************************************************
// ギミッククラスの定義
//**************************************************************
class CGimmick : public CObjectX
{
public:

	// ギミックの状態
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATHSTANDBY,	// 死亡準備状態
		STATE_DEATH,		// 死亡状態

		STATE_MAX
	};

	// ギミックの種類
	enum TYPE
	{
		TYPE_NONE = 0,				// 何もなし
		TYPE_SOILWALL,				// 土の壁
		TYPE_MAX
	};

	CGimmick();		//コンストラクタ
	CGimmick(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);		//コンストラクタ(オーバーロード)
	~CGimmick();		//デストラクタ

	static CGimmick* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(void);		// ヒット処理

	// 設定処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	void SetState(STATE state) { m_state = state; }		// 状態設定

	//取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

	//D3DXVECTOR3 GetSizeMin(void) { return m_min; }		// 大きさの最大値取得
	//D3DXVECTOR3 GetSizeMax(void) { return m_max; }		// 大きさの最小値取得
	STATE GetState(void) { return m_state; }			// 状態取得
	int GetLife(void) { return m_nLife; }				// 寿命取得

private:

	void UpdateState(void);			// 状態の更新処理

	void CheckExist(void);			// ギミックの存在確認処理

	void Screen(void);				// 画面外判定
	void LoadFile(void);			// モデルファイル読み込み

	static int m_nNumAll;			// ギミックの総数
	int m_nIndex;					// ギミックの番号
	int m_nIdxModel;				// モデルの番号

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_posDest;	// 目的の位置

	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_rotDest;	// 目的の向き
	D3DXVECTOR3 m_max;		// 人間の最大値
	D3DXVECTOR3 m_min;		// 人間の最小値
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス

	int m_nLife;			// 寿命
	int m_nDamageCounter;	// ダメージカウンター
	int m_nDeathCounter;	// 死亡状態にするまでのカウンター

	STATE m_state;			// ギミックの状態
	TYPE m_type;			// ギミックの種類
};

#endif