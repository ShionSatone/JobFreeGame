//==============================================================
//
//モーション処理[motion.cpp]
//Author:佐藤根詩音
//
//==============================================================
#include "motion.h"
#include "modelHierarchy.h"
#include "manager.h"
#include "debugproc.h"

//マクロ定義
#define FILE_NAME		"data\\TEXT\\motion_weapon.txt"		//モーションテキスト
#define MAX_STR			(128)		//文字の最大数

//==============================================================
//コンストラクタ
//==============================================================
CMotion::CMotion()
{
	for (int nCntMotion = 0; nCntMotion < MOTIONTYPE_MAX; nCntMotion++)
	{
		m_aInfo[nCntMotion].bLoop = false;		//ループするか
		m_aInfo[nCntMotion].nNumKey = 0;		//キーの総数

		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY; nCntKeyInfo++)
		{
			m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].nFrame = 0;		//フレーム数

			for (int nCntKey = 0; nCntKey < MAX_PARTS; nCntKey++)
			{
				//位置・向き初期化
				m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fPosX = 0.0f;
				m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fPosY = 0.0f;
				m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fPosZ = 0.0f;
				m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fRotX = 0.0f;
				m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fRotY = 0.0f;
				m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fRotZ = 0.0f;
			}
		}

	}

	m_nNumAll = 0;
	m_nType = MOTIONTYPE_NEUTRAL;		//モーションの種類
	m_bLoop = false;					//ループするか
	m_NumKey = 0;			//キー数
	m_nKey = 0;				//キー
	m_nCounter = 0;			//モーションカウンター
	m_bFinish = false;		//終了したかどうか

	m_ppModel = NULL;		//モデルへのポインタ
	m_nNumModel = MAX_PARTS;		//モデルの総数
}

//==============================================================
//デストラクタ
//==============================================================
CMotion::~CMotion()
{

}

//==============================================================
//設定処理
//==============================================================
void CMotion::Set(int nType)
{
	m_nType = nType;		//モーションの種類
	m_nCounter = 0;			//モーションカウンター
	m_NumKey = 0;			//キー数
	m_bLoop = false;		//ループ
	m_nKey = 0;				//キー
	m_bFinish = false;		//終了してない状態にする

							//全モデル(パーツ)の初期設定
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		//パーツの位置を設定
		m_ppModel[nCntModel]->SetPosition(D3DXVECTOR3(
			m_aInfo[m_nType].aKeInfo[0].aKey[nCntModel].fPosX,
			m_aInfo[m_nType].aKeInfo[0].aKey[nCntModel].fPosY,
			m_aInfo[m_nType].aKeInfo[0].aKey[nCntModel].fPosZ));

		//パーツの向きを設定
		m_ppModel[nCntModel]->SetRotation(D3DXVECTOR3(
			m_aInfo[m_nType].aKeInfo[0].aKey[nCntModel].fRotX,
			m_aInfo[m_nType].aKeInfo[0].aKey[nCntModel].fRotY,
			m_aInfo[m_nType].aKeInfo[0].aKey[nCntModel].fRotZ));
	}
}

//==============================================================
//生成処理
//==============================================================
CMotion *CMotion::Create(void)
{
	CMotion *pMotion = NULL;

	if (pMotion == NULL)
	{//メモリが使用されてなかったら

	 //モーションの生成
		pMotion = new CMotion;
	}

	return pMotion;
}

//==============================================================
//初期化処理
//==============================================================
void CMotion::Init(void)
{
	//モーション読み込み
	CMotion::FileLoad();

	//モーションの設定
	CMotion::Set(MOTIONTYPE_NEUTRAL);
}

