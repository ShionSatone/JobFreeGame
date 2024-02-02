//==============================================================
//
//リザルト処理[result.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _RESULT_H_		//このマクロ定義がされていなかったら
#define _RESULT_H_		//2重インクルード防止のマクロを定義する

#include "manager.h"

//リザルト画面クラスの定義
class CResult : public CScene
{
public:
	CResult();
	~CResult();

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:

};

#endif

