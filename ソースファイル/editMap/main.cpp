//==============================================================
//
//メイン処理[main.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "main.h"
#include "manager.h"

//マクロ定義
#define CLASS_NAME			"Window Class"			//ウインドウクラスの名前
#define WINDOW_NAME			"C++"		//ウインドウの名前

//プロトタイプ宣言
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawFPS(void);
void DrawDEBUG(void);

//グローバル変数
LPD3DXFONT g_pFont = NULL;						//フォントへのポインタ
int g_nCountFPS = 0;							//FPSカウンタ
bool g_bDispDebug = false;						//デバッグ表示のON/OFF

#ifdef NDEBUG		//リリース時
MODE g_mode = MODE_TITLE;						//現在のモード
#endif

#ifdef _DEBUG		//デバッグ時
MODE g_mode = MODE_TITLE;						//現在のモード
#endif

//==============================================================
//メイン関数
//==============================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrey, LPSTR lpCmdLine, int nCmdShow)
{
	//終了時にメモリリークを出力
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	CManager *pManager = NULL;
	HWND hWnd;				//ウインドウハンドル(識別子)
	MSG msg;				//メッセージを格納する変数
	DWORD dwCurrentTime;	//現在時刻
	DWORD dwExecLastTime;	//最後に処理した時刻
	DWORD dwFrameCount;		//フレームカウント
	DWORD dwFPSLastTime;	//最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					//WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							//ウインドウのスタイル
		WindowProc,							//ウインドウのスタイル
		0,									//0にする（通常は使用しない）
		0,									//0にする（通常は使用しない）
		hInstance,							//インスタンスハンドル
		LoadIcon(NULL,IDI_APPLICATION),		//タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			//マウスカーソル
		HBRUSH(COLOR_WINDOW + 1),			//クライアント領域の背景色
		NULL,								//メニューバー
		CLASS_NAME,							//ウインドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION),		//ファイルのアイコン
	};

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	//ウインドウを生成
	hWnd = CreateWindowEx(0,		//拡張ウインドウスタイル
		CLASS_NAME,					//ウインドウクラスの名前
		WINDOW_NAME,				//ウインドウの名前
		WS_OVERLAPPEDWINDOW,		//ウインドウスタイル
		CW_USEDEFAULT,				//ウインドウの左上X座標
		CW_USEDEFAULT,				//ウインドウの左上Y座標
		(rect.right - rect.left),	//ウインドウの幅
		(rect.bottom - rect.top),	//ウインドウの幅
		NULL,						//親ウインドウのハンドル
		NULL,						//メニューハンドルまたは子ウインドウID
		hInstance,					//インスタンスハンドル
		NULL);

	//マネージャの生成
	if (pManager == NULL)
	{//メモリが使用されてなかったら

		pManager = new CManager;
	}

	if (pManager != NULL)
	{//メモリが確保できてたら

		if (FAILED(pManager->Init(hInstance, hWnd, TRUE)))
		{//初期化処理が失敗した場合

			return -1;
		}
	}

	//分解能を設定
	timeBeginPeriod(1);

	dwCurrentTime = 0;					//初期化する
	dwExecLastTime = timeGetTime();		//現在時刻を取得（保存）
	dwFrameCount = 0;					//初期化する
	dwFPSLastTime = timeGetTime();		//現在時刻を取得（保存）

	//ウインドウの表示
	ShowWindow(hWnd, nCmdShow);			//ウインドウの表示状態を設定
	UpdateWindow(hWnd);					//クライアント領域を更新

	//メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{//windowsの処理

			if (msg.message == WM_QUIT)
			{//WM_QUITメッセージを受け取ったらメッセージループを抜ける
				break;
			}
			else
			{
				//メッセージの設定
				TranslateMessage(&msg);		//仮想キーメッセージを文字メッセージへ返還
				DispatchMessage(&msg);		//ウインドウプロしーじゃへメッセージを送出
			}
		}
		else
		{//DirectXの処理

			dwCurrentTime = timeGetTime();	//現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{//0.5秒経過

				//FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;		//FPSを測定した時刻を保存
				dwFrameCount = 0;					//フレームカウントをクリア
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{//60分の1秒経過

				dwExecLastTime = dwCurrentTime;		//処理開始の時刻[現在時刻]を保存
				dwFrameCount++;						//フレームカウントを加算

				if (pManager != NULL)
				{//メモリが確保できてたら

					//マネージャの更新処理
					pManager->Update();

					//マネージャの描画処理
					pManager->Draw();
				}

			}
		}
	}

	//マネージャの破棄
	if (pManager != NULL)
	{//メモリが使用されてたら

		//マネージャの終了処理
		pManager->Uninit();

		//メモリ開放
		delete pManager;
		pManager = NULL;		//NULL代入
	}

	//分解能を戻す
	timeEndPeriod(1);

	//ウインドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return(int)msg.wParam;
}

