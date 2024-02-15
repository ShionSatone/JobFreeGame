//==============================================================
//
//床の処理[field.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _FIELD_H_		//このマクロ定義がされていなかったら
#define _FIELD_H_		//2重インクルード防止のマクロを定義する

#include"object3D.h"

//床クラスの定義
class CField : public CObject3D
{
public:
	CField();		//コンストラクタ
	CField(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CField();		//デストラクタ

	static HRESULT Load(void);		//テクスチャの読み込み
	static void Unload(void);		//テクスチャの破棄
	static CField *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void UnInit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
};

#endif
