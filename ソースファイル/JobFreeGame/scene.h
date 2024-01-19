//==============================================================
//
// シーン処理 [scene.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _SCENE_H_		//このマクロ定義がされていなかったら
#define _SCENE_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

//**************************************************************
// 前方宣言
//**************************************************************
class CTitle;			//タイトル画面
class CTutorial;		//チュートリアル画面
class CGame;			//ゲーム画面
class CResult;			//リザルト画面
class CRanking;			//ランキング画面

//**************************************************************
// シーンクラスの定義
//**************************************************************
class CScene : public CObject
{
public:

	//ゲームモード
	enum MODE
	{
		MODE_TITLE = 0,		//タイトル画面
		MODE_TUTORIAL,		//チュートリアル画面
		MODE_GAME,			//ゲーム画面
		MODE_RESULT,		//リザルト画面
		MODE_RANKING,		//ランキング画面
		MODE_MAX
	};

	CScene();
	CScene(MODE mode);
	~CScene();

	static CScene *Create(MODE mode);		//生成処理

	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);			//終了処理
	virtual void Update(void);			//更新処理
	virtual void Draw(void);			//描画処理

	MODE GetMode(void) { return m_mode; }			//モード取得
	void SetMode(MODE mode) { m_mode = mode; }		//モード設定

	static CGame *GetGame(void) { return m_pGame; }		//ゲーム画面取得
	static CRanking *GetRanking(void) { return m_pRanking; }		//ランキング画面取得

private:
	MODE m_mode;			//ゲームモード

	static CTitle *m_pTitle;			//タイトル画面の情報
	static CTutorial *m_pTutorial;		//チュートリアル画面
	static CGame *m_pGame;				//ゲーム画面の情報
	static CResult *m_pResult;			//リザルト画面の情報
	static CRanking *m_pRanking;		//ランキング画面の情報
};


#endif