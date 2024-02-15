//==============================================================
//
//レンダリング処理[renderer.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _RENDERER_H_		//このマクロ定義がされていなかったら
#define _RENDERER_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//前方宣言
class CDebugProc;		//デバッグ表示

//レンダラークラス
class CRenderer
{
public:
	CRenderer();		//コンストラクタ
	~CRenderer();		//デストラクタ
	
	HRESULT Init(HWND hWnd, BOOL bWindow);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	LPDIRECT3DDEVICE9 GetDevice(void) { return m_pD3DDevice; }		//デバイスの取得

private:
	//static CDebugProc *m_pDebugProc;		//デバッグ表示の情報

	LPDIRECT3D9 m_pD3D;						//Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_pD3DDevice;			//Direct3Dデバイスへのポインタ

	LPDIRECT3DTEXTURE9 m_pTexture;			//テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//頂点バッファへのポインタ
};

#endif