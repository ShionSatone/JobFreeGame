//==============================================================
//
// ルクミンの処理[lucmin.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _LUCMIN_H_		// このマクロ定義がされていなかったら
#define _LUCMIN_H_		// 2重インクルード防止のマクロを定義する

#include "object.h"
#include "game.h"

//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 前方宣言
//**************************************************************
class CModelHier;	//モデルの階層構造
class CMotion;		//モーションの階層構造

//**************************************************************
// ルクミンクラスの定義
//**************************************************************
class CLucmin : public CObject
{
public:

	// ルクミンの状態
	enum STATE
	{
		STATE_NONE = 0,		// 通常状態
		STATE_FOLLOW,		// 追尾状態
		STATE_THROW,		// 投げられてる状態
		STATE_SEARCH,		// 探す状態
		STATE_ATTACK,		// 攻撃状態
		STATE_WHISTLE,		// 呼ばれる状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX
	};

	//投げる状態
	enum THROWSTATE
	{
		THROWSTATE_NONE = 0,		// 何もない状態
		THROWSTATE_STANDBY,			// 準備状態
		THROWSTATE_THROW,			// 投げる状態
		THROWSTATE_MAX
	};

	CLucmin();		//コンストラクタ
	CLucmin(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CLucmin();		//デストラクタ

	static CLucmin *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// 設定処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	void SetState(STATE state) { m_state = state; }		// 状態設定

	//取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得

	D3DXVECTOR3 GetSizeMin(void) { return m_min; }		// 大きさの最大値取得
	D3DXVECTOR3 GetSizeMax(void) { return m_max; }		// 大きさの最小値取得
	STATE GetState(void) { return m_state; }			// 状態取得

private:

	//ルクミンのモーション状態
	enum MOTIONSTATE
	{
		MOTIONSTATE_NONE = 0,		// 何もしてない状態
		MOTIONSTATE_MOVE,			// 歩いてる状態
		MOTIONSTATE_JUMP,			// ジャンプ状態
		MOTIONSTATE_LAND,			// 着地状態
		MOTIONSTATE_MAX
	};

	//ルクミンのパーツ
	enum PARTS
	{
		PARTS_BODY = 0,		// [0]体
		PARTS_HEAD,			// [1]頭
		PARTS_HAIR,			// [2]髪
		PARTS_LU_ARM,		// [3]左腕上
		PARTS_LD_ARM,		// [4]左腕下
		PARTS_L_HAND,		// [5]左手
		PARTS_RU_ARM,		// [6]右腕上
		PARTS_RD_ARM,		// [7]右腕下
		PARTS_R_HAND,		// [8]右手
		PARTS_WAIST,		// [9]腰
		PARTS_LU_LEG,		// [10]左太もも
		PARTS_LD_LEG,		// [11]左ふくらはぎ
		PARTS_L_SHOE,		// [12]左靴
		PARTS_RU_LEG,		// [13]右太もも
		PARTS_RD_LEG,		// [14]右ふくらはぎ
		PARTS_R_SHOE,		// [15]右靴
		PARTS_MAX
	};

	void UpdateState(void);			// 状態の更新処理
	void UpdateThrowState(void);	// 投げられ状態の更新処理

	void MotionManager(void);		// モーション管理
	void FollowMove(void);			// プレイヤーについていく処理

	void Screen(void);						// 画面外判定
	void LoadFile(void);					// モデルファイル読み込み
	void RotNormalize(void);				// 向きの補正処理
											   
	static char *m_apFileName[PARTS_MAX];		// ファイル名
	static int m_nNumAll;						// ルクミンの総数
	int m_nIndex;								// ルクミンの番号

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_posDest;	// 目的の位置

	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_rotDest;	// 目的の向き
	D3DXVECTOR3 m_max;		// 人間の最大値
	D3DXVECTOR3 m_min;		// 人間の最小値
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	CModelHier *m_apModel[PARTS_MAX];		// モデル(パーツ)へのポインタ
	int m_nNumModel;		// モデル(パーツ)の総数
	int m_nCntDamage;		// ダメージカウンター

	float m_fRotDest;		// 目標
	float m_fRotDiff;		// 差分

	STATE m_state;			// ルクミンの状態
	THROWSTATE m_throwState;		// ルクミンの投げられ状態
	MOTIONSTATE m_MotionState;		// ルクミンの動きの状態

	CMotion *m_pMotion;		// モーション情報
};

#endif
