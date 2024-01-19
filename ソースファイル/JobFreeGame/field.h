//==============================================================
//
// 床の処理[field.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _FIELD_H_		// このマクロ定義がされていなかったら
#define _FIELD_H_		// 2重インクルード防止のマクロを定義する

#include "object3D.h"

//**************************************************************
// 床クラスの定義
//**************************************************************
class CField : public CObject3D
{
public:
	CField();		//コンストラクタ
	CField(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// コンストラクタ(オーバーロード)
	~CField();		//デストラクタ

	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// 生成処理

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nIdxTexture;		// テクスチャの番号

	D3DXVECTOR3 m_pos;		// 位置
	int m_nType;			// 壁の種類
};

#endif
