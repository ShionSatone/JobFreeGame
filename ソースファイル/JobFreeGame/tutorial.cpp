//==============================================================
//
//チュートリアル処理[tutorial.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "tutorial.h"
#include "manager.h"
#include "wall.h"
#include "input.h"
#include "camera.h"
#include "sound.h"
#include "fade.h"
#include "object2D.h"
#include "texture.h"

#include "bg.h"

//静的メンバ変数宣言
CPlayerModel *CTutorial::m_pPlayerModel = NULL;	//プレイヤーの情報
CEdit *CTutorial::m_pEdit = NULL;				//エディットの情報
bool CTutorial::bReset = true;					//リセットしたかどうか
CBulletRemain *CTutorial::m_pBulletRemain = NULL;		//残弾数の情報
int CTutorial::m_step = STEP_SYNOP_FIRST;		//現在のステップ
CObject2D *CTutorial::m_pObject2D[NUM_TUTORIAL_TEX] = {};		//オブジェクト2Dの情報

char *CTutorial::m_apFileName[STEP_MAX] =
{
	"data\\TEXTURE\\tutorial00.jpg",
	"data\\TEXTURE\\tutorial01.jpg",
	"data\\TEXTURE\\tutorial02.jpg",
	"data\\TEXTURE\\tutorial03.jpg",
	"data\\TEXTURE\\tutorial00.png",
	"data\\TEXTURE\\tutorial01.png",
	"data\\TEXTURE\\tutorial02.png",
	"data\\TEXTURE\\tutorial04.jpg",
};

//==============================================================
//コンストラクタ
//==============================================================
CTutorial::CTutorial()
{
	m_step = STEP_SYNOP_FIRST;		//現在のステップ

	for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
	{
		m_nIdxTexture[nCntTex] = -1;		//テクスチャ番号
	}

	m_nNumBullet = 0;		//弾を撃った数
	m_bMoveR = false;		//右移動
	m_bMoveL = false;		//左移動
	m_bPad = false;				//パッドのスティックを倒したか
}

//==============================================================
//デストラクタ
//==============================================================
CTutorial::~CTutorial()
{

}

//==============================================================
//初期化処理
//==============================================================
HRESULT CTutorial::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CSound *pSound = CManager::GetInstance()->GetSound();

	//カメラの初期化処理
	pCamera->Init();

	CBg::Create();

	//BGM再生
	pSound->Play(pSound->SOUND_LABEL_BGM001);

	return S_OK;
}

//==============================================================
//終了処理
//==============================================================
void CTutorial::Uninit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	//BGMの停止
	pSound->Stop();

	//オブジェクト2Dの破棄
	/*for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
	{
		if (m_pObject2D[nCntTex] != NULL)
		{
			m_pObject2D[nCntTex]->Uninit();
			m_pObject2D[nCntTex] = NULL;
		}
	}*/

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//更新処理
//==============================================================
void CTutorial::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//パッドの情報取得
	CFade *pFade = CManager::GetInstance()->GetFade();			//フェードの情報取得

	//ステップ処理
	//CTutorial::Step();

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true/* || 
		pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true*/)
	{//ENTERキー押したら

		//ゲーム画面
		pFade->SetFade(CScene::MODE_GAME);

		//bReset = true;
	}
	/*else
	{
		bReset = false;
	}*/

}

//==============================================================
//描画処理
//==============================================================
void CTutorial::Draw(void)
{
	//オブジェクト2Dの描画処理
	/*for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
	{
		m_pObject2D[nCntTex]->Draw();
	}*/
}

