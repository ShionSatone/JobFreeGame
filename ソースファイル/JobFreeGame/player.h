//==============================================================
//
// プレイヤーのモデル処理[player.h]
// Author:佐藤根詩音
//
//==============================================================
#ifndef _PLAYER_H_		// このマクロ定義がされていなかったら
#define _PLAYER_H_		// 2重インクルード防止のマクロを定義する

#include "object.h"

//**************************************************************
// 前方宣言
//**************************************************************
class CModelHier;	// モデルの階層構造
class CMotion;		// モーションの階層構造

//**************************************************************
// プレイヤークラスの定義
//**************************************************************
class CPlayer : public CObject
{
public:

	//プレイヤーの状態
	typedef enum
	{
		STATE_NONE = 0,		//通常状態
		STATE_ATTACK,		//攻撃状態
		STATE_DAMAGE,		//ダメージ状態
		STATE_DEATH,		//死亡状態
		STATE_APPEAR,		//点滅状態
		STATE_WAIT,			//待ち状態
		STATE_MAX
	} STATE;

	CPlayer();		// コンストラクタ
	CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// コンストラクタ(オーバーロード)
	~CPlayer();		// デストラクタ

	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		// 生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void Hit(void);

	// 設定処理
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// 位置設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// 向き設定
	void SetState(STATE state) { m_state = state; }			// 状態設定

	void SetDash(int nCntDash) { m_nDashCounter = nCntDash; }	// ダッシュ回数設定

	// 取得処理
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// 位置取得
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// 向き取得
	D3DXVECTOR3 GetMove(void) { return m_move; }			// 移動量取得

	//bool GetIsJump(void) { return m_bJump; }				// ジャンプの取得
	//bool GetIsMove(void) { return m_bMove; }				// 移動の取得
	//bool GetIsDash(void) { return m_bDash; }				// ダッシュの取得
	//bool GetIsLand(void) { return m_bLand; }				// 着地の取得

	D3DXVECTOR3 GetSizeMin(void) { return m_min; }			// 大きさの最大値取得
	D3DXVECTOR3 GetSizeMax(void) { return m_max; }			// 大きさの最小値取得

private:
	// プレイヤーのパーツ
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

	void MotionManager(void);				// モーション管理

	// 操作処理
	void ControlKeyboard(void);				// キーボード操作
	void ControlKeyboardMove(void);			// キーボードの移動操作
	//void ControlKeyboardInstructions(void);	// キーボードの指示操作
	//void ControlJoyPad(void);				// パッド操作
	//void ControltJoyPadMove(void);		// パッドの移動操作

	void Command(void);				// 命令処理

	void Screen(void);						// 画面外判定
	void LoadFile(void);					// モデルファイル読み込み
	void RotCorrection(void);				// 向きの補正処理
	void SEStep(void);				// 足音鳴らす処理
	void CollisionAction(void);		// 当たった時の処理

	static char *m_apFileName[PARTS_MAX];	// ファイル名

	D3DXVECTOR3 m_pos;		// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;		// 移動量
	D3DXVECTOR3 m_moveSave;	// 移動量保存用
	D3DXVECTOR3 m_rot;		// 向き
	D3DXVECTOR3 m_max;		// 最大値
	D3DXVECTOR3 m_min;		// 最小値
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	CModelHier *m_apModel[PARTS_MAX];		// モデル(パーツ)へのポインタ
	int m_nNumModel;		// モデル(パーツ)の総数
	float m_fRotDest;		// 目標
	float m_fRotDiff;		// 差分

	STATE m_state;			// プレイヤーの状態

	int m_nDashCounter;		// ダッシュした回数

	int m_particleType;		// パーティクルの種類
	int m_nType;			// 何番目のパーティクルか
	int m_nParticleLife;	// パーティクルの寿命
	int m_nCntHit;			// ヒットできるまでのカウンター

	bool m_bPad;			// パッドのスティックを倒してるか

	CMotion *m_pMotion;		// モーション情報
};

#endif
