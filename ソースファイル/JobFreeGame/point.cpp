//==============================================================
//
//�|�C���g�̏���[point.h]
//Author:����������
//
//==============================================================
#include "point.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "player.h"
#include "texture.h"
#include "whistle.h"

//**************************************************************
//�}�N����`
//**************************************************************
#define PRIORITY			(3)			//�D�揇��
#define POS					(10.0f)		//pos�����l
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x

#define MOVE				(6.0f)		//�ړ���
#define DISTANCE			(500.0f)	//����

#define HIT_CNT				(60 * 2)	//�U��������܂ł̃J�E���g��
#define DAMAGE_CNT			(9)			//�_���[�W�J�E���g��
#define APP_CNT				(100)		//�_�Ŏ���

//**************************************************************
//�ÓI�����o�ϐ��錾
//**************************************************************

//==============================================================
//�R���X�g���N�^
//==============================================================
CPoint::CPoint()
{
	m_nIdxTexture = -1;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu

	m_state = STATE_NONE;			// ���
	m_color = COLOR_NONE;			// �F
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CPoint::CPoint(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_nIdxTexture = -1;
	m_pos = pos;									// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu

	m_state = STATE_NONE;			// ���
	m_color = COLOR_NONE;			// �F
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CPoint::~CPoint()
{

}

//==============================================================
//�|�C���g�̐�������
//==============================================================
CPoint* CPoint::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPoint* pPoint = NULL;

	if (pPoint == NULL)
	{//���������g�p����ĂȂ�������

		//�|�C���g�̐���
		pPoint = new CPoint(pos, rot);
	}

	//����������
	pPoint->Init();

	return pPoint;
}

//==============================================================
//�|�C���g�̏���������
//==============================================================
HRESULT CPoint::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//�n��e�N�X�`���̓ǂݍ���
	m_nIdxTexture = pTexture->Regist("data\\TEXTURE\\point000.png");

	// �e�N�X�`���̊��蓖��
	CObject3D::BindTexture(m_nIdxTexture);

	//�I�u�W�F�N�g3D�̏���������
	CObject3D::Init();

	//��ނ̐ݒ�
	CObject3D::SetType(CObject::TYPE_POINT);

	//�ʒu�̐ݒ�
	CObject3D::SetPos(m_pos);

	//�|�C���g�̑傫���ݒ�
	CObject3D::SetSize(D3DXVECTOR3(100.0f, 0.0f, 100.0f));

	return S_OK;
}

//==============================================================
//�|�C���g�̏I������
//==============================================================
void CPoint::Uninit(void)
{
	//�I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//==============================================================
//�|�C���g�̍X�V����
//==============================================================
void CPoint::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	//�����蔻��
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	// �J�̏���
	ContlolWhistle();

	// ��ԍX�V
	UpdateState();

	// �F�X�V
	UpdateColor();

	// ��ʊO����
	Screen();

	//�f�o�b�O�\��
	pDebugProc->Print("\n�|�C���g�̈ʒu (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//==============================================================
// ��ԍX�V����
//==============================================================
void CPoint::UpdateState(void)
{
	switch (m_state)
	{
	case CPoint::STATE_NONE:		// �ʏ���

		// �ǔ�����
		Follow();

		break;

	case CPoint::STATE_LOCKON:		// ���b�N�I��
		break;
	case CPoint::STATE_THROW:		// ��������

		//���쏈��
		ControlKeyboard();

		break;

	case CPoint::STATE_WHISTLE:		// �Ăя��

		// �ǔ�����
		Follow();

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
void CPoint::Follow(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// �|�C���g�̏��擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();

	m_pos.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * DISTANCE;
	m_pos.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * DISTANCE;

	// �ʒu�ݒ�
	CObject3D::SetPos(m_pos);
}

//==============================================================
// �F�X�V����
//==============================================================
void CPoint::UpdateColor(void)
{
	switch (m_color)
	{
	case CPoint::COLOR_NONE:		// �ʏ�F(�D�F)
		break;
	case CPoint::COLOR_SPRING:		// �t(�s���N)
		break;
	case CPoint::COLOR_SUMMER:		// ��(��)
		break;
	case CPoint::COLOR_FALL:		// �H(�I�����W)
		break;
	case CPoint::COLOR_WINTER:		// �~(��)
		break;
	default:

		// ��~����
		assert(false);

		break;
	}
}

//==============================================================
// �|�C���g�̃L�[�{�[�h���쏈��
//==============================================================
void CPoint::ControlKeyboard(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//�ړ�
	if (pInputKeyboard->GetPress(DIK_L) == true)
	{//�E
		if (pInputKeyboard->GetPress(DIK_I) == true)
		{//��
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;

		}
		else if (pInputKeyboard->GetPress(DIK_K) == true)
		{//��O
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;

		}
		else
		{
			m_pos.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_J) == true)
	{//��

		if (pInputKeyboard->GetPress(DIK_I) == true)
		{//��
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;

		}
		else if (pInputKeyboard->GetPress(DIK_K) == true)
		{//��O
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;

		}
		else
		{
			m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
			m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
		}
	}
	else if (pInputKeyboard->GetPress(DIK_I) == true)
	{//��
		m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
		m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
	}
	else if (pInputKeyboard->GetPress(DIK_K) == true)
	{//��O
		m_pos.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
		m_pos.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
	}

	//�ʒu�̐ݒ�
	CObject3D::SetPos(m_pos);
}

//==============================================================
// �Ăт�������
//==============================================================
void CPoint::ContlolWhistle(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CWhistle* pWhistle = nullptr;

	if (pInputKeyboard->GetPress(DIK_RETURN) == true)
	{ // ENTER���������Ƃ�

		if (m_state != STATE_WHISTLE)
		{ // �W����Ԃ���Ȃ��Ƃ�

			if (pWhistle == nullptr)
			{
				// �W���͈͂̐���
				pWhistle = CWhistle::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));

				// �Ăт����J�n��Ԃɂ���
				pWhistle->SetState(CWhistle::STATE_PLANE);
			}

			// �W����Ԃɂ���
			m_state = STATE_WHISTLE;

		}

	}
}

//==============================================================
// ��ʊO���菈��
//==============================================================
void CPoint::Screen(void)
{
	
}

//==============================================================
// �|�C���g�̕`�揈��
//==============================================================
void CPoint::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}