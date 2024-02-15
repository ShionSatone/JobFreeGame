//==============================================================
//
//配置処理[edit.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "edit.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "input.h"
#include "object.h"
#include "model.h"
#include "debugproc.h"
#include "playerModel.h"

//マクロ定義
#define MODEL_LEN			(3.0f)		//距離
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define MOVE_MODEL			(50.0f)		//モデルの移動速度
#define FILE_MAP			"data\\TEXT\\map.txt"		//マップの配置情報ファイル

//静的メンバ変数宣言
LPD3DXMESH CEdit::m_pMesh = NULL;						//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER CEdit::m_pBuffMat = NULL;					//マテリアルへのポインタ
DWORD CEdit::m_dwNumMat = NULL;
const char *CEdit::m_apFilename[MAX_MODEL] = {};		//モデルの配置のファイル名
int CEdit::m_nNumAll = 0;			//モデルの総数
CObjectX *CEdit::m_apObjectX[MAX_MODEL] = {};

//==============================================================
//コンストラクタ
//==============================================================
CEdit::CEdit()
{
	//モデルの構造体初期化
	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		m_aEdit[nCntEdit].dwNumMat = NULL;
		m_aEdit[nCntEdit].pBuffMat = NULL;		//マテリアルへのポインタ
		m_aEdit[nCntEdit].pMesh = NULL;			//メッシュ(頂点情報)へのポインタ
		m_aEdit[nCntEdit].pTexture = NULL;		//テクスチャのポインタ
		m_aEdit[nCntEdit].vtxMin = D3DXVECTOR3(100000.0f, 100000.0f, 100000.0f);		//頂点数の最小値
		m_aEdit[nCntEdit].vtxMax = D3DXVECTOR3(-100000.0f, -100000.0f, -100000.0f);		//頂点数の最大値
		m_aEdit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		m_aEdit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		m_aEdit[nCntEdit].nIndex = -1;			//モデル番号

		m_aSetEdit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		m_aSetEdit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		m_aSetEdit[nCntEdit].nIndex = -1;

		for (int nCntName = 0; nCntName < MAX_STR; nCntName++)
		{
			m_aEdit[nCntEdit].aFilename[nCntName] = NULL;			//ファイル名
			m_aEdit[nCntEdit].aFilecomment[nCntName] = NULL;		//ファイルコメント
		}
	}

	//m_pTexture = NULL;			//テクスチャへのポインタ
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//向き
	m_nNumModel = 0;			//モデルの番号
	m_nMaxModel = 0;			//モデルの設置最大数
}

//==============================================================
//デストラクタ
//==============================================================
CEdit::~CEdit()
{
	
}

//==============================================================
//Xファイルの読み込み
//==============================================================
HRESULT CEdit::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得

	CEdit::LoadFile();		//ファイル読み込み

	for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		//Xファイルの読み込み
		D3DXLoadMeshFromX(&m_aEdit[nCntEdit].aFilename[0],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_aEdit[nCntEdit].pBuffMat,
			NULL,
			&m_aEdit[nCntEdit].dwNumMat,
			&m_aEdit[nCntEdit].pMesh);
	}

	return S_OK;
}

//==============================================================
//マテリアル・メッシュの破棄
//==============================================================
void CEdit::Unload(void)
{
	for (int nCntEdit = 0; nCntEdit < MAX_MODEL; nCntEdit++)
	{
		//メッシュの破棄
		if (m_aEdit[nCntEdit].pMesh != NULL)
		{
			m_aEdit[nCntEdit].pMesh->Release();
			m_aEdit[nCntEdit].pMesh = NULL;
		}

		//マテリアルの破棄
		if (m_aEdit[nCntEdit].pBuffMat != NULL)
		{
			m_aEdit[nCntEdit].pBuffMat->Release();
			m_aEdit[nCntEdit].pBuffMat = NULL;
		}
	}
}

//==============================================================
//モデルの配置の生成処理
//==============================================================
CEdit *CEdit::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CEdit *pEdit = NULL;

	if (pEdit == NULL)
	{//メモリが使用されてなかったら

		//モデルの配置の生成
		pEdit = new CEdit;
	}

	//初期化処理
	pEdit->Init(pos, rot);

	return pEdit;
}

