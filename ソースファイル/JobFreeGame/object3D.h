//==============================================================
//
//オブジェクト3D処理[object3D.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _OBJECT3D_H_		//このマクロ定義がされていなかったら
#define _OBJECT3D_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//**************************************************************
//オブジェクト3Dクラス
//**************************************************************
class CObject3D : public CObject
{
public:
	CObject3D();	//コンストラクタ
	~CObject3D();	//デストラクタ

	static CObject3D *Create(void);		//生成処理

	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理

	void BindTexture(int nIdxTexture);					//外部からテクスチャ指定

	// セット・ゲット関数
	void SetType(TYPE type) { m_type = type; /*CObject::SetType(m_type);*/ }		//オブジェクトの種類の設定
	void SetSize(D3DXVECTOR3 m_size);							//オブジェクトのサイズ設定
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//位置設定

	TYPE GetType(void) { return m_type; }					//種類取得
	D3DXVECTOR3 GetPos(void) { return m_pos; }				//位置取得
	D3DXVECTOR3 GetSize(void) { return m_size; }			//サイズ取得

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_size;		//サイズ
	D3DXVECTOR3 m_rot;		//向き
	D3DXCOLOR m_col;		//色
	float m_fHeight;		//高さ
	float m_fWidth;			//横幅
	TYPE m_type;			//種類
};

#endif
