//==============================================================
//
//タイトルのテクスチャ処理[titleTexture.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TITLETEXTURE_H_		//このマクロ定義がされていなかったら
#define _TITLETEXTURE_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//マクロ定義
#define NUM_TITLE_TEX		(2)		//タイトルのテクスチャの数

//前方宣言
class CObject2D;		//オブジェクト2D

//タイトルテクスチャクラス
class CTitleTex : public CObject
{
public:
	CTitleTex();	//コンストラクタ
	~CTitleTex();	//デストラクタ

	static CTitleTex *Create(void);		//生成処理

	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理

private:
	static CObject2D *m_apObject2D[NUM_TITLE_TEX];		//オブジェクト2Dのポインタ
	int m_nIdxTexture[NUM_TITLE_TEX];		//テクスチャの番号
	int m_nCntAppear;				//点滅カウンター
	bool m_bAppear;					//点滅の切り替え
	float m_fAlpha;					//アルファ値
	STATE m_state;					//状態
};

#endif