//==============================================================
//モデルの配置の初期化処理
//==============================================================
HRESULT CEdit::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATERIAL *pMat;		//マテリアルへのポインタ

	m_pos = pos;

	//モデルの配置情報の読み込み
	CEdit::Load();

	for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		m_aEdit[nCntEdit].pTexture = new LPDIRECT3DTEXTURE9[m_aEdit[nCntEdit].dwNumMat];

		//マテリアル情報に対するポインタを取得
		pMat = (D3DXMATERIAL*)m_aEdit[nCntEdit].pBuffMat->GetBufferPointer();

		for (int nCntMat = 0; nCntMat < (int)m_aEdit[nCntEdit].dwNumMat; nCntMat++)
		{
			//if (m_pTexture[nCntMat] != NULL)
			{
				if (pMat[nCntMat].pTextureFilename != NULL)
				{//テクスチャが存在する
					D3DXCreateTextureFromFile(pDevice,
						pMat[nCntMat].pTextureFilename,
						&m_aEdit[nCntEdit].pTexture[nCntMat]);
				}
				else
				{//テクスチャが存在しない
					m_aEdit[nCntEdit].pTexture[nCntMat] = NULL;
				}
			}
		}
	}

	//大きさ設定
	CEdit::SetSize();

	return S_OK;
}

