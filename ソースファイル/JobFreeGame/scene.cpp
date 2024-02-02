//==============================================================
//
// �V�[������ [scene.cpp]
// Author:����������
//
//==============================================================
#include "scene.h"
#include "manager.h"
#include "debugproc.h"
#include "title.h"
//#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "ranking.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define PRIORITY		(3)		//�D�揇��

//**************************************************************
// �ÓI�����o�ϐ��錾
//**************************************************************
CTitle *CScene::m_pTitle = NULL;			//�^�C�g����ʂ̏��
CTutorial *CScene::m_pTutorial = NULL;		//�`���[�g���A�����
CGame *CScene::m_pGame = NULL;				//�Q�[����ʂ̏��
CResult *CScene::m_pResult = NULL;			//���U���g��ʂ̏��
CRanking *CScene::m_pRanking = NULL;		//�����L���O��ʂ̏��

//==============================================================
//�V�[���̃R���X�g���N�^
//==============================================================
CScene::CScene()
{
	m_mode = MODE_TITLE;		//�Q�[�����[�h
}

//==============================================================
//�V�[���̃R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CScene::CScene(MODE mode)
{
	m_mode = mode;		//�Q�[�����[�h
}

//==============================================================
//�V�[���̃f�X�g���N�^
//==============================================================
CScene::~CScene()
{

}

//==============================================================
//�V�[���̐�������
//==============================================================
CScene *CScene::Create(MODE mode)
{
	CScene *pScene = NULL;

	if (pScene == NULL)
	{//���������g�p����ĂȂ�������

		//�w�肳�ꂽ���[�h�𐶐�
		pScene = new CScene(mode);
	}

	return pScene;
}

//==============================================================
//�V�[���̏���������
//==============================================================
HRESULT CScene::Init(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:		//�^�C�g�����

		//�^�C�g����ʂ̐���
		if (m_pTitle == NULL)
		{
			m_pTitle = new CTitle;

			if (m_pTitle != NULL)
			{
				//�Q�[����ʂ̏�����
				if (FAILED(m_pTitle->Init()))
				{//���������������s�����ꍇ

					return E_FAIL;
				}
			}
		}

		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����

		////�`���[�g���A����ʂ̐���
		//if (m_pTutorial == NULL)
		//{
		//	m_pTutorial = new CTutorial;

		//	if (m_pTutorial != NULL)
		//	{
		//		//�Q�[����ʂ̏�����
		//		if (FAILED(m_pTutorial->Init()))
		//		{//���������������s�����ꍇ

		//			return E_FAIL;
		//		}
		//	}
		//}

		break;

	case MODE_GAME:			//�Q�[�����

		//�Q�[����ʂ̐���
		if (m_pGame == NULL)
		{
			m_pGame = new CGame;

			if (m_pGame != NULL)
			{
				//�Q�[����ʂ̏�����
				if (FAILED(m_pGame->Init()))
				{//���������������s�����ꍇ

					return E_FAIL;
				}
			}
		}

		break;

	case MODE_RESULT:		//���U���g���

		//���U���g��ʂ̐���
		if (m_pResult == NULL)
		{
			m_pResult = new CResult;

			if (m_pResult != NULL)
			{
				//���U���g��ʂ̏�����
				if (FAILED(m_pResult->Init()))
				{//���������������s�����ꍇ

					return E_FAIL;
				}
			}
		}

		break;

	case MODE_RANKING:		//�����L���O���

		//�����L���O��ʂ̐���
		if (m_pRanking == NULL)
		{
			m_pRanking = new CRanking;

			if (m_pRanking != NULL)
			{
				//�����L���O��ʂ̏�����
				if (FAILED(m_pRanking->Init()))
				{//���������������s�����ꍇ

					return E_FAIL;
				}
			}
		}

		break;

	default:

		// ��~����
		assert(false);

		break;
	}


	return S_OK;
}

//==============================================================
//�V�[���̏I������
//==============================================================
void CScene::Uninit(void)
{
	switch (m_mode)
	{
	case MODE_TITLE:		//�^�C�g�����

		//�^�C�g����ʂ̔j��
		if (m_pTitle != NULL)
		{
			//�I������
			m_pTitle->Uninit();
			m_pTitle = NULL;
		}

		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����

		////�`���[�g���A����ʂ̔j��
		//if (m_pTutorial != NULL)
		//{
		//	//�I������
		//	m_pTutorial->Uninit();
		//	m_pTutorial = NULL;
		//}

		break;

	case MODE_GAME:			//�Q�[�����

		//�Q�[����ʂ̔j��
		if (m_pGame != NULL)
		{
			//�I������
			m_pGame->Uninit();
			m_pGame = NULL;
		}

		break;

	case MODE_RESULT:		//���U���g���

		//���U���g��ʂ̔j��
		if (m_pResult != NULL)
		{
			//�I������
			m_pResult->Uninit();
			m_pResult = NULL;
		}

		break;

	case MODE_RANKING:		//�����L���O���

		//�����L���O��ʂ̔j��
		if (m_pRanking != NULL)
		{
			//�I������
			m_pRanking->Uninit();
			m_pRanking = NULL;
		}

		break;

	default:

		// ��~����
		assert(false);

		break;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//�V�[���̍X�V����
//==============================================================
void CScene::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();

	switch (m_mode)
	{
	case MODE_TITLE:		//�^�C�g�����

		pDebugProc->Print("<<<<<<�^�C�g��>>>>>>\n");

		pDebugProc->Print("\n�`�f�o�b�O�p������@�`\n");
		pDebugProc->Print("[ENTER]�F��ʑJ��\n");

		break;

	case MODE_TUTORIAL:		//�`���[�g���A�����

		pDebugProc->Print("<<<<<<�`���[�g���A��>>>>>>\n");

		break;

	case MODE_GAME:			//�Q�[�����

		pDebugProc->Print("<<<<<<�Q�[��>>>>>>\n");

		pDebugProc->Print("\n�`�f�o�b�O�p������@�`\n");
		pDebugProc->Print("[1]�F��ʑJ��\n");

		break;

	case MODE_RESULT:		//���U���g���

		pDebugProc->Print("<<<<<<���U���g>>>>>>\n");

		pDebugProc->Print("\n�`�f�o�b�O�p������@�`\n");
		pDebugProc->Print("[ENTER]�F��ʑJ��\n");

		break;

	case MODE_RANKING:		//�����L���O���

		pDebugProc->Print("<<<<<<�����L���O>>>>>>\n");

		pDebugProc->Print("\n�`�f�o�b�O�p������@�`\n");
		pDebugProc->Print("[ENTER]�F��ʑJ��\n");

		break;

	default:

		assert(false);

		break;
	}
}

//==============================================================
//�V�[���̕`�揈��
//==============================================================
void CScene::Draw(void)
{

}