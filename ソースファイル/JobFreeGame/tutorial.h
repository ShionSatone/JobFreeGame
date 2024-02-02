//==============================================================
//
//チュートリアル処理[tutorial.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TUTORIAL_H_		//このマクロ定義がされていなかったら
#define _TUTORIAL_H_		//2重インクルード防止のマクロを定義する

#include "manager.h"

//マクロ定義
#define NUM_TUTORIAL_TEX		(3)		//テクスチャの数

//前方宣言
class CScore;			//スコアの情報
class CPlayerModel;		//プレイヤーの情報
class CEnemyModel;		//敵の情報
class CModel;			//モデルの情報
class CWall;			//壁の情報
class CField;			//床の情報
class CEdit;			//エディットの情報
class CBulletRemain;	//弾数
class CObject2D;		//オブジェクト2D

//チュートリアル画面クラスの定義
class CTutorial : public CScene
{
public:

	//ステップ
	enum STEP
	{
		STEP_SYNOP_FIRST = 0,		//あらすじ1
		STEP_SYNOP_SECOND,			//あらすじ2
		STEP_SYNOP_THIRD,			//あらすじ3
		STEP_SYNOP_FORTH,			//あらすじ4
		STEP_MOVE,					//移動方法
		STEP_JUMP,					//ジャンプ方法
		STEP_BULLET,				//撃ち方
		STEP_RULE,					//ルール
		STEP_MAX
	};

	CTutorial();
	~CTutorial();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Step(void);		//ステップ処理

	static CPlayerModel *GetPlayerModel(void) { return m_pPlayerModel; }				//プレイヤーの情報
	static CBulletRemain *GetBulletRemain(void) { return m_pBulletRemain; }				//残弾数の情報
	static int GetStep(void) { return m_step; }
	D3DXVECTOR3 GetMove(void) { return m_move; }			//移動量の取得

private:
	static CScore *m_pScore;		//スコアの情報
	static CPlayerModel *m_pPlayerModel;	//プレイヤーの情報
	static CEnemyModel *m_pEnemyModel;		//敵の情報
	static CModel *m_pModel;		//モデルの情報
	static CWall *m_pWall;			//壁の情報
	static CField *m_pField;		//床の情報
	static CEdit *m_pEdit;			//エディットの情報
	static CBulletRemain *m_pBulletRemain;		//残弾数の情報

	static bool bReset;				//リセットしたか

	static char *m_apFileName[STEP_MAX];		//ファイル名

	static int m_step;		//現在のステップ

	static CObject2D *m_pObject2D[NUM_TUTORIAL_TEX];		//オブジェクト2Dの情報

	int m_nIdxTexture[NUM_TUTORIAL_TEX];			//テクスチャ番号
	D3DXVECTOR3 m_move;			//移動量
	bool m_bMoveL;				//左に移動した
	bool m_bMoveR;				//右に移動した
	bool m_bPad;				//パッドのスティックを倒したか
	int m_nNumBullet;			//撃った回数

};

#endif

