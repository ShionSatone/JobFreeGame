//=============================================================================
//
//スコア処理 [score.cpp]
//Author:佐藤根詩音
//
//=============================================================================
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "number.h"
#include "texture.h"
#include "game.h"
//#include "UI_death.h"
//#include "UI_item.h"

//マクロ定義
#define PRIORITY				(7)						//優先順位
#define SCORE_POS_X				(880.0f)				//スコアのXの位置
#define SCORE_POS_Y				(50.0f)					//スコアのYの位置
#define RESULT_SCORE_POS_X		(440.0f)				//スコアのXの位置
#define RESULT_SCORE_POS_Y		(550.0f)				//スコアのYの位置
#define SCORE_WIDTH				(50.0f * 0.5f)			//スコアの横幅
#define SCORE_HEIGHT			(80.0f * 0.5f)			//スコアの縦幅

#define SCORE_INTER				(50.0f)					//スコアの間隔
#define RESULT_SCORE_INTER		(60.0f)					//リザルトのスコアの間隔
#define NUM_TEX					(10)					//テクスチャの数字の数

#define CLEAR_BONUS				(3150)					//クリアボーナス
#define NO_DEATH_BONUS			(188888)				//1回も死亡してないボーナス
#define DEATH_SCORE				(-1050)					//死んだときに引かれるスコア
#define ITEM_SCORE				(13000)					//アイテムスコア

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CScore::m_pTexture = NULL;			//テクスチャ
CNumber *CScore::m_apNumber[NUM_DIGIT] = {};
int CScore::m_nNum = 0;			//スコアの値
int CScore::m_aTexU[NUM_DIGIT] = {};

//==============================================================
//コンストラクタ
//==============================================================
CScore::CScore()
{
	//変数初期化
	m_nIdxTex = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_nNum = 0;		//スコアの値

	for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
	{
		m_aTexU[nCntScore] = 0;
	}
}

//==============================================================
//デストラクタ
//==============================================================
CScore::~CScore()
{

}

//==============================================================
//スコアのテクスチャの読み込み
//==============================================================
HRESULT CScore::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	if (m_pTexture == NULL)
	{//テクスチャ設定されてないとき

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\score002.png",
			&m_pTexture);
	}

	return S_OK;
}

//==============================================================
//スコアのテクスチャの破棄
//==============================================================
void CScore::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==============================================================
//生成処理
//==============================================================
CScore *CScore::Create(void)
{
	CScore *pScore = NULL;

	if (pScore == NULL)
	{//メモリが使用されてなかったら

		//スコアの生成
		pScore = new CScore;
	}

	if (pScore != NULL)
	{//メモリ確保できてたら

		//初期化処理
		pScore->Init();

		if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
		{//リザルトだったら
			
			m_nNum = pScore->Calculation();		//スコア
			CManager::GetInstance()->SetNumScore(m_nNum);		//スコア設定

		}

		//種類設定
		pScore->SetType(TYPE_SCORE);

		//スコアの設定
		pScore->Set(m_nNum);
	}

	return pScore;
}

//==============================================================
//スコアの初期化処理
//==============================================================
HRESULT CScore::Init(void)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	m_nIdxTex = pTexture->Regist("data\\TEXTURE\\number02.png");

	//初期化処理
	for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] == NULL)
		{//使用されてないとき

			//数字生成
			m_apNumber[nCntScore] = CNumber::Create();

			if (m_apNumber[nCntScore] != NULL)
			{//生成出来たら

				//大きさ設定
				m_apNumber[nCntScore]->SetSize(SCORE_WIDTH, SCORE_HEIGHT);

				//地上テクスチャ割り当て
				m_apNumber[nCntScore]->BindTexture(m_nIdxTex);

				if (CManager::GetInstance()->GetMode() == CScene::MODE_RESULT)
				{//リザルトだったら

					//スコアの位置設定
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(RESULT_SCORE_POS_X + (nCntScore * RESULT_SCORE_INTER), RESULT_SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				}
				else
				{
					//スコアの位置設定
					m_apNumber[nCntScore]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(SCORE_POS_X + (nCntScore * SCORE_INTER), SCORE_POS_Y, 0.0f), SCORE_WIDTH, SCORE_HEIGHT);
				}
			}
		}
	}

	return S_OK;
}

//==============================================================
//スコアの終了処理
//==============================================================
void CScore::Uninit(void)
{
	for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
	{
		if (m_apNumber[nCntScore] != NULL)
		{//使用されてるとき

			//終了処理
			m_apNumber[nCntScore]->Uninit();
			m_apNumber[nCntScore] = NULL;

		}
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//スコアの更新処理
//==============================================================
void CScore::Update(void)
{
	
}

//==============================================================
//スコアの描画処理
//==============================================================
void CScore::Draw(void)
{
	
}

//==============================================================
//スコアの設定処理
//==============================================================
void CScore::Set(int nNum)
{
	int nDigit;		//桁数
	m_nNum = nNum;	//スコア代入

	//スコア初期化
	for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_DIGIT - nCntScore));		//桁数

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//スコアのテクスチャ設定
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//スコアの加算処理
//==============================================================
void CScore::Add(int nValue)
{
	int nDigit;		//桁数
	m_nNum += nValue;	//スコア加算

	//スコア初期化
	for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
	{
		nDigit = (int)pow(10, (NUM_DIGIT - nCntScore));		//桁数

		m_aTexU[nCntScore] = (int)(m_nNum % nDigit / (nDigit * 0.1f));

		//スコアのテクスチャ設定
		m_apNumber[nCntScore]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);
	}
}

//==============================================================
//テクスチャ割り当て処理
//==============================================================
void CScore::BindTexture(LPDIRECT3DTEXTURE9 pTexture)
{
	m_pTexture = pTexture;		//テクスチャ割り当て
}

//==============================================================
//スコアの計算処理
//==============================================================
int CScore::Calculation(void)
{
	int nNumDeath = CManager::GetInstance()->GetNumDeath();		//死亡数取得
	int nNumItem = CManager::GetInstance()->GetNumItem();		//アイテム数取得
	int nBonus = 0;		//ボーナススコア

	nBonus += CLEAR_BONUS;		//クリアボーナス

	if(nNumDeath <= 0)
	{//1回も死んでない

		nBonus += NO_DEATH_BONUS;		//死んでないボーナス
	}
	else if (nNumItem <= 0 && nNumDeath > 0)
	{//アイテム取得してない && 死亡したとき

		return nBonus;
	}

	nBonus = (nNumItem * ITEM_SCORE) + (nNumDeath * DEATH_SCORE) + nBonus;

	return nBonus;
}