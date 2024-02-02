//==============================================================
//
// 笛の範囲処理[whistle.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _WHISTLE_H_		// このマクロ定義がされていなかったら
#define _WHISTLE_H_		// 2重インクルード防止のマクロを定義する

#include "object3D.h"

//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 前方宣言
//**************************************************************

//**************************************************************
// 笛の範囲クラスの定義
//**************************************************************
class CWhistle : public CObject3D
{
public:

	// 笛の範囲の状態
	enum STATE
	{
		STATE_NONE = 0,		// 何もしない状態
		STATE_PLANE,		// 平面状態
		STATE_CYLINDER,		// 円柱状態
		STATE_MAX
	};

	// 集合の種類
	enum TYPE
	{
		TYPE_PLANE = 0,		// 平面上
		//TYPE_CYLINDER,		// 円柱
		TYPE_MAX
	};

	CWhistle();		//コンストラクタ
	CWhistle(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CWhistle();		//デストラクタ

	static CWhistle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetPos(D3DXVECTOR3 pos);		// 位置設定
	//void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	void SetState(STATE state) { m_state = state; }		// 状態設定

	//取得処理
	D3DXVECTOR3 GetPos(void);			// 位置取得
	//D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

	STATE GetState(void) { return m_state; }			// 状態取得

private:

	// 関数
	void UpdateState(void);			// 状態の更新処理
	void Follow(void);					// 追尾処理

	// 変数
	static CObject3D* m_pObject3D[TYPE_MAX];	// 3Dオブジェクト
	int m_nIdxTexture[TYPE_MAX];					// テクスチャの番号

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_size;		// 大きさ
	int m_nWhistleCounter;	// 笛を鳴らす時間
	float m_fscale;			// 拡大率

	STATE m_state;		// 状態
	TYPE m_type;		// 種類
};

#endif
