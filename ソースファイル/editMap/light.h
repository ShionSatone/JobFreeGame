//==============================================================
//
//ライト処理[light.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _LIGHT_H_		//このマクロ定義がされていなかったら
#define _LIGHT_H_		//2重インクルード防止のマクロを定義する

#include"main.h"

//マクロ定義
#define MAX_LIGHT			(3)			//ライトの最大数

//ライトクラスの定義
class CLight
{
public:
	
	CLight();		//コンストラクタ
	~CLight();		//デストラクタ

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理

private:
	D3DLIGHT9 m_alight[MAX_LIGHT];		//ライトの情報
};

#endif
