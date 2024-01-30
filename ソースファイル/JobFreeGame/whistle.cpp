//==============================================================
//
//笛の範囲処理[whistle.h]
//Author:佐藤根詩音
//
//==============================================================
#include "whistle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "player.h"
#include "texture.h"

//**************************************************************
//マクロ定義
//**************************************************************
#define PRIORITY			(3)			//優先順位
#define POS					(10.0f)		//pos初期値
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度

#define MOVE				(6.0f)		//移動量
#define DISTANCE			(500.0f)	//距離

#define HIT_CNT				(60 * 2)	//攻撃当たるまでのカウント数
#define DAMAGE_CNT			(9)			//ダメージカウント数
#define APP_CNT				(100)		//点滅時間

//**************************************************************
//静的メンバ変数宣言
//**************************************************************

//==============================================================
//コンストラクタ
//==============================================================
CWhistle::CWhistle()
{
	for (int nCnt = 0; nCnt < MAX_WHISTLE_TEX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;

	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CWhistle::CWhistle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_WHISTLE_TEX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;

	}
	m_pos = pos;									// 位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 前回の位置
}

//==============================================================
//デストラクタ
//==============================================================
CWhistle::~CWhistle()
{

}

//==============================================================
//ポイントの生成処理
//==============================================================
CWhistle* CWhistle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CWhistle* pWhistle = NULL;

	if (pWhistle == NULL)
	{//メモリが使用されてなかったら

		//ポイントの生成
		pWhistle = new CWhistle(pos, rot);
	}

	//初期化処理
	pWhistle->Init();

	return pWhistle;
}

//==============================================================
//ポイントの初期化処理
//==============================================================
HRESULT CWhistle::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//地上テクスチャの読み込み
	m_nIdxTexture[0] = pTexture->Regist("data\\TEXTURE\\point000.png");
	m_nIdxTexture[1] = pTexture->Regist("data\\TEXTURE\\point000.png");

	for (int nCnt = 0; nCnt < MAX_WHISTLE_TEX; nCnt++)
	{
		// テクスチャの割り当て
		CObject3D::BindTexture(m_nIdxTexture[nCnt]);
	}
	

	//オブジェクト3Dの初期化処理
	CObject3D::Init();

	//種類の設定
	CObject3D::SetType(CObject::TYPE_POINT);

	//位置の設定
	CObject3D::SetPos(m_pos);

	//ポイントの大きさ設定
	CObject3D::SetSize(D3DXVECTOR3(100.0f, 0.0f, 100.0f));

	return S_OK;
}

//==============================================================
//ポイントの終了処理
//==============================================================
void CWhistle::Uninit(void)
{
	//オブジェクト3Dの終了処理
	CObject3D::Uninit();
}

//==============================================================
//ポイントの更新処理
//==============================================================
void CWhistle::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//キーボードの情報取得
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//カメラの情報取得
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();

	//前回の位置更新
	m_posOld = m_pos;

	//当たり判定
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//デバッグ表示
	pDebugProc->Print("\nポイントの位置 (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//==============================================================
// 追尾処理
//==============================================================
void CWhistle::Follow(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// プレイヤーの情報取得
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();

	m_pos.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * DISTANCE;
	m_pos.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * DISTANCE;

	// 位置設定
	CObject3D::SetPos(m_pos);
}

//==============================================================
//ポイントの描画処理
//==============================================================
void CWhistle::Draw(void)
{
	// オブジェクト3Dの描画処理
	CObject3D::Draw();
}