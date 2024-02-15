//==============================================================
//
//モデル処理[model.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MODEL_H_		//このマクロ定義がされていなかったら
#define _MODEL_H_		//2重インクルード防止のマクロを定義する

#include"objectX.h"

//マクロ定義
#define MAX_MODEL			(1028)		//モデルの数

class CModel : public CObjectX
{
public:
	CModel();		//コンストラクタ
	CModel(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//コンストラクタ(オーバーロード)
	~CModel();		//デストラクタ

	static HRESULT Load(const char *pFilename);		//テクスチャの読み込み
	//static void Unload(void);		//テクスチャの破棄
	static CModel *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *pFilename);		//生成処理

	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	static LPD3DXMESH m_pMesh;						//メッシュ（頂点情報）へのポインタ
	static LPD3DXBUFFER m_pBuffMat;					//マテリアルへのポインタ
	static DWORD m_dwNumMat;						//マテリアルの数
	static const char *m_pFilename;					//ファイルの名前

	int m_nIdxModel;		//モデルの番号

	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
};

#endif
