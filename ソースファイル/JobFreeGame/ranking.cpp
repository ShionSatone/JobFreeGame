//==============================================================
//
//ランキング処理[ranking.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "ranking.h"
#include "manager.h"
#include "score.h"
#include "input.h"
//#include "bg.h"
#include "texture.h"
#include "number.h"
#include "game.h"
#include "fade.h"
#include "sound.h"

//マクロ定義
#define FILE_RANK		"data\\TEXT\\ranking.txt"		//ランキングファイル
#define SCORE_WIDTH		(50.0f * 0.5f)		//一桁の横の長さ
#define SCORE_HEIGHT	(100.0f * 0.5f)		//一桁の縦の長さ
#define SCORE_INTER_U	(50.0f)				//スコアの間隔(横)
#define SCORE_INTER_V	(100.0f)			//スコアの間隔(縦)
#define NUM_TEX			(10)				//テクスチャの数字の数
#define TRANS_TIME		(60 * 10)			//遷移するまでの時間

//静的メンバ変数宣言
bool CRanking::bReset = true;						//リセットしたかどうか
CNumber *CRanking::m_apNumber[NUM_DIGIT * MAX_RANK] = {};		//数字の情報
int CRanking::m_aTexU[NUM_DIGIT] = {};
int CRanking::m_nNum = 0;			//スコアの値
int CRanking::m_nRankUpdate = -1;		//更新ランクNo,

//==============================================================
//コンストラクタ
//==============================================================
CRanking::CRanking()
{
	m_nIdxTexture = -1;

	//桁数
	for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
	{
		m_aTexU[nCntScore] = 0;
	}

	//ランキング
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_ranking[nCntRank].nRanking = 0;			//スコア
		m_ranking[nCntRank].pos = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (SCORE_WIDTH * 2.0f * 4), 200.0f, 0.0f);	//位置
	}

	m_bBgm = false;			//BGM消すか
	m_nCntTrans = 0;		//遷移するまでの時間
	m_nCntColor = 0;		//ランキングの色変更間隔
	m_bCol = true;			//色変更するか
	m_nRankUpdate = -1;
}

//==============================================================
//デストラクタ
//==============================================================
CRanking::~CRanking()
{

}

//==============================================================
//初期化処理
//==============================================================
HRESULT CRanking::Init(void)
{
	D3DXVECTOR3 posScore = D3DXVECTOR3(SCREEN_WIDTH * 0.5f - (SCORE_WIDTH * 2.0f * 3.5f), 200.0f, 0.0f);	//位置
	m_nNum = CManager::GetInstance()->GetNumScore();		//スコア代入

	int nDigit;		//桁数

	//CBg::Create();

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//地上テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\number02.png");

	//リセット処理
	CRanking::Reset();

	//設定処理
	if (CManager::GetInstance()->GetBgm() == false)
	{
		CRanking::Set(m_nNum);
	}

	//初期化処理
	for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{//ランキングの数分

		for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
		{//スコアの桁数分

			int nCnt = nCntScore + (nCntRank * NUM_DIGIT);

			if (m_apNumber[nCnt] == NULL)
			{//使用されてないとき

				//数字生成
				m_apNumber[nCnt] = CNumber::Create();

				if (m_apNumber[nCnt] != NULL)
				{//生成出来たら

					nDigit = (int)pow(10, (NUM_DIGIT - nCntScore));		//桁数

					m_aTexU[nCntScore] = (int)(m_ranking[nCntRank].nRanking % nDigit / (nDigit * 0.1f));

					//スコアのテクスチャ設定
					m_apNumber[nCnt]->SetAnim(m_aTexU[nCntScore], 1.0f / NUM_TEX);

					//大きさ設定
					m_apNumber[nCnt]->SetSize(SCORE_WIDTH, SCORE_HEIGHT);

					//地上テクスチャ割り当て
					m_apNumber[nCnt]->BindTexture(m_nIdxTexture);

					//スコアの位置設定
					m_apNumber[nCnt]->SetPosition(CObject::TYPE_SCORE,
						D3DXVECTOR3(posScore.x + (nCntScore * SCORE_INTER_U), posScore.y + (nCntRank * SCORE_INTER_V), posScore.z), SCORE_WIDTH, SCORE_HEIGHT);
				}
			}
		}
	}

	return S_OK;
}

