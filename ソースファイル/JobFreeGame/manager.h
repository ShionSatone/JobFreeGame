//==============================================================
//
// マネージャ処理 [manager.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _MANAGER_H_		// このマクロ定義がされていなかったら
#define _MANAGER_H_		// 2重インクルード防止のマクロを定義する

#include "main.h"
#include "scene.h"

//**************************************************************
// 前方宣言
//**************************************************************
class CRenderer;		// レンダラー
class CInputKeyboard;	// キーボード入力
class CInputJoyPad;		// ゲームパッドの入力
class CDebugProc;		// デバッグ表示
class CSound;			// サウンド
class CScene;			// シーン
class CFade;			// フェード
class CTexture;			// テクスチャ
class CMaterial;		// マテリアル
class CCamera;			// カメラ
class CLight;			// ライト

//**************************************************************
// マネージャクラスの定義
//**************************************************************
class CManager
{
public:

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance,
		HWND hWnd,
		BOOL bWindow);		// 初期化処理
	void Uninit(void);		// 終了処理
	void Update(void);		// 更新処理
	void Draw(void);		// 描画処理
	void ResetMap(void);	// マップの再生成

	static CManager *GetInstance(void);		// マネージャの情報

	CRenderer *GetRenderer(void) { return m_pRenderer; }		// レンダラーの取得
	CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }		// キーボードの取得
	CInputJoyPad *GetInputJoyPad(void) { return m_pInputJoyPad; }			// ゲームパッドの取得
	CDebugProc *GetDebugProc(void) { return m_pDebugProc; }	// デバッグ表示の取得
	CSound *GetSound(void) { return m_pSound; }				// サウンドの取得
	CScene *GetScene(void) { return m_pScene; }				// シーンの情報
	CTexture *GetTexture(void) { return m_pTexture; }		// テクスチャの取得
	CMaterial *GetMaterial(void) { return m_pMaterial; }		// マテリアルの取得
	CFade *GetFade(void) { return m_pFade; }					// フェードの取得

	CCamera *GetCamera(void) { return m_pCamera; }			// カメラの情報
	CLight *GetLight(void) { return m_pLight; }				// ライトの情報

	void SetMode(CScene::MODE mode);		// ゲームモード設定
	CScene::MODE GetMode(void);				// ゲームモード取得
	void SetNumScore(int nNum) { m_nScore = nNum; }		// スコアの設定
	void SetNumDeath(int nNum) { m_nNumDeath = nNum; }	// 死亡数の設定
	void SetNumItem(int nNum) { m_nNumItem = nNum; }	// アイテム数の設定

	int GetNumScore(void) { return m_nScore; }			// スコアの取得
	int GetNumDeath(void) { return m_nNumDeath; }		// 死亡数の取得
	int GetNumItem(void) { return m_nNumItem; }			// アイテム数の取得

	void SetBgm(bool bBgm) { m_bBgm = bBgm; }		// BGMの設定
	bool GetBgm(void) { return m_bBgm; }				// BGMの取得
	void SetResult(bool bResult) { m_bResult = bResult; }	// リザルトの判定の設定
	bool GetResult(void) { return m_bResult; }				// リザルトの判定取得

	void SetFps(int nFps);			// FPSの設定

private:
	static CManager *m_pManager;			//マネージャの情報

	static CRenderer *m_pRenderer;		//レンダラーの情報
	static CDebugProc *m_pDebugProc;	//デバッグ表示の情報
	static CInputKeyboard *m_pInputKeyboard;	//キーボードの情報
	static CInputJoyPad *m_pInputJoyPad;		//ゲームパッドの情報
	static CSound *m_pSound;			//サウンドの情報
	static CScene *m_pScene;			//シーンの情報
	static CTexture *m_pTexture;		//テクスチャの情報
	static CMaterial *m_pMaterial;		//マテリアルの情報
	static CFade *m_pFade;				//フェードの情報
	static CCamera *m_pCamera;			//カメラの情報
	static CLight *m_pLight;			//ライトの情報

	static int m_nScore;				//スコア
	static int m_nNumDeath;				//死亡数
	static int m_nNumItem;				//アイテム数

	static bool m_bBgm;					//BGMストップするか
	static bool m_bResult;				//リザルトの判定
};

#endif
