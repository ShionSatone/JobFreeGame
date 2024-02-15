//==============================================================
//
//レンダリング処理[renderer.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "renderer.h"
#include "object.h"
#include "debugproc.h"
#include "manager.h"

//==============================================================
//コンストラクタ
//==============================================================
CRenderer::CRenderer()
{
	m_pD3D = NULL;			//Direct3Dオブジェクトへのポインタ
	m_pD3DDevice = NULL;	//Direct3Dデバイスへのポインタ
	m_pTexture = NULL;		//テクスチャへのポインタ
	m_pVtxBuff = NULL;		//頂点バッファへのポインタ
}

//==============================================================
//デストラクタ
//==============================================================
CRenderer::~CRenderer()
{

}

//==============================================================
//初期化処理
//==============================================================
HRESULT CRenderer::Init(HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			//ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	//プレゼンテーションパラメータ

	//Direct3Dオブジェクトの生成
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));							//パラメーターのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						//ゲーム画面サイズ（幅）
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						//ゲーム画面サイズ（高さ）
	d3dpp.BackBufferFormat = d3ddm.Format;						//バックバッファの形式
	d3dpp.BackBufferCount = 1;									//バクバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					//ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;						//デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					//デプスバッファとしてbit16を使う
	d3dpp.Windowed = bWindow;									//ウインドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	//リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	//インターバル

	//Direct3Dデバイスの生成
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_pD3DDevice)))
	{
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_pD3DDevice)))
		{
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	//レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		//テクスチャ繰り返しの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

	//テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	//デバッグ表示用フォントの生成
	/*D3DXCreateFont(m_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
		"Terminal", &g_pFont);*/

	//デバッグ表示の生成
	//if (m_pDebugProc == NULL)
	//{//メモリが使用されてないとき

	//	m_pDebugProc = new CDebugProc;
	//}

	//if (m_pDebugProc != NULL)
	//{//メモリが確保されたとき

	//	//デバッグ表示の初期化処理
	//	m_pDebugProc->Init();
	//}

	//乱数の種を設定
	srand((unsigned int)time(0));

	return S_OK;
}

//==============================================================
//終了処理
//==============================================================
void CRenderer::Uninit(void)
{
	//デバッグ表示用フォントの破棄
	/*if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}*/

	//デバッグ表示の終了処理
	/*if (m_pDebugProc != NULL)
	{
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = NULL;
	}*/

	//Direct3Dデバイスの破棄
	if (m_pD3DDevice != NULL)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = NULL;
	}

	//Direct3Dオブジェクトの破棄
	if (m_pD3D != NULL)
	{
		m_pD3D->Release();
		m_pD3D = NULL;
	}
}

//==============================================================
//更新処理
//==============================================================
void CRenderer::Update(void)
{
	//全てのオブジェクトの更新処理
	CObject::UpdateAll();

	//デバッグ表示の更新処理
	//m_pDebugProc->Update();
}

//==============================================================
//描画処理
//==============================================================
void CRenderer::Draw(void)
{
	D3DVIEWPORT9 viewportDef;

	//画面クリア（バックバッファとZバッファのクリア）
	m_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	//通常
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	//描画開始
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{//描画開始が成功した場合

		//現在のビューポートを取得
		m_pD3DDevice->GetViewport(&viewportDef);



		//ビューポートを元に戻す
		m_pD3DDevice->SetViewport(&viewportDef);

		//全てのオブジェクトの描画処理
		CObject::DrawAll();

		//デバッグ表示の取得
		CDebugProc *pDebugProc = CManager::GetDebugProc();

		//デバッグ表示の描画処理
		pDebugProc->Draw();

#ifdef _DEBUG
		//FPSの表示
		//DrawFPS();

		////マウスのデバッグ表示
		//GetMouseDebug();
#endif

		//描画終了
		m_pD3DDevice->EndScene();
	}

	//バックバッファとフロントバッファの入れ替え
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}