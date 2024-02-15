//==============================================================
//
//床の処理[field.h]
//Author:佐藤根詩音
//
//==============================================================
#include "field.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//マクロ定義
#define WIDTH_FIELD			(1000.0f * 0.5f)	//床の横幅
#define HEIGHT_FIELD		(1000.0f * 0.5f)	//床の縦幅
#define COL_R				(1.0f)		//赤色の数値
#define COL_G				(1.0f)		//緑色の数値
#define COL_B				(1.0f)		//青色の数値
#define COL_A				(1.0f)		//不透明度の数値
#define TEX_LEFT_X			(0.0f)		//テクスチャの左xの初期値
#define TEX_RIGHT_X			(1.0f)		//テクスチャの右xの初期値
#define TEX_UP_Y			(0.0f)		//テクスチャの上yの初期値
#define TEX_DOWN_Y			(1.0f)		//テクスチャの下yの初期値

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CField::m_pTexture = NULL;

//==============================================================
//コンストラクタ
//==============================================================
CField::CField()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CField::CField(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//位置

}

//==============================================================
//デストラクタ
//==============================================================
CField::~CField()
{

}

//==============================================================
//テクスチャの読み込み
//==============================================================
HRESULT CField::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得

	if (m_pTexture == NULL)
	{//テクスチャ設定されてないとき

		//テクスチャの読み込み
		D3DXCreateTextureFromFile(pDevice,
			"data\\TEXTURE\\000.jpg",
			&m_pTexture);
	}

	return S_OK;
}

//==============================================================
//テクスチャの破棄
//==============================================================
void CField::Unload(void)
{
	//テクスチャの破棄
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}

//==============================================================
//床の生成処理
//==============================================================
CField *CField::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CField *pField = NULL;

	if (pField == NULL)
	{//メモリが使用されてなかったら

		//床の生成
		pField = new CField(pos, rot);
	}

	//初期化処理
	pField->Init(pos, rot);

	return pField;
}

//==============================================================
//床の初期化処理
//==============================================================
HRESULT CField::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CTexture *pTexture = CManager::GetTexture();

	//テクスチャの読み込み
	m_nIdxTexture = pTexture->Regit("data\\TEXTURE\\meadow.jpg");

	//テクスチャの割り当て
	CObject3D::BindTexture(m_nIdxTexture);

	//オブジェクト3Dの初期化処理
	CObject3D::Init(pos, rot);

	//種類の設定
	CObject::SetType(CObject::TYPE_FIELD);

	//床の大きさ設定
	CObject3D::SetSize(WIDTH_FIELD, HEIGHT_FIELD);

	return S_OK;
}

//==============================================================
//床の終了処理
//==============================================================
void CField::UnInit(void)
{
	//オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//==============================================================
//床の更新処理
//==============================================================
void CField::Update(void)
{

}

//==============================================================
//床の描画処理
//==============================================================
void CField::Draw(void)
{
	//オブジェクト3Dの描画処理
	CObject3D::Draw();
}