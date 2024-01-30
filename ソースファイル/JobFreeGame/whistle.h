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
#define MAX_WHISTLE_TEX		(2)		// 集合画像の数

//**************************************************************
// 前方宣言
//**************************************************************

//**************************************************************
// 笛の範囲クラスの定義
//**************************************************************
class CWhistle : public CObject3D
{
public:	

	// 集合の種類
	enum CALL
	{
		CALL_PLANE = 0,		// 平面上
		CALL_CYLINDER,		// 円柱
		CALL_MAX
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
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定
	//void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	//void SetState(STATE state) { m_state = state; }		// 状態設定

	//取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置取得
	//D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

	//STATE GetState(void) { return m_state; }			// 状態取得

private:

	//void UpdateState(void);			// 状態の更新処理
	void Follow(void);					// 追尾処理

	CObject3D m_pObject3D[MAX_WHISTLE_TEX];

	int m_nIdxTexture[MAX_WHISTLE_TEX];		//テクスチャの番号

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
};

#endif
