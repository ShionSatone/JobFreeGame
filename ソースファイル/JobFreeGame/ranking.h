//==============================================================
//
//ランキング処理[ranking.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _RANKING_H_		//このマクロ定義がされていなかったら
#define _RANKING_H_		//2重インクルード防止のマクロを定義する

#include "manager.h"

//マクロ定義
#define MAX_RANK		(5)			//ランキング数
#define NUM_DIGIT		(8)			//スコアの桁数

//前方宣言
class CNumber;			//数字の情報

//ランキング画面クラスの定義
class CRanking : public CScene
{
public:
	//ランキング構造体
	typedef struct
	{
		D3DXVECTOR3 pos;			//位置
		int nRanking;				//スコア
	} Ranking;

	CRanking();
	~CRanking();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Reset(void);
	void Set(int nNum);
	void SetSound(bool bgm) { m_bBgm = bgm; }

private:
	static CNumber *m_apNumber[NUM_DIGIT * MAX_RANK];
	static bool bReset;				//リセットしたか
	static int m_nNum;				//スコアの値

	static int m_aTexU[NUM_DIGIT];

	static int m_nRankUpdate;		//更新ランクNo,

	int m_nIdxTexture;		//テクスチャの番号
	bool m_bBgm;			//BGM消すか
	bool m_bCol;			//色変更するか
	int m_nCntTrans;		//遷移するまでの時間
	int m_nCntColor;		//ランキングの色変更間隔
	Ranking m_ranking[MAX_RANK];		//ランキングの情報

};

#endif

