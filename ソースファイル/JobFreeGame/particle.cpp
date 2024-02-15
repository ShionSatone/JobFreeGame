//==============================================================
//
//パーティクルの処理[particle.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "effect.h"
#include "input.h"

//マクロ定義
#define MAX_PARTICLE		(512)				//パーティクルの最大数
#define WIDTH_PARTICLE		(40.0f * 0.5f)		//パーティクルの横幅
#define HEIGHT_PARTICLE		(40.0f * 0.5f)		//パーティクルの縦幅
#define RADIUS_PARTICLE		(40.0f * 0.5f)		//パーティクルの半径
#define NUM_PATTERN			(10)				//パターン数
#define MOVE_SPEED			(3.0f)				//パーティクルの移動速度
#define DEST_PARTICLE		(8)					//パーティクルが消滅するまでの時間
#define MOVE_WATER			(70)				//水の移動量
#define MOVE_ICE			(20)				//氷の移動量
#define MOVE_VAPOR			(10)				//蒸気の移動量
#define MOVE_EXPLOSION		(50)				//爆発の移動量
#define LIFE_ICE			(35)				//氷の寿命
#define LIFE_VAPOR			(50)				//蒸気の寿命

//静的メンバ変数宣言
LPDIRECT3DTEXTURE9 CParticle::m_pTexture = NULL;		//テクスチャ

//==============================================================
//コンストラクタ
//==============================================================
CParticle::CParticle()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//移動量
	m_col = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);		//色
	m_fRadius = 0.0f;			//半径（大きさ）
	m_type = PARTICLETYPE_NONE;				//種類
	m_nLife = DEST_PARTICLE;	//パーティクル消滅の時間
}

//==============================================================
//デストラクタ
//==============================================================
CParticle::~CParticle()
{

}

//==============================================================
//パーティクルの生成処理
//==============================================================
CParticle *CParticle::Create(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CParticle *pParticle = NULL;

	if (pParticle == NULL)
	{//メモリが使用されてなかったら

		//パーティクルの生成
		pParticle = new CParticle;
	}

	//初期化処理
	pParticle->Init();

	//設定処理
	pParticle->Set(pos, col, type, nLife, fRadius);

	return pParticle;
}

//==============================================================
//パーティクルの初期化処理
//==============================================================
HRESULT CParticle::Init(void)
{

	return S_OK;
}

//==============================================================
//パーティクルの終了処理
//==============================================================
void CParticle::Uninit(void)
{
	CObject::Release();
}

//==============================================================
//パーティクルの更新処理
//==============================================================
void CParticle::Update(void)
{
	
}

//==============================================================
//パーティクルの描画処理
//==============================================================
void CParticle::Draw(void)
{
	
}

//==============================================================
//パーティクルの設定処理
//==============================================================
void CParticle::Set(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	m_type = type;

	//パーティクル生成
	switch (m_type)
	{
	case PARTICLETYPE_NONE:		//何もなし

		break;

	case PARTICLETYPE_EXPLOSION:	//爆発

		CParticle::GenerateExplosion(pos, col, m_type, nLife, fRadius);

		break;

	case PARTICLETYPE_LIFE:			//回復

		CParticle::GenerateLife(pos, col, m_type, nLife, fRadius);

		break;

	case PARTICLETYPE_JUMP:			//ジャンプ

		CParticle::GenerateJump(pos, col, m_type, nLife, fRadius);

		break;

	case PARTICLETYPE_MOVE:			//歩き

		CParticle::GenerateMove(pos, col, m_type, nLife, fRadius);

		break;
	}

	//パーティクルの終了処理
	CParticle::Uninit();
}

