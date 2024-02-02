//=============================================================================
//
//スコア処理 [score.h]
//Author:佐藤根詩音
//
//=============================================================================
#ifndef _SCORE_H_		//このマクロ定義がされていなかったら
#define _SCORE_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//マクロ定義
#define NUM_DIGIT		(8)			//スコアの桁数

class CNumber;		//数字

//スコアクラス
class CScore : public CObject
{
public:
	CScore();	//コンストラクタ
	~CScore();	//デストラクタ

	static HRESULT Load(void);		//テクスチャの読み込み
	static void Unload(void);		//テクスチャの破棄
	static CScore *Create(void);	//生成処理

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void BindTexture(LPDIRECT3DTEXTURE9 pTexture);		//外部からテクスチャ指定

	static void Set(int nNum);					//スコア設定
	int Get(void) { return m_nNum; }			//スコア取得
	static void Add(int nValue);				//スコア足して設定

private:
	int Calculation(void);		//スコア計算処理

	static LPDIRECT3DTEXTURE9 m_pTexture;		//テクスチャへのポインタ
	int m_nIdxTex;		//テクスチャの番号

	D3DXVECTOR3 m_pos;			//スコアの位置
	static int m_nNum;			//スコアの値
	static int m_aTexU[NUM_DIGIT];

	static CNumber *m_apNumber[NUM_DIGIT];
};

#endif