//==============================================================
//更新処理
//==============================================================
void CMotion::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetDebugProc();

	D3DXVECTOR3 posDiff, rotDiff;		//差分
	D3DXVECTOR3 posDest, rotDest;		//目標

	m_NumKey = m_aInfo[m_nType].nNumKey;		//キー数代入
	m_bLoop = m_aInfo[m_nType].bLoop;			//ループするか

												//全モデル(パーツ)の更新
	for (int nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		//位置の差分
		posDiff.x = m_aInfo[m_nType].aKeInfo[(m_nKey + 1) % m_NumKey].aKey[nCntModel].fPosX - m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fPosX;
		posDiff.y = m_aInfo[m_nType].aKeInfo[(m_nKey + 1) % m_NumKey].aKey[nCntModel].fPosY - m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fPosY;
		posDiff.z = m_aInfo[m_nType].aKeInfo[(m_nKey + 1) % m_NumKey].aKey[nCntModel].fPosZ - m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fPosZ;

		//向きの差分
		rotDiff.x = m_aInfo[m_nType].aKeInfo[(m_nKey + 1) % m_NumKey].aKey[nCntModel].fRotX - m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fRotX;
		rotDiff.y = m_aInfo[m_nType].aKeInfo[(m_nKey + 1) % m_NumKey].aKey[nCntModel].fRotY - m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fRotY;
		rotDiff.z = m_aInfo[m_nType].aKeInfo[(m_nKey + 1) % m_NumKey].aKey[nCntModel].fRotZ - m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fRotZ;

		//位置の目標
		posDest.x = m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fPosX + posDiff.x * ((float)m_nCounter / (float)m_aInfo[m_nType].aKeInfo[m_nKey].nFrame);
		posDest.y = m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fPosY + posDiff.y * ((float)m_nCounter / (float)m_aInfo[m_nType].aKeInfo[m_nKey].nFrame);
		posDest.z = m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fPosZ + posDiff.z * ((float)m_nCounter / (float)m_aInfo[m_nType].aKeInfo[m_nKey].nFrame);

		//向きの差分
		rotDest.x = m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fRotX + rotDiff.x * ((float)m_nCounter / (float)m_aInfo[m_nType].aKeInfo[m_nKey].nFrame);
		rotDest.y = m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fRotY + rotDiff.y * ((float)m_nCounter / (float)m_aInfo[m_nType].aKeInfo[m_nKey].nFrame);
		rotDest.z = m_aInfo[m_nType].aKeInfo[m_nKey].aKey[nCntModel].fRotZ + rotDiff.z * ((float)m_nCounter / (float)m_aInfo[m_nType].aKeInfo[m_nKey].nFrame);

		//初期値の分加算
		posDest += m_ppModel[nCntModel]->GetDefaultPosition();
		rotDest += m_ppModel[nCntModel]->GetDefaultRotation();

		//パーツの位置・向きを設定
		m_ppModel[nCntModel]->SetPosition(posDest);
		m_ppModel[nCntModel]->SetRotation(rotDest);
	}

	if (m_nCounter < m_aInfo[m_nType].aKeInfo[m_nKey].nFrame)
	{//モーションカウンターがフレーム数に達してないとき

		m_nCounter++;		//現在のキー加算
	}
	else
	{//モーションカウンターがフレーム数に達したら

		m_nKey++;			//現在のキー加算

		if (m_nKey >= m_NumKey)
		{//キーが最大のキー数を超えてたら

			m_nKey = 0;		//初期化

			if (m_bLoop == false)
			{//ループしないとき

				m_bFinish = true;		//終了した状態にする
			}
		}

		m_nCounter = 0;		//初期化
	}
}

//==============================================================
//モーションが終了したかの判定
//==============================================================
bool CMotion::IsFinish(void)
{
	return m_bFinish;
}

