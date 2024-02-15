//==============================================================
//
//エフェクトの処理[effect.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _EFFECT_H_		//このマクロ定義がされていなかったら
#define _EFFECT_H_		//2重インクルード防止のマクロを定義する

#include "billboard.h"

//プレイヤークラス
class CEffect : public CBillboard
{
public:
	CEffect();	//コンストラクタ
	~CEffect();	//デストラクタ

	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type);		//生成処理

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void Set(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXCOLOR m_col;		//色
	float m_fRadius;		//半径（大きさ）
	int m_nLife;			//エフェクトの寿命
	int m_type;
};

#endif