//==============================================================
//
//タイトル処理[title.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _TITLE_H_		//このマクロ定義がされていなかったら
#define _TITLE_H_		//2重インクルード防止のマクロを定義する

#include "manager.h"

#define NUM_TITLE_TEX		(2)		//タイトルのテクスチャの数

//前方宣言
class CModel;			//モデルの情報
class CWall;			//壁の情報
class CField;			//床の情報
class CEdit;			//エディットの情報

//タイトル画面クラスの定義
class CTitle : public CScene
{
public:
	CTitle();
	~CTitle();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static CModel *m_pModel;		//モデルの情報
	static CWall *m_pWall;			//壁の情報
	static CField *m_pField;		//床の情報
	static CEdit *m_pEdit;			//エディットの情報

	static bool bReset;				//リセットしたかどうか
	int m_nIdxTexture[NUM_TITLE_TEX];			//テクスチャ番号
	int m_nCntTrans;				//遷移するまでの時間
};

#endif

