//==============================================================
//
//フェード処理[fade.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _FADE_H_		//このマクロ定義がされていなかったら
#define _FADE_H_		//2重インクルード防止のマクロを定義する

#include "object2D.h"
#include "manager.h"

//**************************************************************
//フェードクラス
//**************************************************************
class CFade : public CObject2D
{
public:
	//フェードの状態
	enum FADE
	{
		FADE_NONE = 0,		//何もしていない状態
		FADE_IN,			//フェードイン状態
		FADE_OUT,			//フェードアウト状態
		FADE_MAX
	};

	CFade();	//コンストラクタ
	~CFade();	//デストラクタ

	static CFade *Create();		//生成処理

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	void SetFade(CScene::MODE modeNext);		//フェードの設定
	void SetNormalFade(void);					//ただのフェードの設定
	FADE GetFade(void) { return m_fade; }		//フェードの取得

private:
	int m_nIdxTexture;		//テクスチャの番号

	bool m_bTrans;			//画面遷移するか

	FADE m_fade;										//フェードの状態
	CScene::MODE m_modeNext;							//次の画面（モード）
	D3DXCOLOR m_colorFade;								//ポリゴン（フェード）の色
};

#endif
