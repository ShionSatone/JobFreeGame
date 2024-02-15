//==============================================================
//
//オブジェクト処理[object.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "object.h"
#include "renderer.h"
#include "manager.h"
#include "camera.h"

//静的メンバ変数の宣言
CObject *CObject::m_apObject[MAX_OBJECT] = {};
int CObject::m_nNumAll = 0;		//全てのオブジェクトの総数

//==============================================================
//コンストラクタ
//==============================================================
CObject::CObject()
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == NULL)
		{//メモリが使用されてなかったら

			m_apObject[nCntObject] = this;		//自分自身を代入
			m_nID = nCntObject;		//自分自身のIDを保存
			m_nNumAll++;			//総数をカウントアップ

			break;
		}
	}
}

//==============================================================
//コンストラクタ(オーバーロード)
//==============================================================
CObject::CObject(TYPE type)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] == NULL)
		{//メモリが使用されてなかったら

			m_apObject[nCntObject] = this;		//自分自身を代入
			m_nID = nCntObject;		//自分自身のIDを保存
			m_type = type;			//オブジェクトの種類を設定
			m_nNumAll++;			//総数をカウントアップ

			break;
		}
	}
}

//==============================================================
//デストラクタ
//==============================================================
CObject::~CObject()
{

}

//==============================================================
//全てのオブジェクトの破棄
//==============================================================
void CObject::ReleaseAll(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != NULL)
		{//メモリが使用されてたら

			//終了処理
			m_apObject[nCntObject]->Uninit();
		}
	}
}

//==============================================================
//全てのオブジェクトの更新処理
//==============================================================
void CObject::UpdateAll(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != NULL)
		{//メモリ確保されてたら

			//更新処理
			m_apObject[nCntObject]->Update();
		}
	}
}

//==============================================================
//全てのオブジェクトの描画処理
//==============================================================
void CObject::DrawAll(void)
{
	CCamera *pCamera = CManager::GetCamera();

	//カメラの設定
	pCamera->SetCamera();

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != NULL)
		{//メモリ確保されてたら

			//描画処理
			m_apObject[nCntObject]->Draw();
		}
	}
}

//==============================================================
//オブジェクトの破棄
//==============================================================
void CObject::Release(void)
{
	int nID = m_nID;		//ID代入

	if (m_apObject[nID] != NULL)
	{//メモリが使用されてたら

		//メモリ開放
		delete m_apObject[nID];
		m_apObject[nID] = NULL;

		m_nNumAll--;		//総数をカウントダウン
	}
}

//==============================================================
//マップのオブジェクトのリセット処理
//==============================================================
void CObject::ResetMapAll(void)
{
	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nCntObject] != NULL)
		{//メモリが使用されてたら

			if (m_apObject[nCntObject]->m_type != TYPE_PLAYER &&
				m_apObject[nCntObject]->m_type != TYPE_NONE)
			{
				//終了処理
				m_apObject[nCntObject]->Uninit();
			}
		}
	}
}