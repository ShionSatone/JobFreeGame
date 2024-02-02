//==============================================================
//
// メッシュの円柱の処理[meshcylinder.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _CYLINDER_H_		// このマクロ定義がされていなかったら
#define _CYLINDER_H_		// 2重インクルード防止のマクロを定義する

#include "object.h"

//**************************************************************
// メッシュシリンダークラスの定義
//**************************************************************
class CMesuCylinder : public CObject
{
public:
	CMesuCylinder();		//コンストラクタ
	//CMesuCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// コンストラクタ(オーバーロード)
	~CMesuCylinder();		//デストラクタ

	static CMesuCylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// 生成処理

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void BindTexture(int nIdxTexture);					//外部からテクスチャ指定

private:

	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//インデックスバッファのポインタ
	D3DXMATRIX m_mtxWorld;					//ワールドマトリックス
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_rot;		// 向き

};

#endif
