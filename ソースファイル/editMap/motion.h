//==============================================================
//
//モーション処理[motion.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MOTION_H_		//このマクロ定義がされていなかったら
#define _MOTION_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_PARTS		(18)		//パーツ最大数
#define MAX_KEY			(32)		//キー最大数

//前方宣言
class CModelHier;			//モデルの階層構造

//モーションクラスの定義
class CMotion
{
public:
	//モーションの種類
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,		//待機
		MOTIONTYPE_MOVE,			//移動
		MOTIONTYPE_ACTION,			//アクション
		MOTIONTYPE_JUMP,			//ジャンプ
		MOTIONTYPE_LAND,			//着地

		MOTIONTYPE_MAX
	} MOTIONTYPE;

	//キーの構造体
	typedef struct
	{
		float fPosX;		//位置X
		float fPosY;		//位置Y
		float fPosZ;		//位置Z
		float fRotX;		//向きX
		float fRotY;		//向きY
		float fRotZ;		//向きZ

	} KEY;

	//キー情報の構造体
	typedef struct
	{
		int nFrame;		//再生フレーム
		KEY aKey[MAX_PARTS];	//各モデルのキー要素
	} KEY_INFO;

	//モーション情報の構造体
	typedef struct
	{
		bool bLoop;		//ループするかどうか
		int nNumKey;	//キーの総数
		KEY_INFO aKeInfo[MAX_KEY];		//キー情報

	} INFO;

	CMotion();		//コンストラクタ
	~CMotion();		//デストラクタ

	static CMotion *Create(void);		//生成処理

	void Set(int nType);	//設定処理
	void Init(void);		//初期化処理
	void Update(void);		//更新処理
	int GetType(void) { return m_nType; }
	bool IsFinish(void);
	void SetInfo(INFO info) { m_aInfo[m_nType] = info; }
	void SetModel(CModelHier **ppModel, int nNumModel) { m_ppModel = ppModel; }		//モデルの情報設定

	void FileLoad(void);

private:
	INFO m_aInfo[MOTIONTYPE_MAX];
	int m_nNumAll;
	int m_nType;		//モーションの種類
	bool m_bLoop;		//ループするか
	int m_NumKey;		//キー数
	int m_nKey;			//キー
	int m_nCounter;		//モーションカウンター
	bool m_bFinish;		//終了したかどうか
	
	CModelHier **m_ppModel;		//モデルへのポインタ
	int m_nNumModel;				//モデルの総数
};

#endif