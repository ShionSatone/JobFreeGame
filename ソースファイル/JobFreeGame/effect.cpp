//==============================================================
//
//エフェクトの処理[effect.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "texture.h"

//マクロ定義
#define RADIUS_EFFECT		(40.0f * 0.5f)		//エフェクトの半径
#define NUM_PATTERN			(10)				//パターン数
#define MOVE_SPEED			(3.0f)				//エフェクトの移動速度
#define DEST_EFFECT			(8)					//エフェクトが消滅するまでの時間
#define SUB_RADIUS			(0.5f)				//エフェクトの大きさ

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CEffect::m_pTexture = NULL;		//テクスチャ

//==============================================================
//コンストラクタ
//==============================================================
CEffect::CEffect()
{
	m_nIdxTexture = -1;		//テクスチャの番号
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	//色
	m_fRadius = RADIUS_EFFECT;			//半径（大きさ）
	m_nLife = DEST_EFFECT;		//エフェクト消滅の時間
}

//==============================================================
//デストラクタ
//==============================================================
CEffect::~CEffect()
{

}

//==============================================================
//エフェクトの生成処理
//==============================================================
CEffect *CEffect::Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type)
{
	CEffect *pEffect = NULL;

	if (pEffect == NULL)
	{//メモリが使用されてなかったら

		//エフェクトの生成
		pEffect = new CEffect;
	}

	//初期化処理
	pEffect->Init(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	//エフェクトの設定
	pEffect->Set(pos, move, col, fRadius, nLife, type);

	return pEffect;
}

//==============================================================
//エフェクトの初期化処理
//==============================================================
HRESULT CEffect::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect001.png");

	//テクスチャの割り当て
	CBillboard::BindTexture(m_nIdxTexture);

	//種類設定
	CBillboard::SetType(CObject::TYPE_EFFECT);

	//ビルボードの初期化処理
	CBillboard::Init();

	//位置設定
	CBillboard::SetPosition(pos);

	//エフェクトの大きさ設定
	CBillboard::SetSize(m_fRadius, m_fRadius);

	return S_OK;
}

//==============================================================
//エフェクトの終了処理
//==============================================================
void CEffect::Uninit(void)
{
	//ビルボードの終了処理
	CBillboard::Uninit();
}

//==============================================================
//エフェクトの更新処理
//==============================================================
void CEffect::Update(void)
{
	//消滅までの時間減算
	m_nLife--;

	//位置取得
	m_pos = CBillboard::GetPosition();

	//色取得
	m_col = CBillboard::GetColor();

	//移動量加算
	m_pos += m_move;

	//色減算
	m_col -= D3DXCOLOR(0.02f, 0.02f, 0.02f, 0.0f);

	//半径減算
	m_fRadius += -SUB_RADIUS;

	//位置設定
	CBillboard::SetPosition(m_pos);

	//サイズ設定
	CBillboard::SetSize(m_fRadius, m_fRadius);

	//色設定
	CBillboard::SetColor(m_col);

	//ビルボードの更新処理
	CBillboard::Update();

	//エフェクト削除
	if (m_nLife <= 0 && m_fRadius <= 0.0f)
	{//エフェクトが消滅する時間になったら

		//エフェクト削除
		CEffect::Release();
	}
}

//==============================================================
//エフェクトの描画処理
//==============================================================
void CEffect::Draw(void)
{
	//ビルボードの描画処理
	//if (m_type != PARTICLETYPE_ENEMY && m_type != PARTICLETYPE_LIFE && m_type != PARTICLETYPE_MOVE)
	/*{
		CBillboard::SetDraw();
	}
	else*/
	{
		CBillboard::Draw();

	}
}

//==============================================================
//エフェクトの設定処理
//==============================================================
void CEffect::Set(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, int nLife, int type)
{
	//変数代入
	m_pos = pos;
	m_move = move;
	m_col = col;
	m_fRadius = fRadius;
	m_nLife = nLife;
	m_type = type;

	CTexture *pTexture = CManager::GetInstance()->GetTexture();

	//テクスチャの読み込み
	switch (m_type)
	{
	case PARTICLETYPE_NONE:		//何もなし

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect004.png");

		break;

	case PARTICLETYPE_EXPLOSION:	//爆発

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

		break;

	case PARTICLETYPE_LIFE:			//回復

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect005.png");

		break;

	case PARTICLETYPE_MOVE:			//移動

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect005.png");

		break;

	default:		//その他

		m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\effect000.jpg");

		break;
	}

	//テクスチャの割り当て
	CBillboard::BindTexture(m_nIdxTexture);

	//位置設定
	CBillboard::SetPosition(m_pos);

	//色設定
	CBillboard::SetColor(m_col);
}