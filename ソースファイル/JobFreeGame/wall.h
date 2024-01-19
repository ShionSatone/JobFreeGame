//==============================================================
//
//壁の処理[wall.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _WALL_H_		//このマクロ定義がされていなかったら
#define _WALL_H_		//2重インクルード防止のマクロを定義する

#include "object3D.h"

//壁クラスの定義
class CWall : public CObject3D
{
public:
	CWall();		//コンストラクタ
	CWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//コンストラクタ(オーバーロード)
	~CWall();		//デストラクタ

	static CWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		//生成処理

	HRESULT Init(void);
	void UnInit(void);
	void Update(void);
	void Draw(void);

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;
	int m_nIdxTexture;		//テクスチャの番号

	D3DXVECTOR3 m_pos;		//位置
	int m_nType;			//壁の種類
};

#endif