//==============================================================
//モーション読み込み
//==============================================================
void CMotion::FileLoad(void)
{
	FILE *pFile;
	char aString[MAX_STR];
	int nLoop = 0;			//ループ情報
	int nCntMotion = 0, nCntKeyInfo = 0, nCntKey = 0;		//モーションの数、キー情報の数、キー要素
	int nResult = 0;		//最後の行まで来たか

	//ファイル開く
	pFile = fopen(FILE_NAME, "r");

	if (pFile != NULL)
	{//ファイルが開けたら

		while (nResult != EOF)
		{//最後の行じゃない間

			while (strcmp(&aString[0], "MOTIONSET") != 0 && nResult != EOF)
			{//[MOTIONSET]がくるまでの間 && テキストの終わりが来ない間

				nResult = fscanf(pFile, "%s", &aString[0]);		//文字読み込み
			}

			if (strcmp(&aString[0], "MOTIONSET") == 0)
			{//[MOTIONSET]が来たら

				nCntKeyInfo = 0;		//キー情報の数リセット

				while (strcmp(&aString[0], "END_MOTIONSET") != 0)
				{//[END_MOTIONSET]がくるまでの間


					while (strcmp(&aString[0], "KEYSET") != 0)
					{//[KEYSET]がくるまでの間

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み

						if (strcmp(&aString[0], "LOOP") == 0)
						{//ループ情報

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み
							fscanf(pFile, "%d", &nLoop);		//ループ情報読み込み

							if (nLoop == 0)
							{//ループしない場合

								m_aInfo[nCntMotion].bLoop = false;		//ループしない状態にする
							}
							else if (nLoop == 1)
							{//ループする場合

								m_aInfo[nCntMotion].bLoop = true;		//ループする状態にする

							}
						}
						else if (strcmp(&aString[0], "NUM_KEY") == 0)
						{//キー数

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み

							fscanf(pFile, "%d", &m_aInfo[nCntMotion].nNumKey);		//キー数読み込み

						}
					}

					if (strcmp(&aString[0], "KEYSET") == 0)
					{//[KEYSET]が来たら

						nCntKey = 0;		//キー数リセット

						fscanf(pFile, "%s", &aString[0]);		//文字読み込み

						while (strcmp(&aString[0], "FRAME") != 0)
						{//[FRAME]が来るまでの間

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み

							if (strcmp(&aString[0], "FRAME") == 0)
							{
								fscanf(pFile, "%s", &aString[0]);		//文字読み込み
								fscanf(pFile, "%d", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].nFrame);		//フレーム数読み込み

								break;
							}
						}

						while (strcmp(&aString[0], "END_KEYSET") != 0)
						{//[END_KEY]が来るまでの間

							fscanf(pFile, "%s", &aString[0]);		//文字読み込み

							if (strcmp(&aString[0], "KEY") == 0)
							{//[KEY]が来たら

								while (strcmp(&aString[0], "END_KEY") != 0)
								{//[END_KEY]が来るまでの間

									fscanf(pFile, "%s", &aString[0]);		//文字読み込み

									if (strcmp(&aString[0], "POS") == 0)
									{//位置情報

										fscanf(pFile, "%s", &aString[0]);		//文字読み込み

										fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fPosX);		//位置読み込み
										fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fPosY);		//位置読み込み
										fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fPosZ);		//位置読み込み
									}
									else if (strcmp(&aString[0], "ROT") == 0)
									{//向き情報

										fscanf(pFile, "%s", &aString[0]);		//文字読み込み

										fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fRotX);		//向き読み込み
										fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fRotY);		//向き読み込み
										fscanf(pFile, "%f", &m_aInfo[nCntMotion].aKeInfo[nCntKeyInfo].aKey[nCntKey].fRotZ);		//向き読み込み
									}
								}
								nCntKey++;		//キー数加算
							}
						}
						nCntKeyInfo++;		//キー情報加算
					}

					fscanf(pFile, "%s", &aString[0]);		//文字読み込み
				}

				nCntMotion++;		//モーションの種類加算

				nResult = fscanf(pFile, "%s", &aString[0]);		//最後の行か確認
			}
		}

		//ファイル閉じる
		fclose(pFile);
	}
}