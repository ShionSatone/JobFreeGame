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
//#include "playerModel.h"
#include "game.h"

//マクロ定義
#define PRIORITY			(3)			//優先順位
#define MODEL_LEN			(3.0f)		//距離
#define CURVE_RL			(0.5f)		//左右の角度
#define CURVE_UP			(0.0f)		//上の角度
#define CURVE_DOWN			(1.0f)		//下の角度
#define MOVE_MODEL			(1.0f)		//モデルの移動速度
#define FILE_MAP			"data\\TXT\\map.txt"		//マップの配置情報ファイル

//静的メンバ変数宣言
LPD3DXMESH CEdit::m_pMesh = NULL;						//メッシュ（頂点情報）へのポインタ
LPD3DXBUFFER CEdit::m_pBuffMat = NULL;					//マテリアルへのポインタ
DWORD CEdit::m_dwNumMat = NULL;
const char *CEdit::m_apFilename[MAX_DEBUG_MODEL] = {};		//モデルの配置のファイル名
int CEdit::m_nNumAll = 0;			//モデルの総数
CObjectX *CEdit::m_apObjectX[MAX_DEBUG_MODEL] = {};
int CEdit::m_nCnt = 0;

//==============================================================
//コンストラクタ
//==============================================================
CEdit::CEdit()
{
	//モデルの構造体初期化
	for (int nCntEdit = 0; nCntEdit < MAX_DEBUG_MODEL; nCntEdit++)
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
		m_aEdit[nCntEdit].nType = -1;			//ギミックの種類

		m_aSetEdit[nCntEdit].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置
		m_aSetEdit[nCntEdit].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//向き
		m_aSetEdit[nCntEdit].nIndex = -1;
		m_aSetEdit[nCntEdit].nType = -1;

		for (int nCntName = 0; nCntName < MAX_STR; nCntName++)
		{
			m_aEdit[nCntEdit].aFilename[nCntName] = NULL;			//ファイル名
			m_aEdit[nCntEdit].aFilecomment[nCntName] = NULL;		//ファイルコメント
		}
	}

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
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

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
	for (int nCntEdit = 0; nCntEdit < MAX_DEBUG_MODEL; nCntEdit++)
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
CEdit *CEdit::Create(void)
{
	CEdit *pEdit = NULL;

	if (pEdit == NULL)
	{//メモリが使用されてなかったら

		//モデルの配置の生成
		pEdit = new CEdit;

	}

	//初期化処理
	pEdit->Init();

	m_nCnt++;

	return pEdit;
}

//==============================================================
//モデルの配置の初期化処理
//==============================================================
HRESULT CEdit::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスの取得

	//モデルの配置情報の読み込み
	CEdit::Load();

	//大きさ設定
	CEdit::SetSize();

	return S_OK;
}

//==============================================================
//モデルの配置の終了処理
//==============================================================
void CEdit::Uninit(void)
{
	

	//オブジェクト（自分自身の破棄）
	CObject::Release();
}

//==============================================================
//モデルの配置の更新処理
//==============================================================
void CEdit::Update(void)
{
	
}

//==============================================================
//モデルの配置の描画処理
//==============================================================
void CEdit::Draw(void)
{
	
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
			/*else */if (vtx.y > m_aEdit[nCntEdit].vtxMax.y)
			{
				m_aEdit[nCntEdit].vtxMax.y = vtx.y;
			}
			/*else */if (vtx.z > m_aEdit[nCntEdit].vtxMax.z)
			{
				m_aEdit[nCntEdit].vtxMax.z = vtx.z;
			}

			//全ての頂点を比較してモデルの最小値を抜き出す
			if (vtx.x < m_aEdit[nCntEdit].vtxMin.x)
			{
				m_aEdit[nCntEdit].vtxMin.x = vtx.x;
			}
			/*else */if (vtx.y < m_aEdit[nCntEdit].vtxMin.y)
			{
				m_aEdit[nCntEdit].vtxMin.y = vtx.y;
			}
			/*else */if (vtx.z < m_aEdit[nCntEdit].vtxMin.z)
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
					else if (strcmp(&aString[0], "TYPE") == 0)
					{//[TYPE]を読み込んだら

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み
						fscanf(pFile, "%d", &m_aSetEdit[m_nMaxModel].nType);		//モデル番号読み込み

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