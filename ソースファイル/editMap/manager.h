//==============================================================
//
//マネージャ処理[manager.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MANAGER_H_		//このマクロ定義がされていなかったら
#define _MANAGER_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//前方宣言
class CRenderer;		//レンダラー
class CInputKeyboard;	//キーボード入力
class CInputJoyPad;		//ゲームパッドの入力
class CCamera;			//カメラ
class CLight;			//ライト
class CDebugProc;		//デバッグ表示
class CTexture;			//テクスチャ
class CEdit;			//モデル配置
class CPlayerModel;		//プレイヤー
class CMaterial;		//マテリアル

//マネージャクラスの定義
class CManager
{
public:

	CManager();		//コンストラクタ
	~CManager();	//デストラクタ

	HRESULT Init(HINSTANCE hInstance,
		HWND hWnd,
		BOOL bWindow);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	void Load3DAll(void);	//全てのテクスチャ2Dの読み込み
	void UnLoad3DAll(void);	//全てのテクスチャ2Dの破棄
	void Create3DAll(void);	//全てのオブジェクト3Dの生成
	void ResetMap(void);	//マップの再生成

	static CRenderer *GetRenderer(void) { return m_pRenderer; }		//レンダラーの取得
	static CInputKeyboard *GetInputKeyboard(void) { return m_pInputKeyboard; }		//キーボードの取得
	static CInputJoyPad *GetInputJoyPad(void) { return m_pInputJoyPad; }			//ゲームパッドの取得

	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; }	//デバッグ表示の取得

	static CCamera *GetCamera(void) { return m_pCamera; }			//カメラの情報
	static CLight *GetLight(void) { return m_pLight; }				//ライトの情報
	static CTexture *GetTexture(void) { return m_pTexture; }		//テクスチャの情報
	static CEdit *GetEdit(void) { return m_pEdit; }					//モデル配置の情報
	static CPlayerModel *GetPlayerModel(void) { return m_pPlayerModel; }		//モデル配置の情報
	static CMaterial *GetMaterial(void) { return m_pMaterial; }		//マテリアルの情報

private:
	static CRenderer *m_pRenderer;		//レンダラーの情報
	static CInputKeyboard *m_pInputKeyboard;	//キーボードの情報
	static CInputJoyPad *m_pInputJoyPad;		//ゲームパッドの情報

	static CDebugProc *m_pDebugProc;		//デバッグ表示の情報

	static CCamera *m_pCamera;			//カメラの情報
	static CLight *m_pLight;			//ライトの情報
	static CTexture *m_pTexture;		//テクスチャの情報
	static CEdit *m_pEdit;				//モデル配置の情報
	static CPlayerModel *m_pPlayerModel;		//プレイヤーの情報
	static CMaterial *m_pMaterial;		//マテリアルの情報
	
	HINSTANCE m_hInstance;
	HWND m_hWnd;
	BOOL m_bWindow;
};

#endif