//==============================================================
//ウインドウプロシージャ
//==============================================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;												//返り値を格納
	const RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };	//ウインドウの領域（四角）

	switch (uMsg)
	{
	case WM_DESTROY:			//ウインドウ破棄のメッセージ

		//WM_QUITメッセージを返す
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:			//キー押下のメッセージ

		switch (wParam)
		{
		case VK_ESCAPE:			//[ESC]キーが押された

			nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

			if (nID == IDYES)
			{
				//ウインドウを破棄する(WM_DESTROYメッセージを送る)
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:				//閉じるボタン押下のメッセージ

		nID = MessageBox(hWnd, "終了しますか？", "終了メッセージ", MB_YESNO);

		if (nID == IDYES)
		{
			//ウインドウを破棄する(WM_DESTROYメッセージを送る)
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;		//0を返さないと終了してしまう
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);		//既定の処理を返す
}

//==============================================================
//FPSの表示
//==============================================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[256];

	//文字列に代入
	wsprintf(&aStr[0], "FPS：%d\n", g_nCountFPS);

	//テキストの描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT,
		D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================================
//デバッグ表示
//==============================================================
void DrawDEBUG(void)
{
	//RECT rect = { 0,15,SCREEN_WIDTH,SCREEN_HEIGHT };
	//char aStr[256];
	//Camera *pCamera = GetCamera();

	////文字列に代入
	//wsprintf(&aStr[0], "\n---照準操作---\n上：[W]\n下：[S]\n左：[A]\n右：[D]\n\n---弾---\n発射：[SPACE]\n\nリザルト画面：[Enter]\n(ゲーム画面で)");

	////テキストの描画
	//g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT,
	//	D3DCOLOR_RGBA(255, 255, 255, 255));
}

//==============================================================
//モード設定
//==============================================================
void SetMode(MODE mode)
{
	//現在の画面(モード)の終了処理
	//switch (g_mode)
	//{
	//case MODE_TITLE:		//タイトル画面
	//	UninitTitle();
	//	break;

	//case MODE_TUTORIAL:		//ポーズ画面
	//	UninitTutorial();
	//	break;

	//case MODE_GAME:			//ゲーム画面
	//	UninitGame();
	//	break;

	//case MODE_RESULT:		//リザルト画面
	//	UninitResult();
	//	break;

	//case MODE_RANKING:		//ランキング画面
	//	UninitRanking();
	//	break;
	//}

	//新しい画面(モード)の初期化処理
	//switch (mode)
	//{
	//case MODE_TITLE:		//タイトル画面
	//	g_mode = mode;		//モードを設定する

	//	InitTitle();
	//	break;

	//case MODE_TUTORIAL:		//ポーズ画面
	//	InitTutorial();
	//	break;

	//case MODE_GAME:			//ゲーム画面
	//	g_mode = mode;		//モードを設定する

	//	InitGame();
	//	break;

	//case MODE_RESULT:		//リザルト画面
	//	InitResult();
	//	break;

	//case MODE_RANKING:		//ランキング画面
	//	InitRanking();
	//	break;
	//}

	g_mode = mode;			//モードを設定する
}

//==============================================================
//モードの取得
//==============================================================
MODE GetMode(void)
{
	return g_mode;
}

//==============================================================
//フォントの取得
//==============================================================
LPD3DXFONT GetFont(void)
{
	return g_pFont;
}