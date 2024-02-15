//==============================================================
//
//配置処理[edit.h]
//Author:佐藤根詩音
//
//==============================================================
#ifndef _EDIT_H_		//このマクロ定義がされていなかったら
#define _EDIT_H_		//2重インクルード防止のマクロを定義する

#include"objectX.h"

//マクロ定義
#define MAX_DEBUG_MODEL			(1040)		//モデルの数
#define MAX_STR					(128)		//最大文字数

//前方宣言
class CObjectX;		//オブジェクトX

//モデル配置クラスの定義
class CEdit : CObject
{
public:

	//マップ配置の構造体
	struct Edit
	{
		LPD3DXMESH pMesh;			//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER pBuffMat;		//マテリアルへのポインタ
		DWORD dwNumMat;				//マテリアルの数
		LPDIRECT3DTEXTURE9 *pTexture;	//テクスチャへのポインタ
		D3DXMATRIX mtxWorld;			//ワールドマトリックス
		D3DXVECTOR3 vtxMin;				//モデルの最小値
		D3DXVECTOR3 vtxMax;				//モデルの最大値
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//向き
		int nIndex;				//モデルの番号
		int nType;				//ギミックの種類
		char aFilename[MAX_STR];			//モデルのファイル名
		char aFilecomment[MAX_STR];			//モデルのファイルコメント

	};

	//配置したモデルの構造体
	struct SetEdit
	{
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//向き
		int nIndex;				//モデルの番号
		int nType;				//ギミックの種類
	};

	CEdit();		//コンストラクタ
	~CEdit();		//デストラクタ

	HRESULT Load(void);		//テクスチャの読み込み
	void Unload(void);		//テクスチャの破棄
	static CEdit *Create(void);		//生成処理

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetSize(void);			//モデルの大きさ設定
	void LoadFile(void);		//データ読み込み

private:
	
	static int m_nNumAll;			//モデルの種類
	static CObjectX *m_apObjectX[MAX_DEBUG_MODEL];

	static LPD3DXMESH m_pMesh;			//メッシュ（頂点情報）へのポインタ
	static LPD3DXBUFFER m_pBuffMat;		//マテリアルへのポインタ
	static DWORD m_dwNumMat;			//マテリアルの数
	static const char *m_apFilename[MAX_DEBUG_MODEL];		//ファイル名

	static int m_nCnt;

	D3DXMATRIX m_mtxWorld;						//ワールドマトリックス
	D3DXVECTOR3 m_vtxMin;						//モデルの最小値
	D3DXVECTOR3 m_vtxMax;						//モデルの最大値

	D3DXVECTOR3 m_pos;			//位置
	D3DXVECTOR3 m_rot;			//向き
	int m_nNumModel;			//モデルの番号
	int m_nMaxModel;			//モデルの設置数

	Edit m_aEdit[MAX_DEBUG_MODEL];	//エディットの情報
	SetEdit m_aSetEdit[MAX_DEBUG_MODEL];	//設置したモデルの情報
};

#endif