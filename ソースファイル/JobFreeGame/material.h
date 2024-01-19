//==============================================================
//
//マテリアルの処理[texture.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _MATERIAL_H_		//このマクロ定義がされていなかったら
#define _MATERIAL_H_		//2重インクルード防止のマクロを定義する

#include "main.h"

//**************************************************************
// マクロ定義
//**************************************************************
#define MAX_TEX		(128)		//マテリアルの最大数

//**************************************************************
// マテリアルクラスの定義
//**************************************************************
class CMaterial
{
public:
	//マテリアルの種類
	enum MAT
	{
		//PARTS_BODY = 0,		//[0]体
		//PARTS_HEAD,			//[1]頭
		//PARTS_HAIR,			//[2]髪
		//PARTS_LU_ARM,		//[3]左腕上
		//PARTS_LD_ARM,		//[4]左腕下
		//PARTS_L_HAND,		//[5]左手
		//PARTS_RU_ARM,		//[6]右腕上
		//PARTS_RD_ARM,		//[7]右腕下
		//PARTS_R_HAND,		//[8]右手
		//PARTS_WAIST,		//[9]腰
		//PARTS_LU_LEG,		//[10]左太もも
		//PARTS_LD_LEG,		//[11]左ふくらはぎ
		//PARTS_L_SHOE,		//[12]左靴
		//PARTS_RU_LEG,		//[13]右太もも
		//PARTS_RD_LEG,		//[14]右ふくらはぎ
		//PARTS_R_SHOE,		//[15]右靴
		MAT_MAX
	};

	//モデルの構造体の定義
	struct Material
	{
		LPD3DXMESH pMesh;					//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER pBuffMat;				//マテリアルへのポインタ
		DWORD dwNumMat;						//マテリアルの数
		LPDIRECT3DTEXTURE9 *pTexture;
	};

	CMaterial();		//コンストラクタ
	~CMaterial();	//デストラクタ

	HRESULT Load(void);		//テクスチャ読み込み処理
	void Unload(void);		//テクスチャ破棄処理

	int Regist(const char *pFilename);							//テクスチャの番号指定
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
