//==============================================================
//
// 春春ルクミンの処理[lucmin_spring.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _LUCMIN_SPRING_H_		// このマクロ定義がされていなかったら
#define _LUCMIN_SPRING_H_		// 2重インクルード防止のマクロを定義する

#include "lucmin.h"

//**************************************************************
// マクロ定義
//**************************************************************

//**************************************************************
// 前方宣言
//**************************************************************
class CModelHier;	//モデルの階層構造
class CMotion;		//モーションの階層構造

//**************************************************************
// 春ルクミンクラスの定義
//**************************************************************
class CLucminSpring : public CLucmin
{
public:

	CLucminSpring();		//コンストラクタ
	CLucminSpring(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CLucminSpring();		//デストラクタ

	static CLucminSpring *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetNumAll(void) { return m_nNumAll; }			// 春ルクミンの総数

private:

	//春ルクミンのモーション状態
	enum MOTIONSTATE
	{
		MOTIONSTATE_NONE = 0,		// 何もしてない状態
		MOTIONSTATE_MOVE,			// 歩いてる状態
		MOTIONSTATE_JUMP,			// ジャンプ状態
		MOTIONSTATE_LAND,			// 着地状態
		MOTIONSTATE_MAX
	};

	//春ルクミンのパーツ
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

	void MotionManager(void);		// モーション管理

	void LoadFile(void);					// モデルファイル読み込み
											   
	static char *m_apFileName[PARTS_MAX];		// ファイル名
	static int m_nNumAll;						// 春ルクミンの総数
	int m_nIndex;								// 春ルクミンの番号

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

	MOTIONSTATE m_MotionState;		// 春ルクミンの動きの状態
	CMotion *m_pMotion;		// モーション情報
};

#endif