//==============================================================
//モデルの配置の終了処理
//==============================================================
void CEdit::Uninit(void)
{
	CEdit::Unload();		//マテリアル・メッシュの破棄

	//for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	//{
	//	if (m_apObjectX[nCntEdit] != NULL)
	//	{//使用されてたら

	//		//オブジェクトXの終了処理
	//		m_apObjectX[nCntEdit]->Uninit();
	//	}
	//}

	//終了処理
	for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		//for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
		//{
		//	if (m_pTexture[nCntMat] != NULL)
		//	{
		//		m_pTexture[nCntMat]->Release();
		//		m_pTexture[nCntMat] = NULL;
		//	}
		//}
#if 0 // リークしてるぞ。
		for (int nCntMat = 0; nCntMat < (int)m_aEdit[nCntEdit].dwNumMat; nCntMat++)
		{
			if (m_aEdit[nCntEdit].pTexture != NULL)
			{
				if (m_aEdit[nCntEdit].pTexture[nCntMat] != NULL)
				{
					m_aEdit[nCntEdit].pTexture[nCntMat]->Release();
					m_aEdit[nCntEdit].pTexture[nCntMat] = NULL;
				}

				delete[] m_aEdit[nCntEdit].pTexture;
				m_aEdit[nCntEdit].pTexture = NULL;
			}
		}
#endif
	}

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//モデルの配置の更新処理
//==============================================================
void CEdit::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetDebugProc();					//デバッグ表示の情報取得

	m_aEdit[m_nNumModel].pos = m_pos;

	//操作処理
	CEdit::Control();

	pDebugProc->Print("モデルの番号：%d\n", m_nNumModel);
	pDebugProc->Print("モデルの位置：(%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//==============================================================
//モデルの配置の操作処理
//==============================================================
void CEdit::Control(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInputKeyboard();		//キーボードの情報取得
	CPlayerModel *pPlayerModel = CManager::GetPlayerModel();			//プレイヤーの情報取得

	if (pInputKeyboard->GetTrigger(DIK_2) == true)
	{//2キー押したら

		if (m_nNumAll - 1 > m_nNumModel && 0 <= m_nNumModel)
		{//モデルの番号が最大値未満の場合

			m_nNumModel++;		//次のモデルに変更する
		}
		else if (m_nNumAll - 1 >= m_nNumModel)
		{//モデルの番号が最大数の場合

			m_nNumModel = 0;	//最初のモデルに変更する
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_1) == true)
	{//1キー押したら

		if (m_nNumAll - 1 >= m_nNumModel && 0 < m_nNumModel)
		{//モデルの番号が最小値より大きいの場合

			m_nNumModel--;		//次のモデルに変更する
		}
		else if (0 <= m_nNumModel)
		{//モデルの番号が最小値の場合

			m_nNumModel = m_nNumAll - 1;	//最後のモデルに変更する
		}
	}

	if (pInputKeyboard->GetPress(DIK_RIGHT) == true)
	{//→キーを押したら

		//if (pInputKeyboard->GetTrigger(DIK_UP) == true)
		//{//↑キーを押したら

		//	//右奥
		//	m_pos.x += sinf(m_rot.y + D3DX_PI * 0.25f) * MOVE_MODEL;
		//	m_pos.y += cosf(m_rot.y + D3DX_PI * 0.25f) * MOVE_MODEL;
		//}
		//else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		//{//↓キーを押したら

		//	//右手前
		//	m_pos.x += sinf(m_rot.y + D3DX_PI * 0.75f) * MOVE_MODEL;
		//	m_pos.y += cosf(m_rot.y + D3DX_PI * 0.75f) * MOVE_MODEL;
		//}
		/*else */if (pInputKeyboard->GetTrigger(DIK_RIGHT) == true)
		{
			//右
			m_pos.x += sinf(m_rot.y + D3DX_PI * 0.5f) * MOVE_MODEL;
			m_pos.y += cosf(m_rot.y + D3DX_PI * 0.5f) * MOVE_MODEL;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_LEFT) == true)
	{//←キーを押したら

		//if (pInputKeyboard->GetTrigger(DIK_UP) == true)
		//{//↑キーを押したら

		//	//左奥
		//	m_pos.x += sinf(m_rot.y + -D3DX_PI * 0.25f) * MOVE_MODEL;
		//	m_pos.y += cosf(m_rot.y + -D3DX_PI * 0.25f) * MOVE_MODEL;
		//}
		//else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
		//{//↓キーを押したら

		//	//左手前
		//	m_pos.x += sinf(m_rot.y + -D3DX_PI * 0.75f) * MOVE_MODEL;
		//	m_pos.y += cosf(m_rot.y + -D3DX_PI * 0.75f) * MOVE_MODEL;
		//}
		/*else */if (pInputKeyboard->GetTrigger(DIK_LEFT) == true)
		{
			//左
			m_pos.x += sinf(m_rot.y + -D3DX_PI * 0.5f) * MOVE_MODEL;
			m_pos.y += cosf(m_rot.y + -D3DX_PI * 0.5f) * MOVE_MODEL;
		}
	}
	else if (pInputKeyboard->GetTrigger(DIK_UP) == true)
	{//↑キーを押したら

		//m_pos.z += MOVE_MODEL;		//奥に進む

		m_pos.y += MOVE_MODEL;		//上に上げる

	}
	else if (pInputKeyboard->GetTrigger(DIK_DOWN) == true)
	{//↓キーを押したら

		//m_pos.z += -MOVE_MODEL;		//手前に進む

		m_pos.y += -MOVE_MODEL;		//下に下げる

	}
	//else if (pInputKeyboard->GetPress(DIK_U) == true)
	//{//Uキーを押したら

	//	m_pos.y += 1.0f;		//上に上げる
	//}
	//else if (pInputKeyboard->GetPress(DIK_M) == true)
	//{//Mキーを押したら

	//	m_pos.y += -1.0f;		//下に下げる
	//}

	if (pInputKeyboard->GetTrigger(DIK_RETURN) == true)
	{//ENTERキーを押したら

		//モデルの設定処理
		CEdit::SetModel();
	}

	if (pInputKeyboard->GetTrigger(DIK_F9) == true)
	{//F9キーを押したら

		//セーブ処理
		CEdit::SaveFile();
	}

	if (pInputKeyboard->GetTrigger(DIK_DELETE) == true)
	{//DELETEキーを押したら

		//モデル削除
		CEdit::Delete();
	}

	if (pInputKeyboard->GetTrigger(DIK_0) == true)
	{//位置モデルの奥行リセット

		m_pos.z = 0.0f;
	}

	if (pInputKeyboard->GetTrigger(DIK_9) == true)
	{//位置モデルの高さリセット

		if (pPlayerModel->GetType() == pPlayerModel->PLAYERTYPE_HUMAN)
		{//人の時

			m_pos.y = 0.0f;

		}
		else if (pPlayerModel->GetType() == pPlayerModel->PLAYERTYPE_GOLDFISH)
		{//金魚のとき

			m_pos.y = -5000.0f;
		}
	}
}

//==============================================================
//モデルの配置の描画処理
//==============================================================
void CEdit::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		//デバイスの取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;

	//ワールドマトリックスを初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	//マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_aEdit[m_nNumModel].pBuffMat->GetBufferPointer();

	for (int nCntMat = 0; nCntMat < (int)m_aEdit[m_nNumModel].dwNumMat; nCntMat++)
	{
		//マテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		//テクスチャの設定
		pDevice->SetTexture(0, m_aEdit[m_nNumModel].pTexture[nCntMat]);

		//モデル（パーツ）の描画
		m_aEdit[m_nNumModel].pMesh->DrawSubset(nCntMat);
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);

	//オブジェクトXの描画処理
	/*for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		m_apObjectX[nCntEdit]->Draw();
	}*/
}

