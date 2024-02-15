//==============================================================
//
//テクスチャの処理[texture.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TEXTURE_H_		//このマクロ定義がされていなかったら
#define _TEXTURE_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_TEX		(128)		//テクスチャの最大数

//テクスチャクラスの定義
class CTexture
{
public:
	//テクスチャの種類
	typedef enum
	{
		TEX_PLAYER = 0,		//プレイヤー
		TEX_ENEMY,			//敵
		TEX_FIELD,			//床
		//TEX_WALL,			//壁
		TEX_BLOCK,			//ブロック
		TEX_BULLET,			//弾
		TEX_EFFECT,			//エフェクト
		TEX_SCORE,			//スコア
		TEX_MAX

	} TEX;

	CTexture();		//コンストラクタ
	~CTexture();	//デストラクタ

	HRESULT Load(void);		//テクスチャ読み込み処理
	void Unload(void);		//テクスチャ破棄処理

	int Regit(const char *pFilename);				//テクスチャの番号指定
	LPDIRECT3DTEXTURE9 GetAddress(int nIdx);		//テクスチャの情報取得

private:
	LPDIRECT3DTEXTURE9 m_apTexture[MAX_TEX];
	static int m_nNumAll;		//テクスチャの数
	static const char *m_apFilename[MAX_TEX];		//テクスチャの名前
};

#endif