//==============================================================
//ステップ処理
//==============================================================
//void CTutorial::Step(void)
//{
//	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
//	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//パッドの情報取得
//	CTexture *pTexture = CManager::GetInstance()->GetTexture();
//	CSound *pSound = CManager::GetInstance()->GetSound();
//
//	switch (m_step)
//	{
//	case STEP_SYNOP_FIRST:		//あらすじ1
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTERキーを押したら
//
//			//BGM再生
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//
//			for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
//			{
//				//テクスチャの読み込み
//				m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//				//テクスチャ割り当て
//				m_pObject2D[nCntTex]->BindTexture(m_nIdxTexture[nCntTex]);
//			}
//		}
//
//		break;
//
//	case STEP_SYNOP_SECOND:		//あらすじ2
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTERキーを押したら
//
//			m_step++;
//
//			//BGM再生
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
//			{
//				//テクスチャの読み込み
//				m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//				//テクスチャ割り当て
//				m_pObject2D[nCntTex]->BindTexture(m_nIdxTexture[nCntTex]);
//			}
//		}
//
//		break;
//
//	case STEP_SYNOP_THIRD:		//あらすじ3
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTERキーを押したら
//
//			//BGM再生
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//
//			for (int nCntTex = 0; nCntTex < NUM_TUTORIAL_TEX; nCntTex++)
//			{
//				//テクスチャの読み込み
//				m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//				//テクスチャ割り当て
//				m_pObject2D[nCntTex]->BindTexture(m_nIdxTexture[nCntTex]);
//			}
//		}
//
//		break;
//
//	case STEP_SYNOP_FORTH:		//あらすじ4
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTERキーを押したら
//
//			//BGM再生
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//
//			//テクスチャの読み込み
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//テクスチャ割り当て
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//
//			//位置設定
//			m_pObject2D[0]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(800.0f, 200.0f, 0.0f), 300.0f, 100.0f);
//			m_pObject2D[1]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(800.0f, 1000.0f, 0.0f), 300.0f, 100.0f);
//			m_pObject2D[2]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(800.0f, 1000.0f, 0.0f), 300.0f, 100.0f);
//
//			//エディットの生成
//			m_pEdit = CEdit::Create(D3DXVECTOR3(0.0f, 1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
//
//			//プレイヤーの生成
//			m_pPlayerModel = CPlayerModel::Create(D3DXVECTOR3(0.0f, 5.0f, 0.0f), D3DXVECTOR3(0.0f, -0.5f * D3DX_PI, 0.0f));
//
//			//地上の生成
//			CWall::Create(D3DXVECTOR3(300.0f, 100.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0);
//		}
//
//		break;
//
//	case STEP_MOVE:		//移動方法
//
//		if (pInputKeyboard->GetTrigger(DIK_D) == true ||
//			pInputJoyPad->GetPressLX(0).x > 0.0f)
//		{//Dキーを押したら
//
//			m_bMoveR = true;		//右移動した状態にする
//			
//		}
//		else if (pInputKeyboard->GetTrigger(DIK_A) == true ||
//			pInputJoyPad->GetPressLX(0).x < 0.0f)
//		{//Dキーを押したら
//
//			m_bMoveL = true;		//右移動した状態にする
//
//		}
//
//		if (m_bMoveR == true && m_bMoveL == true)
//		{//移動したら
//
//			m_step++;
//
//			//テクスチャの読み込み
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//テクスチャ割り当て
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//		}
//
//		break;
//
//	case STEP_JUMP:		//ジャンプ方法
//
//		if (pInputKeyboard->GetTrigger(DIK_SPACE) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_B, 0) == true)
//		{//SPACEキーを押したら
//
//			m_step++;
//
//			//テクスチャの読み込み
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//テクスチャ割り当て
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//		}
//
//		break;
//
//	case STEP_BULLET:		//撃ち方
//
//		if (pInputJoyPad->GetPressRX(0).y == 0.0f && pInputJoyPad->GetPressRX(0).x == 0.0f)
//		{
//			m_bPad = false;		//スティックを倒してない状態にする
//		}
//
//		if (pInputKeyboard->GetTrigger(DIK_J) == true ||
//			pInputKeyboard->GetTrigger(DIK_I) == true ||
//			pInputKeyboard->GetTrigger(DIK_K) == true ||
//			pInputKeyboard->GetTrigger(DIK_L) == true ||
//			pInputKeyboard->GetTrigger(DIK_U) == true ||
//			pInputKeyboard->GetTrigger(DIK_M) == true ||
//			pInputKeyboard->GetTrigger(DIK_O) == true ||
//			pInputKeyboard->GetTrigger(DIK_PERIOD) == true ||
//			(pInputJoyPad->GetPressRX(0).x < 0.0f && m_bPad == false) ||
//			(pInputJoyPad->GetPressRX(0).x > 0.0f && m_bPad == false) ||
//			(pInputJoyPad->GetPressLX(0).y > 0.0f && m_bPad == false) ||
//			(pInputJoyPad->GetPressLX(0).y < 0.0f && m_bPad == false))
//		{//Jキーを押したら
//
//			m_bPad = true;
//			m_nNumBullet++;
//		}
//
//		if (m_nNumBullet >= 5)
//		{
//			m_step++;
//
//			//テクスチャの読み込み
//			m_nIdxTexture[0] = pTexture->Regist(m_apFileName[m_step]);
//
//			//テクスチャ割り当て
//			m_pObject2D[0]->BindTexture(m_nIdxTexture[0]);
//
//			//位置設定
//			m_pObject2D[0]->SetPosition(CObject::TYPE_NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
//			m_pObject2D[1]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(1180.0f, 680.0f, 0.0f), 90.0f, 45.0f);
//			m_pObject2D[2]->SetPosition(CObject::TYPE_EFFECT, D3DXVECTOR3(1100.0f, 60.0f, 0.0f), 150.0f, 40.0f);
//
//		}
//
//		break;
//
//	case STEP_RULE:		//ルール
//
//		if (pInputKeyboard->GetTrigger(DIK_RETURN) == true ||
//			pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)
//		{//ENTERキーを押したら
//
//			//BGM再生
//			pSound->Play(pSound->SOUND_LABEL_SE_ENTER001);
//
//			m_step++;
//		}
//
//		break;
//	}
//}