//==============================================================
//モデルの設定処理
//==============================================================
void CEdit::SetModel(void)
{
	//モデル生成
	CModel::Create(m_pos, m_rot, &m_aEdit[m_nNumModel].aFilename[0]);

	m_aSetEdit[m_nMaxModel].pos = m_pos;
	m_aSetEdit[m_nMaxModel].rot = m_rot;
	m_aSetEdit[m_nMaxModel].nIndex = m_nNumModel;

	m_nMaxModel++;		//最大設置数加算
}

//==============================================================
//モデルの大きさ設定処理
//==============================================================
void CEdit::SetSize(void)
{
	int nNumVtx;					//頂点数
	DWORD dwSizeFVF;				//頂点フォーマットのサイズ
	BYTE *pVtxBuff;					//頂点バッファへのポインタ

	for (int nCntEdit = 0; nCntEdit < m_nNumAll; nCntEdit++)
	{
		//頂点数を取得
		nNumVtx = m_aEdit[nCntEdit].pMesh->GetNumVertices();

		//頂点フォーマットのサイズを取得
		dwSizeFVF = D3DXGetFVFVertexSize(m_aEdit[nCntEdit].pMesh->GetFVF());

		//頂点バッファをロック
		m_aEdit[nCntEdit].pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;			//頂点座標の代入

			//全ての頂点を比較してモデルの最大値を抜き出す
			if (vtx.x > m_aEdit[nCntEdit].vtxMax.x)
			{
				m_aEdit[nCntEdit].vtxMax.x = vtx.x;
			}
			else if (vtx.y > m_aEdit[nCntEdit].vtxMax.y)
			{
				m_aEdit[nCntEdit].vtxMax.y = vtx.y;
			}
			else if (vtx.z > m_aEdit[nCntEdit].vtxMax.z)
			{
				m_aEdit[nCntEdit].vtxMax.z = vtx.z;
			}

			//全ての頂点を比較してモデルの最小値を抜き出す
			if (vtx.x < m_aEdit[nCntEdit].vtxMin.x)
			{
				m_aEdit[nCntEdit].vtxMin.x = vtx.x;
			}
			else if (vtx.y < m_aEdit[nCntEdit].vtxMin.y)
			{
				m_aEdit[nCntEdit].vtxMin.y = vtx.y;
			}
			else if (vtx.z < m_aEdit[nCntEdit].vtxMin.z)
			{
				m_aEdit[nCntEdit].vtxMin.z = vtx.z;
			}

			pVtxBuff += dwSizeFVF;								//頂点フォーマットのサイズ分ポインタを進める
		}

		//頂点バッファをアンロック
		m_aEdit[nCntEdit].pMesh->UnlockVertexBuffer();
	}
}

