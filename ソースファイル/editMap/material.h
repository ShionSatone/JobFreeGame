//==============================================================
//
//マテリアルの処理[texture.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MATERIAL_H_		//このマクロ定義がされていなかったら
#define _MATERIAL_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//マクロ定義
#define MAX_TEX		(128)		//マテリアルの最大数

//マテリアルクラスの定義
class CMaterial
{
public:
	//テクスチャの種類
	typedef enum
	{
		TEX_MAX = 0

	} TEX;

	//モデルの構造体の定義
	typedef struct
	{
		LPD3DXMESH pMesh;					//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER pBuffMat;				//マテリアルへのポインタ
		DWORD dwNumMat;						//マテリアルの数
		LPDIRECT3DTEXTURE9 *pTexture;
	} Material;


	CMaterial();		//コンストラクタ
	~CMaterial();	//デストラクタ

	HRESULT Load(void);		//テクスチャ読み込み処理
	void Unload(void);		//テクスチャ破棄処理

	int Regit(const char *pFilename);				//テクスチャの番号指定
	LPDIRECT3DTEXTURE9 GetTexAddress(int nIdx, int nMat);		//テクスチャの情報取得
	LPD3DXBUFFER GetMatAddress(int nIdx);		//マテリアルの情報取得
	DWORD GetNumMatAddress(int nIdx);			//マテリアルの数の情報取得
	LPD3DXMESH GetMeshAddress(int nIdx);		//メッシュの情報取得

private:
	static int m_nNumAll;		//テクスチャの数
	static const char *m_apFilename[MAX_TEX];		//モデルのテキストファイル名
	static Material m_Material[MAX_TEX];			//マテリアルの情報
};

#endif
