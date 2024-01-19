//==============================================================
//
//オブジェクト2D処理[object2D.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _OBJECT2D_H_		//このマクロ定義がされていなかったら
#define _OBJECT2D_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//オブジェクト2Dクラス
class CObject2D : public CObject
{
public:
	CObject2D();	//コンストラクタ
	~CObject2D();	//デストラクタ

	static CObject2D *Create(void);		//生成処理

	virtual HRESULT Init();			//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理

	void BindTexture(int nIdxTexture);		//外部からテクスチャ指定

	void SetType(TYPE type);		//オブジェクトの種類の設定
	void SetDraw(void);

	// セット・ゲット関数
	void SetPos(D3DXVECTOR3 pos);									//位置設定
	void SetMove(D3DXVECTOR3 move) { m_move = move; }				//移動量設定
	void SetColor(D3DXCOLOR col);									//色設定
	void SetSize(float fWidth, float fHeight) { m_fHeight = fHeight; m_fWidth = fWidth; }	//オブジェクトのサイズ設定
	void SetAnim(int nPatternAnim);						//アニメーション設定
	void SetbgMulti(float fTexU, float fTexV);			//多重背景設定

	D3DXVECTOR3 GetPos(void) { return m_pos; }	//位置取得
	D3DXVECTOR3 GetMove(void) { return m_move; }	//移動量取得
	D3DXCOLOR GetColor(void) { return m_col; }		//色取得

private:

	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き
	D3DXCOLOR m_col;		//色
	float m_fHeight;		//高さ
	float m_fWidth;			//横幅
	TYPE m_type;			//種類
};

#endif
