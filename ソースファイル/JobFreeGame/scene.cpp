//==============================================================
//
// シーン処理 [scene.cpp]
// Author:佐藤根詩音
//
//==============================================================
#include "scene.h"
#include "manager.h"
#include "debugproc.h"
#include "title.h"
//#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define PRIORITY		(3)		//優先順位

//**************************************************************
// 静的メンバ変数宣言
//**************************************************************
CTitle *CScene::m_pTitle = NULL;			//タイトル画面の情報
CTutorial *CScene::m_pTutorial = NULL;		//チュートリアル画面
CGame *CScene::m_pGame = NULL;				//ゲーム画面の情報
CResult *CScene::m_pResult = NULL;			//リザルト画面の情報
CRanking *CScene::m_pRanking = NULL;		//ランキング画面の情報

//==============================================================
//シーンのコンストラクタ
//==============================================================
CScene::CScene()
{
	m_mode = MODE_TITLE;		//ゲームモード
}

//==============================================================
//シーンのコンストラクタ(オーバーロード)
//==============================================================
CScene::CScene(MODE mode)
{
	m_mode = mode;		//ゲームモード
}

//==============================================================
//シーンのデストラクタ
//==============================================================
CScene::~CScene()
{

}

//==============================================================
//シーンの生成処理
//==============================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = NULL;

	if (pScene == NULL)
	{//メモリが使用されてなかったら

		//指定されたモードを生成
		pScene = new CScene(mode);
	}

	return pScene;
}

//==============================================================
//シーンの初期化処理
//==============================================================
HRESULT CScene::Init(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:		//タイトル画面

		//タイトル画面の生成
		if (m_pTitle == NULL)
		{
			m_pTitle = new CTitle;

			if (m_pTitle != NULL)
			{
				//ゲーム画面の初期化
				if (FAILED(m_pTitle->Init()))
				{//初期化処理が失敗した場合

					return E_FAIL;
				}
			}
		}

		break;

	case MODE_TUTORIAL:		//チュートリアル画面

		////チュートリアル画面の生成
		//if (m_pTutorial == NULL)
		//{
		//	m_pTutorial = new CTutorial;

		//	if (m_pTutorial != NULL)
		//	{
		//		//ゲーム画面の初期化
		//		if (FAILED(m_pTutorial->Init()))
		//		{//初期化処理が失敗した場合

		//			return E_FAIL;
		//		}
		//	}
		//}

		break;

	case MODE_GAME:			//ゲーム画面

		//ゲーム画面の生成
		if (m_pGame == NULL)
		{
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				//ゲーム画面の初期化
				if (FAILED(m_pGame->Init()))
				{//初期化処理が失敗した場合

					return E_FAIL;
				}
			}
		}

		break;

	case MODE_RESULT:		//リザルト画面

		//リザルト画面の生成
		if (m_pResult == NULL)
		{
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				//リザルト画面の初期化
				if (FAILED(m_pResult->Init()))
				{//初期化処理が失敗した場合

					return E_FAIL;
				}
			}
		}

		break;

	case MODE_RANKING:		//ランキング画面

		//ランキング画面の生成
		if (m_pRanking == NULL)
		{
			m_pRanking = new CRanking;

			if (m_pRanking != NULL)
			{
				//ランキング画面の初期化
				if (FAILED(m_pRanking->Init()))
				{//初期化処理が失敗した場合

					return E_FAIL;
				}
			}
		}

		break;

	default:

		// 停止する
		assert(false);

		break;
	}


	return S_OK;
}

//==============================================================
//シーンの終了処理
//==============================================================
void CScene::Uninit(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:		//タイトル画面

		//タイトル画面の破棄
		if (m_pTitle != NULL)
		{
			//終了処理
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}

		break;

	case MODE_TUTORIAL:		//チュートリアル画面

		////チュートリアル画面の破棄
		//if (m_pTutorial != NULL)
		//{
		//	//終了処理
		//	m_pTutorial->Uninit();
		//	m_pTutorial = NULL;
		//}

		break;

	case MODE_GAME:			//ゲーム画面

		//ゲーム画面の破棄
		if (m_pGame != NULL)
		{
			//終了処理
			m_pGame->Uninit();
			m_pGame = NULL;
		}

		break;

	case MODE_RESULT:		//リザルト画面

		//リザルト画面の破棄
		if (m_pResult != NULL)
		{
			//終了処理
			m_pResult->Uninit();
			m_pResult = NULL;
		}

		break;

	case MODE_RANKING:		//ランキング画面

		//ランキング画面の破棄
		if (m_pRanking != NULL)
		{
			//終了処理
			m_pRanking->Uninit();
			m_pRanking = NULL;
		}

		break;

	default:

		// 停止する
		assert(false);

		break;
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//シーンの更新処理
//==============================================================
void CScene::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	switch (m_mode)
	{
	case MODE_TITLE:		//タイトル画面

		pDebugProc->Print("<<<<<<タイトル>>>>>>\n");

		pDebugProc->Print("\n～デバッグ用操作方法～\n");
		pDebugProc->Print("[ENTER]：画面遷移\n");

		break;

	case MODE_TUTORIAL:		//チュートリアル画面

		pDebugProc->Print("<<<<<<チュートリアル>>>>>>\n");

		break;

	case MODE_GAME:			//ゲーム画面

		pDebugProc->Print("<<<<<<ゲーム>>>>>>\n");

		pDebugProc->Print("\n～デバッグ用操作方法～\n");
		pDebugProc->Print("[1]：画面遷移\n");

		break;

	case MODE_RESULT:		//リザルト画面

		pDebugProc->Print("<<<<<<リザルト>>>>>>\n");

		pDebugProc->Print("\n～デバッグ用操作方法～\n");
		pDebugProc->Print("[ENTER]：画面遷移\n");

		break;

	case MODE_RANKING:		//ランキング画面

		pDebugProc->Print("<<<<<<ランキング>>>>>>\n");

		pDebugProc->Print("\n～デバッグ用操作方法～\n");
		pDebugProc->Print("[ENTER]：画面遷移\n");

		break;

	default:

		assert(false);

		break;
	}
}

//==============================================================
//シーンの描画処理
//==============================================================
void CScene::Draw(void)
{

}