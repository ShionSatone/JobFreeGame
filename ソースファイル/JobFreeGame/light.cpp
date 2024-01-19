//==============================================================
//
//ライト処理[light.h]
//Author:佐藤根詩音
//
//==============================================================
#include "light.h"
#include "manager.h"
#include "renderer.h"

//==============================================================
//コンストラクタ
//==============================================================
CLight::CLight()
{
	//ライトの情報をクリアする
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));
}

//==============================================================
//デストラクタ
//==============================================================
CLight::~CLight()
{

}

//==============================================================
//ライトの初期化処理
//==============================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得
	D3DXVECTOR3 vecDir[MAX_LIGHT];				//設定用方向ベクトル

	//ライトの情報をクリアする
	ZeroMemory(&m_aLight, sizeof(D3DLIGHT9));

	//ライトの種類を設定
	for (int nCntLight = 0; nCntLight < MAX_LIGHT; nCntLight++)
	{
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		if (nCntLight == 0)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライトの方向を設定
			vecDir[nCntLight] = D3DXVECTOR3(0.2f, -0.8f, 0.8f);
		}
		else if (nCntLight == 1)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライトの方向を設定
			vecDir[nCntLight] = D3DXVECTOR3(0.2f, -0.8f, -1.0f);
		}
		else if (nCntLight == 2)
		{
			//ライトの拡散光を設定
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//ライトの方向を設定
			vecDir[nCntLight] = D3DXVECTOR3(-0.2f, -0.8f, 0.4f);
		}
		
		D3DXVec3Normalize(&vecDir[nCntLight], &vecDir[nCntLight]);		//ベクトルを正規化する
		m_aLight[nCntLight].Direction = vecDir[nCntLight];

		//ライトを設定する
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		//ライトを有効にする
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//==============================================================
//ライトの終了処理
//==============================================================
void CLight::Uninit(void)
{

}

//==============================================================
//ライトの更新処理
//==============================================================
void CLight::Update(void)
{
	
}