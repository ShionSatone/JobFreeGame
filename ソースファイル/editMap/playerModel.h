//==============================================================
//
//プレイヤーのモデル処理[playerModel.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _PLAYERMODEL_H_		//このマクロ定義がされていなかったら
#define _PLAYERMODEL_H_		//2重インクルード防止のマクロを定義する

#include "object.h"

class CModelHier;	//モデルの階層構造
class CMotion;		//モーションの階層構造

//プレイヤークラスの定義
class CPlayerModel : public CObject
{
public:

	//プレイヤーの種類
	typedef enum
	{
		PLAYERTYPE_HUMAN = 0,		//人間の姿
		PLAYERTYPE_GOLDFISH,		//金魚の姿
		PLAYERTYPE_MAX
	} PLAYERTYPE;

	//プレイヤーのパーツ
	typedef enum
	{
		PARTS_BODY = 0,		//[0]体
		PARTS_HEAD,			//[1]頭
		PARTS_HAIR,			//[2]お下げ
		PARTS_ARMUP_L,		//[3]左腕上
		PARTS_ARMDOWN_L,	//[4]左腕下
		PARTS_HAND_L,		//[5]左手
		PARTS_ARMUP_R,		//[6]右腕上
		PARTS_ARMDOWN_R,	//[7]右腕下
		PARTS_HAND_R,		//[8]右手
		PARTS_WAIST,		//[9]腰
		PARTS_SKIRT,		//[10]スカート
		PARTS_LEG_L,		//[11]左太もも
		PARTS_CALF_L,		//[12]左ふくらはぎ
		PARTS_FOOT_L,		//[13]左靴
		PARTS_LEG_R,		//[14]右太もも
		PARTS_CALF_R,		//[15]右ふくらはぎ
		PARTS_FOOT_R,		//[16]右靴
		PARTS_WEAPON,		//[17]武器(ッテッポウウオ)

		PARTS_MAX
	} PARTS;

	CPlayerModel();		//コンストラクタ
	CPlayerModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CPlayerModel();		//デストラクタ

	static CPlayerModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//生成処理

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void UpdateHuman(void);
	void UpdataFish(void);

	void ControlHuman(void);		//プレイヤー操作(人間)
	void ControlGoldFish(void);		//プレイヤー操作(金魚)
	void Screen(void);		//画面外判定
	void LoadFile(void);	//モデルファイル読み込み
	void RotCorrection(void);	//向きの補正処理

	D3DXVECTOR3 GetPosition(void) { return m_pos; }			//位置取得
	D3DXVECTOR3 GetRotation(void) { return m_rot; }			//向き取得
	PLAYERTYPE GetType(void) { return m_Type; }				//種類取得

private:
	static char *m_apFileName[PARTS_MAX];		//ファイル名
	static bool m_bStart;		//実行してるかどうか

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_posOld;	//前回の位置
	D3DXVECTOR3 m_posKeepHuman;	//人間の位置保存用
	D3DXVECTOR3 m_posKeepFish;	//金魚の位置保存用
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_rot;		//向き
	D3DXVECTOR3 m_max;		//人間の最大値
	D3DXVECTOR3 m_min;		//人間の最小値
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	CModelHier *m_apModel[PARTS_MAX];		//モデル(パーツ)へのポインタ
	int m_nNumModel;		//モデル(パーツ)の総数
	bool m_bJump;			//ジャンプしたかの判定
	bool m_bMove;			//歩いてるかの判定
	bool m_bLand;			//着地したか
	bool m_bAction;			//攻撃してるか

	float m_fRotDest;		//目標
	float m_fRotDiff;		//差分

	PLAYERTYPE m_Type;		//プレイヤーの種類

	CMotion *m_pMotion;		//モーション情報
};

#endif