//==============================================================
//水生成
//==============================================================
void CParticle::GenerateWater(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
	{
		//位置の設定
		m_pos = pos;

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{//↑キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 200 - 100) * 0.01f;
		}
		if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		{//↓キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 300 + 150) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{//←キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 100 - 170) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{//→キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 170 + 50) * 0.01f;
		}

		//移動量を求める
		m_fMove = (float)(rand() % MOVE_WATER) / 10.0f + 1.0f;

		//移動量の設定
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//色の設定
		m_col = col;

		//半径の設定
		m_fRadius = fRadius;

		//寿命の設定
		m_nLife = nLife;

		m_type = type;

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//氷生成
//==============================================================
void CParticle::GenerateIce(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 10; nCntAppear++)
	{
		//位置の設定
		m_pos = pos;

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{//↑キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 200 - 100) * 0.01f;
		}
		if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		{//↓キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 300 + 150) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{//←キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 100 - 170) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{//→キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 170 + 50) * 0.01f;
		}

		//移動量を求める
		m_fMove = (float)(rand() % MOVE_ICE) / 10.0f + 1.0f;

		//移動量の設定
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//色の設定
		m_col = col;

		//半径の設定
		m_fRadius = 30;

		//寿命の設定
		m_nLife = LIFE_ICE;

		m_type = type;

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//蒸気生成
//==============================================================
void CParticle::GenerateVapor(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 20; nCntAppear++)
	{
		//位置の設定
		m_pos = pos;

		if (pInputKeyboard->GetPress(DIK_UP) == true)
		{//↑キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 200 - 100) * 0.01f;
		}
		if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		{//↓キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 300 + 150) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{//←キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 100 - 170) * 0.01f;
		}

		if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{//→キーを押したら

			//角度を求める
			m_fAngle = (float)(rand() % 170 + 50) * 0.01f;
		}

		//移動量を求める
		m_fMove = (float)(rand() % MOVE_VAPOR) / 10.0f + 1.0f;

		//移動量の設定
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//色の設定
		m_col = col;

		//半径の設定
		m_fRadius = 10;

		//寿命の設定
		m_nLife = LIFE_VAPOR;

		m_type = type;

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//爆発生成
//==============================================================
void CParticle::GenerateExplosion(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{
		//if (m_type == CObject::TYPE_ENEMY)
		{//ダメージを受けた時

			//位置の設定
			m_pos = pos;

			//角度を求める
			m_fAngle = (float)(rand() % 629 - 314) / 1.0f;

			//移動量を求める
			m_fMove = (float)(rand() % MOVE_EXPLOSION) / 10.0f + 1.0f;

			//移動量の設定
			m_move.x = sinf(m_fAngle) * m_fMove;
			m_move.y = cosf(m_fAngle) * m_fMove;
			m_move.z = cosf(m_fAngle) * m_fMove;

			//色の設定
			m_col = col;

			//半径の設定
			m_fRadius = fRadius;

			//寿命の設定
			m_nLife = nLife;

			m_type = type;
		}

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//回復生成
//==============================================================
void CParticle::GenerateLife(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得

	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{
		//位置の設定
		m_pos = D3DXVECTOR3(pos.x + sinf(nCntAppear * 0.1f * D3DX_PI) * 30.0f, pos.y + (nCntAppear * 2), pos.z + cosf(nCntAppear * 0.1f * D3DX_PI) * 30.0f);

		//移動量の設定
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//色の設定
		m_col = col;

		//半径の設定
		m_fRadius = fRadius;

		//寿命の設定
		m_nLife = nLife;

		m_type = type;

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//ジャンプ生成
//==============================================================
void CParticle::GenerateJump(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 30; nCntAppear++)
	{
		//位置の設定
		m_pos = D3DXVECTOR3(pos.x, pos.y + 10.0f, pos.z - 10.0f);

		//角度を求める
		m_fAngle = (float)(rand() % 300 + 150) * 0.01f;

		//移動量を求める
		m_fMove = (float)(rand() % MOVE_EXPLOSION) / 10.0f + 1.0f;

		//移動量の設定
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//色の設定
		m_col = col;

		//半径の設定
		m_fRadius = fRadius;

		//寿命の設定
		m_nLife = nLife;

		m_type = type;

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}

//==============================================================
//移動パーティクル生成
//==============================================================
void CParticle::GenerateMove(D3DXVECTOR3 pos, D3DXCOLOR col, int type, int nLife, float fRadius)
{
	//パーティクルの生成
	for (int nCntAppear = 0; nCntAppear < 1; nCntAppear++)
	{
		//位置の設定
		m_pos = pos;

		//角度を求める
		m_fAngle = (float)(rand() % 629 - 314) / 1.0f;

		//移動量を求める
		m_fMove = (float)(rand() % MOVE_EXPLOSION) / 10.0f + 1.0f;

		//移動量の設定
		m_move.x = sinf(m_fAngle) * m_fMove;
		m_move.y = cosf(m_fAngle) * m_fMove;
		m_move.z = cosf(m_fAngle) * m_fMove;

		//色の設定
		m_col = col;

		//半径の設定
		m_fRadius = fRadius;

		//寿命の設定
		m_nLife = nLife;

		m_type = type;

		//エフェクトの生成
		CEffect::Create(m_pos,
			m_move,
			m_col,
			m_fRadius,
			m_nLife,
			m_type);
	}
}