//==============================================================
//モデルの配置情報保存処理
//==============================================================
void CEdit::SaveFile(void)
{
	FILE *pFile;		//ファイルポインタ

	pFile = fopen(FILE_MAP, "w");

	if (pFile != NULL)
	{//ファイルが開けたら

		//ヘッダコメント
		fprintf(pFile, "#=======================================================================\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "# マップ配置情報ファイル [map.txt]\n");
		fprintf(pFile, "# Author;佐藤根詩音\n");
		fprintf(pFile, "#\n");
		fprintf(pFile, "#=======================================================================\n");

		//モデル数
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# モデル数\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "NUM_MODEL = %d\n", m_nNumAll);

		fprintf(pFile, "\n");		//改行

		//モデルファイル名
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# モデルファイル名\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		for (int nCntModel = 0; nCntModel < m_nNumAll; nCntModel++)
		{
			fprintf(pFile, "MODEL_FILENAME = %s				# %s\n", &m_aEdit[nCntModel].aFilename[0], &m_aEdit[nCntModel].aFilecomment[0]);
		}

		fprintf(pFile, "\n");		//改行

		//モデル配置情報
		fprintf(pFile, "#------------------------------------------------------------------------------\n");
		fprintf(pFile, "# モデル配置情報\n");
		fprintf(pFile, "#------------------------------------------------------------------------------\n");

		for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
		{
			fprintf(pFile, "MODELSET\n");
			fprintf(pFile, "	INDEX = %d						# モデル番号\n", m_aSetEdit[nCntModel].nIndex);
			fprintf(pFile, "	POS = %.1f %.1f %.1f\n", m_aSetEdit[nCntModel].pos.x, m_aSetEdit[nCntModel].pos.y, m_aSetEdit[nCntModel].pos.z);
			fprintf(pFile, "END_MODELSET\n\n");
		}

		//ファイル閉じる
		fclose(pFile);
	}
}

//==============================================================
//モデルの配置情報読み込み処理
//==============================================================
void CEdit::LoadFile(void)
{
	FILE *pFile;		//ファイルポインタ
	char aString[MAX_STR];		//文字代入
	int nCntNum = 0, nCntName = 0;			//モデルの番号、ファイル名番号
	int nResult = 0;			//最後の行まで来たか

	pFile = fopen(FILE_MAP, "r");

	if (pFile != NULL)
	{//ファイルが開けたら

		//モデル数読み込み
		while (strcmp(&aString[0], "NUM_MODEL") != 0)
		{//モデル数を読み込むまでの間

			fscanf(pFile, "%s", &aString[0]);		//文字読み込み

			if (strcmp(&aString[0], "NUM_MODEL") == 0)
			{//[NUM_MODEL]を読み込んだら

				fscanf(pFile, "%s", &aString[0]);		//文字読み込み
				fscanf(pFile, "%d", &m_nNumAll);		//モデル数読み込み

				break;
			}
		}

		//モデルのファイル名読み込み
		while (1)
		{
			fscanf(pFile, "%s", &aString[0]);		//文字読み込み

			if (strcmp(&aString[0], "MODEL_FILENAME") == 0)
			{//[MODEL_FILENAME]を読み込んだら

				fscanf(pFile, "%s", &aString[0]);		//文字読み込み

				fscanf(pFile, "%s", &m_aEdit[nCntName].aFilename[0]);	//ファイル名読み込み
				fscanf(pFile, "%s", &aString[0]);		//文字読み込み

				fscanf(pFile, "%s", &m_aEdit[nCntName].aFilecomment[0]);	//ファイルコメント読み込み

				if (nCntName == m_nNumAll - 1)
				{//最大値と同じ番号の時

					break;
				}
				else
				{
					//次の番号に進む
					nCntName++;
				}
			}
		}

		//モデルの配置情報読み込み
		while (strcmp(&aString[0], "MODELSET") != 0 && nResult != EOF)
		{//[MODELSET]を読み込むまでの間 && 最後の行じゃない間

			nResult = fscanf(pFile, "%s", &aString[0]);		//文字読み込み

			if (strcmp(&aString[0], "MODELSET") == 0)
			{//[MODELSET]を読み込んだら

				while (strcmp(&aString[0], "END_MODELSET") != 0)
				{//[END_MODELSET]を読み込むまでの間

					fscanf(pFile, "%s", &aString[0]);		//文字読み込み

					if (strcmp(&aString[0], "INDEX") == 0)
					{//[INDEX]を読み込んだら

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み
						fscanf(pFile, "%d", &m_aSetEdit[m_nMaxModel].nIndex);		//モデル番号読み込み

					}
					else if (strcmp(&aString[0], "POS") == 0)
					{//[POS]を読み込んだら

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み

						//位置読み込み
						fscanf(pFile, "%f", &m_aSetEdit[m_nMaxModel].pos.x);
						fscanf(pFile, "%f", &m_aSetEdit[m_nMaxModel].pos.y);
						fscanf(pFile, "%f", &m_aSetEdit[m_nMaxModel].pos.z);

					}
				}

				m_nMaxModel++;		//モデルの最大設置数加算
			}
		}

		//ファイル閉じる
		fclose(pFile);
	}

	//モデル設置
	for (int nCntModel = 0; nCntModel < m_nMaxModel; nCntModel++)
	{
		//モデル生成
		CModel::Create(m_aSetEdit[nCntModel].pos, m_aSetEdit[nCntModel].rot, &m_aEdit[m_aSetEdit[nCntModel].nIndex].aFilename[0]);
	}
}

//==============================================================
//モデルの削除処理
//==============================================================
void CEdit::Delete(void)
{
	int nCnt;

	nCnt = CObjectX::CollisionEdit(m_aEdit[m_nNumModel].pos, m_aEdit[m_nNumModel].vtxMin, m_aEdit[m_nNumModel].vtxMax);

	m_aSetEdit[nCnt].nIndex = -1;

	//並び替え
	for (int nCntModel = 0; nCntModel < m_nMaxModel - 1; nCntModel++)
	{
		if (m_aSetEdit[nCntModel].nIndex == -1)
		{//消されたら

			m_aSetEdit[nCntModel].nIndex = m_aSetEdit[nCntModel + 1].nIndex;
			m_aSetEdit[nCntModel].pos = m_aSetEdit[nCntModel + 1].pos;
			m_aSetEdit[nCntModel].rot = m_aSetEdit[nCntModel + 1].rot;

			m_aSetEdit[nCntModel + 1].nIndex = -1;
		}
	}

	m_nMaxModel--;		//モデルの設置数減算
}