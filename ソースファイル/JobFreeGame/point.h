//==============================================================
//
// ポイントの処理[point.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _POINT_H_		// このマクロ定義がされていなかったら
#define _POINT_H_		// 2重インクルード防止のマクロを定義する

#include "object3D.h"

//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 前方宣言
//**************************************************************

//**************************************************************
// ポイントクラスの定義
//**************************************************************
class CPoint : public CObject3D
{
public:

	// 状態
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_LOCKON,		// ロックオン状態
		STATE_THROW,		// 投げる状態
		STATE_MAX
	};

	// 色の種類
	enum COLOR
	{
		COLOR_NONE = 0,		// 通常色(灰色)
		COLOR_SPRING,		// 春(ピンク)
		COLOR_SUMMER,		// 夏(緑)
		COLOR_FALL,			// 秋(オレンジ)
		COLOR_WINTER,		// 冬(白)
		COLOR_MAX
	};

	CPoint();		//コンストラクタ
	CPoint(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CPoint();		//デストラクタ

	static CPoint* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

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

	void ControlKeyboard(void);			// キーボード操作
	//void UpdateState(void);			// 状態の更新処理
	void Screen(void);					// 画面外判定
	void UpdateState(void);				// 状態更新処理
	void UpdateColor(void);				// 色更新処理
	void Follow(void);					// 追尾処理

	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置

	STATE m_state;			// 状態
	COLOR m_color;			// 色
};

#endif
