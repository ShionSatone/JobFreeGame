//==============================================================
//
//オブジェクト3D処理[object3D.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _OBJECT3D_H_		//このマクロ定義がされていなかったら
#define _OBJECT3D_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//オブジェクト3Dクラス
class CObject3D : public CObject
{
public:
	CObject3D();	//コンストラクタ
	~CObject3D();	//デストラクタ

	static CObject3D *Create(void);		//生成処理

	virtual HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理

	void BindTexture(int nIdxTexture);		//外部からテクスチャ指定
	void SetSize(float fWidth, float fHeight);			//オブジェクトのサイズ設定

	void SetPosition(TYPE type, D3DXVECTOR3 pos) { m_pos = pos; }			//位置設定
	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//位置取得
	//void SetMove(D3DXVECTOR3 move) { m_move = move; }	//移動量設定
	//D3DXVECTOR3 GetMove(void) { return m_move; }		//移動量取得
	//void SetColor(TYPE type, D3DXCOLOR col);		//色設定
	//D3DXCOLOR GetColor(void) { return m_col; }		//色設定


private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
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