//==============================================================
//終了処理
//==============================================================
void CRanking::Uninit(void)
{
	CSound *pSound = CManager::GetInstance()->GetSound();

	if (CManager::GetInstance()->GetBgm() == false)
	{
		//BGMの停止
		pSound->Stop();
	}
	
	for (int nCntScore = 0; nCntScore < NUM_DIGIT * MAX_RANK; nCntScore++)
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
//更新処理
//==============================================================
void CRanking::Update(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();			//パッドの情報取得
	CFade *pFade = CManager::GetInstance()->GetFade();			//フェードの情報取得

	m_nCntColor++;

	if (((pInputKeyboard->GetTrigger(DIK_RETURN) == true || pInputJoyPad->GetTrigger(pInputJoyPad->BUTTON_A, 0) == true)) ||
		m_nCntTrans >= TRANS_TIME)
	{//ENTERキー押したら

		//タイトル画面
		pFade->SetFade(CScene::MODE_TITLE);
		bReset = true;
	}

	if (CManager::GetInstance()->GetBgm() == false)
	{
		if ((m_nCntColor % 40) == 0 && m_nRankUpdate >= 0)
		{//一定時間たったら

			m_bCol = m_bCol ? false : true;

			if (m_bCol == true)
			{
				//現在のスコアの色戻す
				for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
				{
					int nCnt = nCntScore + (m_nRankUpdate * NUM_DIGIT);

					m_apNumber[nCnt]->SetColor(CObject::TYPE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				}
			}
			else
			{
				//現在のスコアの色変更
				for (int nCntScore = 0; nCntScore < NUM_DIGIT; nCntScore++)
				{
					int nCnt = nCntScore + (m_nRankUpdate * NUM_DIGIT);

					m_apNumber[nCnt]->SetColor(CObject::TYPE_NONE, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

				}
			}

			m_nCntColor = 0;
		}
	}

	m_nCntTrans++;		//遷移するまでの時間
}

//==============================================================
//描画処理
//==============================================================
void CRanking::Draw(void)
{

}

//==============================================================
//ランキングの設定処理
//==============================================================
void CRanking::Set(int nNum)
{
	FILE *pFile;
	int nNewRanking;
	int nTemp;

	nNewRanking = nNum;

	if (nNewRanking > m_ranking[MAX_RANK - 1].nRanking)
	{//新しいスコアが最下位より高かったら

		m_ranking[MAX_RANK - 1].nRanking = nNewRanking;		//最下位に新しいスコア代入

		//現在の順位
		for (int nCntRank = 0; nCntRank < MAX_RANK - 1; nCntRank++)
		{
			//次の順位
			for (int nCntData = nCntRank + 1; nCntData < MAX_RANK; nCntData++)
			{
				if (m_ranking[nCntRank].nRanking < m_ranking[nCntData].nRanking)
				{//スコア高かったら

					//スコア入れ替え
					nTemp = m_ranking[nCntRank].nRanking;
					m_ranking[nCntRank].nRanking = m_ranking[nCntData].nRanking;
					m_ranking[nCntData].nRanking = nTemp;
				}
			}
		}

		//更新ランクNo.変更
		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			//ランキングスコアの並べ替え
			if (nNewRanking == m_ranking[nCntRank].nRanking)
			{
				m_nRankUpdate = nCntRank;
			}
		}
	}

	//ランキング保存
	pFile = fopen(FILE_RANK, "w");			//ファイル読み込み

	if (pFile != NULL)
	{//ファイルがある場合

		for (int nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{//データをセーブする

			fprintf(pFile, "%d\n", m_ranking[nCntRank].nRanking);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}

//==============================================================
//ランキングのリセット
//==============================================================
void CRanking::Reset(void)
{
	FILE *pFile;
	int nCntRank;

	//ランキング情報の初期設定
	for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
	{
		m_ranking[nCntRank].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の初期化
		m_ranking[nCntRank].nRanking = 0;								//ランキングの初期化
	}

	//ファイルを読み込む
	pFile = fopen(FILE_RANK, "r");

	if (pFile != NULL)
	{//ファイルがある場合

		for (nCntRank = 0; nCntRank < MAX_RANK; nCntRank++)
		{
			fscanf(pFile, "%d", &m_ranking[nCntRank].nRanking);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
}