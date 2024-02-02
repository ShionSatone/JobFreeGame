//==============================================================
//
// �J�͈̔͏���[whistle.h]
// Author:����������
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
#include "texture.h"
#include "point.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define PRIORITY			(3)			//�D�揇��
#define POS					(10.0f)		//pos�����l
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x

#define MOVE				(6.0f)		//�ړ���
#define DISTANCE			(500.0f)	//����

#define SIZE_PLANE			(D3DXVECTOR3(2.0f,0.0f,2.0f))		// ���ʔ͈͂̏����l

#define TIME_PLANE			(30.0f)		// ���ʌĂт�������
#define TIME_CYLINDER		(100.0f)	// �~���Ăт�������
#define SCALE_PLANE			(6.0f)		// ���ʂ̊g�嗦


//**************************************************************
// �ÓI�����o�ϐ��錾
//**************************************************************
CObject3D* CWhistle::m_pObject3D[TYPE_MAX] = {};	// 3D�I�u�W�F�N�g

//==============================================================
// �R���X�g���N�^
//==============================================================
CWhistle::CWhistle()
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;		// �W���̏��
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
	m_size = SIZE_PLANE;		// �傫��
	m_nWhistleCounter = 0;		// �J��炷����
	m_fscale = 0.0f;			// �g�嗦

	m_state = STATE_NONE;		// ���
	m_type = TYPE_PLANE;		// ���
}

//==============================================================
// �R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CWhistle::CWhistle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;		// �W���̏��
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
	m_size = SIZE_PLANE;		// �傫��
	m_nWhistleCounter = 0;		// �J��炷����
	m_fscale = 0.0f;			// �g�嗦

	m_state = STATE_NONE;		// ���
	m_type = TYPE_PLANE;		// ���
}

//==============================================================
// �f�X�g���N�^
//==============================================================
CWhistle::~CWhistle()
{

}

//==============================================================
// �J�͈̔͂̐�������
//==============================================================
CWhistle* CWhistle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CWhistle* pWhistle = NULL;

	if (pWhistle == NULL)
	{//���������g�p����ĂȂ�������

		//�J�͈̔͂̐���
		pWhistle = new CWhistle(pos, rot);
	}

	//����������
	pWhistle->Init();

	return pWhistle;
}

//==============================================================
// �J�͈̔͂̏���������
//==============================================================
HRESULT CWhistle::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//�e�N�X�`���̓ǂݍ���
	m_nIdxTexture[TYPE_PLANE] = pTexture->Regist("data\\TEXTURE\\whistle00.png");
	//m_nIdxTexture[TYPE_CYLINDER] = pTexture->Regist("data\\TEXTURE\\whistle01.png");

	m_fscale = SCALE_PLANE;			// �g�嗦

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_pObject3D[nCnt] = CObject3D::Create();

		if (m_pObject3D[nCnt] != nullptr)
		{ // �I�u�W�F�N�g3D�� NULL ����Ȃ��ꍇ

			// �e�N�X�`���̊��蓖��
			m_pObject3D[nCnt]->BindTexture(m_nIdxTexture[nCnt]);

			// �I�u�W�F�N�g3D�̏���������
			m_pObject3D[nCnt]->Init();

			// ��ނ̐ݒ�
			m_pObject3D[nCnt]->SetType(CObject::TYPE_POINT);

			// �ʒu�̐ݒ�
			m_pObject3D[nCnt]->SetPos(m_pos);

			// �傫���ݒ�
			m_pObject3D[nCnt]->SetSize(m_size);
			
		}
		else
		{
			// ��~����
			assert(false);
		}
	}

	return S_OK;
}

//==============================================================
// �J�͈̔͂̏I������
//==============================================================
void CWhistle::Uninit(void)
{
	//�I�u�W�F�N�g3D�̏I������
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �ʒu�ݒ�
		m_pObject3D[nCnt]->Uninit();
	}
}

//==============================================================
// �J�͈̔͂̍X�V����
//==============================================================
void CWhistle::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	// �ǔ�����
	Follow();

	// ��Ԃ̍X�V
	UpdateState();

	//�����蔻��
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//�f�o�b�O�\��
	pDebugProc->Print("\n�J�͈̔͂̈ʒu (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("\n�J�͈̔͂̑傫�� (%f, %f, %f)\n", m_size.x, m_size.y, m_size.z);

}

//==============================================================
// ��Ԃ̍X�V����
//==============================================================
void CWhistle::UpdateState(void)
{
	float fSize = (m_nWhistleCounter / m_fscale);

	switch (m_state)
	{
	case CWhistle::STATE_NONE:	// �������Ȃ����
		break;
	case CWhistle::STATE_PLANE:	// ���ʏ��

		if (m_nWhistleCounter >= TIME_PLANE)
		{ // ��莞�Ԍo������

			// �͈͂��~���ɂ���
			m_state = STATE_CYLINDER;
		}
		else
		{
			// �͈͂�傫������
			m_size = D3DXVECTOR3(m_size.x + SIZE_PLANE.x * m_fscale, m_size.y, m_size.z + SIZE_PLANE.z * m_fscale);

			// �ʒu�̐ݒ�
			m_pObject3D[TYPE_PLANE]->SetSize(m_size);
		}

		// �J�̃J�E���^�[
		m_nWhistleCounter++;

		break;
	case CWhistle::STATE_CYLINDER:	// �~�����
		break;
	
	default:

		// ��~����
		assert(false);

		break;
	}
}

//==============================================================
// �ǔ�����
//==============================================================
void CWhistle::Follow(void)
{
	CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();

	m_pos = pPoint->GetPos();

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �ʒu�ݒ�
		m_pObject3D[nCnt]->SetPos(m_pos);
	}
}

//==============================================================
// �J�͈̔͂̕`�揈��
//==============================================================
void CWhistle::Draw(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		// �I�u�W�F�N�g3D�̕`�揈��
		m_pObject3D[nCnt]->Draw();
	}
}

//==============================================================
// �ʒu�ݒ�
//==============================================================
void CWhistle::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++)
	{
		m_pObject3D[nCnt]->SetPos(m_pos);
	}
}

//==============================================================
// �ʒu�擾
//==============================================================
D3DXVECTOR3 CWhistle::GetPos(void)
{
	return m_pos;
}