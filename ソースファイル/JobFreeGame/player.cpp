//==============================================================
//
//�v���C���[����[playerModel.cpp]
//Author:����������
//
//==============================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "objectX.h"
#include "modelHierarchy.h"
#include "input.h"
#include "motion.h"
#include "camera.h"
#include "debugproc.h"
#include "game.h"
#include "sound.h"
#include "fade.h"
#include "model.h"
#include "lucmin.h"
#include "whistle.h"
#include "point.h"
//#include "particle.h"

//�}�N����`
#define PRIORITY			(3)			//�D�揇��
#define POS					(10.0f)		//pos�����l
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x
#define MAX_STR				(128)		//�����̍ő吔
#define MOVE				(0.6f)		//��O�̎��̈ړ���
#define STOP_MOVE			(0.8f)		//�~�܂锻��̈ړ���
#define FILE_HUMAN			"data\\TXT\\motion_player.txt"		//�v���C���[���f���̃e�L�X�g

#define HIT_CNT				(60 * 2)	//�U��������܂ł̃J�E���g��
#define DAMAGE_CNT			(9)			//�_���[�W�J�E���g��
#define APP_CNT				(100)		//�_�Ŏ���
#define STEP_CNT			(10)		//�������̃J�E���^�[

//�ÓI�����o�ϐ��錾
char *CPlayer::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\player\\00_body.x",
	"data\\MODEL\\player\\01_head.x",
	"data\\MODEL\\player\\02_hair.x",
	"data\\MODEL\\player\\03_LU_arm.x",
	"data\\MODEL\\player\\04_LD_arm.x",
	"data\\MODEL\\player\\05_L_hand.x",
	"data\\MODEL\\player\\06_RU_arm.x",
	"data\\MODEL\\player\\07_RD_arm.x",
	"data\\MODEL\\player\\08_R_arm.x",
	"data\\MODEL\\player\\09_waist.x",
	"data\\MODEL\\player\\10_LU_leg.x",
	"data\\MODEL\\player\\11_LD_leg.x",
	"data\\MODEL\\player\\12_L_shoe.x",
	"data\\MODEL\\player\\13_RU_leg.x",
	"data\\MODEL\\player\\14_RD_leg.x",
	"data\\MODEL\\player\\15_R_shoe.x",

};

//==============================================================
//�R���X�g���N�^
//==============================================================
CPlayer::CPlayer()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʕۑ��p

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//����
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ő�l
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ŏ��l

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = nullptr;		//�v���C���[(�p�[�c)�ւ̃|�C���^
	}

	m_nNumModel = 0;		//�v���C���[(�p�[�c)�̑���
	m_pMotion = nullptr;

	m_fRotDest = 0.0f;		//�ڕW
	m_fRotDiff = 0.0f;		//����

	m_nDashCounter = 0;		//�_�b�V��������

	m_particleType = PARTICLETYPE_NONE;	//�p�[�e�B�N���̎��
	m_nType = 1;		//���Ԗڂ̃p�[�e�B�N����
	m_nParticleLife = 50;			//�p�[�e�B�N���̎���
	m_state = STATE_NONE;			//���
	m_nCntHit = HIT_CNT;			//�U��������܂ł̃J�E���^�[
	m_bPad = false;					//�p�b�h�̃X�e�B�b�N��|���Ă邩
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CPlayer::CPlayer(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;						//�ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�O��̈ʒu
	m_moveSave = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ړ��ʕۑ��p

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//�ړ���
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ő�l
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			//���f���̍ŏ��l
	m_rot = rot;		//����

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		m_apModel[nCntPlayer] = nullptr;		//�v���C���[(�p�[�c)�ւ̃|�C���^
	}

	m_pMotion = nullptr;		//���[�V�����̏��
	m_nNumModel = 0;		//�v���C���[(�p�[�c)�̑���

	//m_bJump = false;		//�W�����v������
	//m_bMove = false;		//�����Ă邩
	//m_bLand = true;			//���n����
	//m_bDash = false;		//�_�b�V��������

	m_fRotDest = 0.0f;	//�ڕW
	m_fRotDiff = 0.0f;	//����

	m_nDashCounter = 0;		//�_�b�V��������

	m_particleType = PARTICLETYPE_NONE;	//�p�[�e�B�N���̎��
	m_nType = 1;				//���Ԗڂ̃p�[�e�B�N����
	m_nParticleLife = 50;		//�p�[�e�B�N���̎���
	m_state = STATE_NONE;		//���
	m_nCntHit = HIT_CNT;		//�U��������܂ł̃J�E���^�[
	m_bPad = false;				//�p�b�h�̃X�e�B�b�N��|���Ă邩

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CPlayer::~CPlayer()
{

}

//==============================================================
//�v���C���[�̐�������
//==============================================================
CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{//���������g�p����ĂȂ�������

		//�v���C���[�̐���
		pPlayer = new CPlayer(pos, rot);
	}

	//����������
	pPlayer->Init();

	return pPlayer;
}

//==============================================================
//�v���C���[�̏���������
//==============================================================
HRESULT CPlayer::Init(void)
{
	//CLife *pLife = CGame::GetLife();

	m_fRotDest = m_rot.y;

	//�v���C���[�̐����i�S�p�[�c���j
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//���[�V�����̏������E����
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//���f���̃t�@�C���ǂݍ���
	CPlayer::LoadFile();

	//�ő�l�E�ŏ��l���
	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//�ő�lY
		if ((nCntPlayer <= PARTS_HEAD) || (nCntPlayer >= PARTS_WAIST && nCntPlayer <= PARTS_L_SHOE))
		{
			m_max.y += m_apModel[nCntPlayer]->GetSizeMax().y;		//�ő�l���Z
		}

		//�ő�l����ւ�
		if (m_max.x < m_apModel[nCntPlayer]->GetSizeMax().x)
		{
			m_max.x = m_apModel[nCntPlayer]->GetSizeMax().x;		//�ŏ��lX
		}
		if (m_max.z < m_apModel[nCntPlayer]->GetSizeMax().z)
		{
			m_max.z = m_apModel[nCntPlayer]->GetSizeMax().z;		//�ŏ��lZ

		}

		//�ŏ��l����ւ�
		if (m_min.x > m_apModel[nCntPlayer]->GetSizeMin().x)
		{
			m_min.x = m_apModel[nCntPlayer]->GetSizeMin().x;		//�ŏ��lX
		}
		if (m_min.y > m_apModel[nCntPlayer]->GetSizeMin().y)
		{
			m_min.y = m_apModel[nCntPlayer]->GetSizeMin().y;		//�ŏ��lY
		}
		if (m_min.z > m_apModel[nCntPlayer]->GetSizeMin().z)
		{
			m_min.z = m_apModel[nCntPlayer]->GetSizeMin().z;		//�ŏ��lZ

		}
	}

	m_max.y += 40.0f;

	//for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	//{
	//	//�v���C���[�̐F�ݒ�
	//	m_apModel[2]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

	//	//	//��Ԑݒ�
	//	m_apModel[2]->SetState(CObjectX::STATE_DAMAGE);		//�_���[�W��Ԃɂ���
	//}

	SetType(CObject::TYPE_PLAYER);

	return S_OK;
}

//==============================================================
//�v���C���[�̏I������
//==============================================================
void CPlayer::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARTS_MAX; nCnt++)
	{
		if (m_apModel[nCnt] != NULL)
		{//�g�p����Ă�Ƃ�

			//�I������
			m_apModel[nCnt]->Uninit();

			delete m_apModel[nCnt];
			m_apModel[nCnt] = NULL;
		}
	}

	if (m_pMotion != NULL)
	{//�g�p����Ă�Ƃ�

		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	Release();
}

//==============================================================
//�v���C���[�̍X�V����
//==============================================================
void CPlayer::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CFade* pFade = CManager::GetInstance()->GetFade();
	//CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	//CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	if (pFade->GetFade() != CFade::FADE_OUT)
	{//�����_�b�V�����ĂȂ��Ƃ�

		//�v���C���[�̑���
		CPlayer::ControlKeyboard();
		//CPlayer::ControltJoyPad();
	}

	//�����炷����
	CPlayer::SEStep();

	//�����������̍s������
	CPlayer::CollisionAction();

	//��ԍX�V
	CPlayer::UpdateState();

	//�����̕␳
	CPlayer::RotNormalize();

	//��ʊO����
	CPlayer::Screen();

	//���[�V�����Ǘ�
	CPlayer::MotionManager();

	if (m_pos.x >= 350.0f &&
		m_pos.x <= 450.0f &&
		m_pos.z >= 1650.0f &&
		m_pos.z <= 1750.0f)
	{
		// ���U���g�ɑJ��
		pFade->SetFade(CScene::MODE_RESULT);
	}

	//���[�V�����̍X�V����
	m_pMotion->Update();

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//�K�w�\���̍X�V����
		m_apModel[nCntPlayer]->Update();
	}

	//�f�o�b�O�\��
	pDebugProc->Print("\n�v���C���[�̈ʒu    (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("�v���C���[�̈ړ���    (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("�v���C���[�̌���      (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);

}

//==============================================================
//��ԍX�V����
//==============================================================
void CPlayer::UpdateState(void)
{
	//CFade *pFade = CManager::GetInstance()->GetFade();

	switch (m_state)
	{
	case STATE_NONE:		// �ʏ���

		break;

	case STATE_THROW:		// ��������

		break;

	case STATE_WHISTLE:		// �Ăт������

		break;

	case STATE_DAMAGE:		// �_���[�W���

		break;

	case STATE_APPEAR:		// �_�ŏ��

		break;

	case STATE_DEATH:		// ���S���

		break;

	default:

		// ��~����
		assert(false);

		break;
	}
}

//==============================================================
//�����������̍s������
//==============================================================
void CPlayer::CollisionAction(void)
{
	//CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	//CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//�p�b�h�̏��擾

	//bool bLand = CObjectX::Collision(&m_pos, &m_posOld, &m_move, m_min, m_max);

	//�����蔻��
	//if (bLand == true)
	//{//���n������

	//	m_nDashCounter = 0;		//�_�b�V�������Z�b�g

	//	if (m_bLand == false)
	//	{
	//		//�p�[�e�B�N���̐���
	//		//CParticle::Create(D3DXVECTOR3(m_pos.x, m_pos.y, m_pos.z), D3DXCOLOR(0.1f, 0.2f, 0.4f, 0.8f), PARTICLETYPE_MOVE, 20, 10.0f);
	//	}

	//	m_bJump = false;	//�W�����v���ĂȂ���Ԃɂ���
	//	m_bLand = true;		//���n������Ԃɂ���

	//}
	//else if (bLand == false &&
	//	pInputKeyboard->GetPress(DIK_SPACE) == false && pInputJoyPad->GetPress(pInputJoyPad->BUTTON_A, 0) == false)
	//{//�n�ʂɂ��ĂȂ� && �W�����v�{�^�������ĂȂ�

	//	m_bJump = true;		//�W�����v���Ă��Ԃɂ���
	//	m_bLand = false;	//���n���ĂȂ���Ԃɂ���
	//}
}

//==============================================================
//���[�V�����Ǘ�����
//==============================================================
void CPlayer::MotionManager(void)
{
	//if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	//{//�����Ă� && �ҋ@���

	//	//��������
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	//}
	////else if (m_pMotion->IsFinish() == true && m_bAction == true)
	////{//�U�����I�������

	////	m_bAction = false;		//�U�����ĂȂ���Ԃɂ���
	////}
	//else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
	//	(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_bMove == false && m_bJump == false && m_bLand == true))
	//{//���[�V�������I��������

	//	//�ҋ@��Ԃɖ߂�
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	//}
}

//==============================================================
//�����̕␳����
//==============================================================
void CPlayer::RotNormalize(void)
{
	//�����̍��������߂�
	m_fRotDiff = m_fRotDest - m_rot.y;

	//�ڕW�̕����܂ł̍������C��
	if (m_fRotDiff < -D3DX_PI)
	{
		m_fRotDiff += D3DX_PI * 2;
	}
	else if (m_fRotDiff > D3DX_PI)
	{
		m_fRotDiff -= D3DX_PI * 2;
	}

	//��������
	m_rot.y += m_fRotDiff * 0.1f;

	//���݂̕����C��
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
}

//==============================================================
//�����炷����
//==============================================================
void CPlayer::SEStep(void)
{
	//CSound *pSound = CManager::GetInstance()->GetSound();

	//�����炷
	//if (m_bMove == true && m_bJump == false)
	//{//�����Ă�Ƃ� && �W�����v���ĂȂ�

	//	if ((m_nCntMove >= STEP_CNT))
	//	{//��莞�Ԃ�������

	//		//BGM�Đ�
	//		//pSound->Play(pSound->SOUND_LABEL_SE_STEP);

	//		m_nCntMove = 0;
	//	}

	//	m_nCntMove++;		//�����炷�J�E���^�[���Z
	//}
	//else if(m_bMove == false)
	//{//�����ĂȂ��Ƃ�

	//	m_nCntMove = STEP_CNT;
	//}

}

//==============================================================
//�v���C���[�̃L�[�{�[�h���쏈��
//==============================================================
void CPlayer::ControlKeyboard(void)
{
	//�ړ�����
	ControlKeyboardMove();

	// ���N�~�����ߏ���
	ControlKeyboardCommand();

	// �ړ��ʉ��Z
	m_pos += m_move;

	// �����t�^
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;
}

//==============================================================
//�v���C���[�̃L�[�{�[�h�̈ړ����쏈��
//==============================================================
void CPlayer::ControlKeyboardMove(void)
{
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//�ړ�
	if (pInputKeyboard->GetPress(DIK_D) == true)
	{//�E
		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//��
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.25f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//��O
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * -0.75f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -0.25f;

		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
		}

		//m_bMove = true;		//��������
	}
	else if (pInputKeyboard->GetPress(DIK_A) == true)
	{//��

		if (pInputKeyboard->GetPress(DIK_W) == true)
		{//��
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.25f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.75f;

		}
		else if (pInputKeyboard->GetPress(DIK_S) == true)
		{//��O
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * 0.75f) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * 0.25f;

		}
		else
		{
			m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
			m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
			m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
		}

		//m_bMove = true;		//��������
	}
	else if (pInputKeyboard->GetPress(DIK_W) == true)
	{//��
		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_UP) * MOVE;
		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_DOWN;

		//m_bMove = true;		//��������
	}
	else if (pInputKeyboard->GetPress(DIK_S) == true)
	{//��O
		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_DOWN) * MOVE;
		m_fRotDest = pCamera->GetRotation().y;

		//m_bMove = true;		//��������
	}

	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
	{//�����ĂȂ��Ƃ�

		//m_bMove = false;		//�����ĂȂ���Ԃɂ���
	}
}

//==============================================================
// ���ߏ���
//==============================================================
void CPlayer::ControlKeyboardCommand(void)
{
	CInputKeyboard* pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾

	if (pInputKeyboard->GetTrigger(DIK_L) == true)
	{ // L�L�[���������Ƃ�

		// �������Ԃɂ���
		m_state = STATE_THROW;

		// ��Ԑݒ�
		SetStateThrow();
	}
	//else if (pInputKeyboard->GetPress(DIK_RETURN) == true && m_state != STATE_WHISTLE)
	//{ // ENTER�L�[�������� && �W����Ԃ���Ȃ��Ƃ�

	//	// �W����Ԃɂ���
	//	m_state = STATE_WHISTLE;

	//	// ��Ԑݒ�
	//	//SetStateWhistle();
	//}
}

//==============================================================
// �������Ԑݒ�
//==============================================================
void CPlayer::SetStateThrow(void)
{
	CLucmin* pLucmin = nullptr;		// ���N�~���̏��

	for (int nCnt = 0; nCnt < MAX_LUCMIN; nCnt++)
	{
		// ���N�~���̏��擾
		pLucmin = CManager::GetInstance()->GetScene()->GetGame()->GetLucmin(nCnt);

		if (pLucmin != nullptr)
		{ // ���N�~���� NULL ����Ȃ��Ƃ�

			if (pLucmin->GetState() == CLucmin::STATE_FOLLOW)
			{ // ���N�~������������� && �Ăт��� && �T����Ԃ���Ȃ��Ƃ�

				pLucmin->SetState(CLucmin::STATE_THROW);		// ��������Ԃɂ���

				break;

			}
		}
	}
}

//==============================================================
// �W����Ԑݒ�
//==============================================================
void CPlayer::SetStateWhistle(void)
{
	//CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();		// �|�C���g�̏��擾

	//pPoint->Set
}

//==============================================================
//�v���C���[�̃p�b�h���쏈��
//==============================================================
//void CPlayer::ControlFrontJoyPad(void)
//{
//	CSound *pSound = CManager::GetInstance()->GetSound();
//
//	//�ړ�����
//	CPlayer::ControlFrontJoyPadMove();
//}
//
////==============================================================
////�v���C���[�̃p�b�h�̈ړ����쏈��
////==============================================================
//void CPlayer::ControlFrontJoyPadMove(void)
//{
//	CInputJoyPad *pInputJoyPad = CManager::GetInstance()->GetInputJoyPad();				//�p�b�h�̏��擾
//	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
//
//	//�ړ�
//	if (pInputJoyPad->GetPressLX(0).x > 0.0f)
//	{//�E
//		m_move.x += sinf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
//		m_move.z += cosf(pCamera->GetRotation().y + D3DX_PI * CURVE_RL) * MOVE;
//		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * -CURVE_RL;
//
//		m_bMove = true;		//��������
//	}
//	else if (pInputJoyPad->GetPressLX(0).x < 0.0f)
//	{//��
//		m_move.x += sinf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
//		m_move.z += cosf(pCamera->GetRotation().y + -D3DX_PI * CURVE_RL) * MOVE;
//		m_fRotDest = pCamera->GetRotation().y + D3DX_PI * CURVE_RL;
//
//		m_bMove = true;		//��������
//	}
//
//	if ((m_move.x <= STOP_MOVE && m_move.x >= -STOP_MOVE) && (m_move.z <= STOP_MOVE && m_move.z >= -STOP_MOVE))
//	{//�����ĂȂ��Ƃ�
//
//		m_bMove = false;		//�����ĂȂ���Ԃɂ���
//	}
//}

//==============================================================
//��ʊO���菈��
//==============================================================
void CPlayer::Screen(void)
{
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;
	}

	//if (m_pos.y <= -6000.0f)
	//{//��ԉ��܂ōs������

	//	CFade *pFade = CManager::GetInstance()->GetFade();			//�t�F�[�h�̏��擾
	//	/*CDeathUI *pDeathUI = CGame::GetDeathUI();
	//	CItemUI *pItemUI = CGame::GetItemUI();*/

	//	//�X�R�A�ۑ�
	//	/*CManager::GetInstance()->SetNumDeath(pDeathUI->Get());
	//	CManager::GetInstance()->SetNumItem(pItemUI->Get());*/

	//	//���U���g�̔���
	//	CManager::GetInstance()->SetResult(true);

	//	//�Q�[�����
	//	pFade->SetFade(CScene::MODE_RESULT);
	//}
}

//==============================================================
//�v���C���[�̕`�揈��
//==============================================================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

	//���[���h�}�g���b�N�X��������
	D3DXMatrixIdentity(&m_mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	{
		//�v���C���[�̕`��
		m_apModel[nCntPlayer]->Draw();
	}
}

//==============================================================
//�v���C���[�̃q�b�g����
//==============================================================
void CPlayer::Hit(void)
{
	//CSound *pSound = CManager::GetInstance()->GetSound();
	//CDeathUI *pDeathUI = CGame::GetDeathUI();
	CFade *pFade = CManager::GetInstance()->GetFade();

	pFade->SetFade(CScene::MODE_RESULT);

	//if (m_state != CObject::STATE_DAMAGE && m_state != CObject::STATE_APPEAR && m_nCntDamage <= 0 && m_nCntHit <= 0 &&
	//	pFade->GetFade() != CFade::FADE_IN)
	//{
	//	//BGM�Đ�
	//	//pSound->Play(pSound->SOUND_LABEL_SE_DAMAGE001);

	//	m_state = CObject::STATE_APPEAR;

	//	m_nCntDamage = HIT_CNT;				//�_���[�W��Ԃ�ۂ��Ԑݒ�
	//	m_nCntHit = 60;				//�U��������܂ł̃J�E���^�[

	//	pDeathUI->Add(1);		//���S�����Z

	//	//�t�F�[�h������
	//	pFade->SetNormalFade();

	//	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	//	{
	//		//�v���C���[�̐F�ݒ�
	//		m_apModel[nCntPlayer]->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	//	}

	//	//�p�[�e�B�N������
	//	//CParticle::Create(m_pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), PARTICLETYPE_EXPLOSION, 30, 40);

	//	//��Ԑݒ�
	//	for (int nCntPlayer = 0; nCntPlayer < PARTS_MAX; nCntPlayer++)
	//	{
	//		m_apModel[nCntPlayer]->SetState(m_state);		//�_���[�W��Ԃɂ���
	//	}
	//}
}

//==============================================================
//���f���t�@�C���ǂݍ��ݏ���
//==============================================================
void CPlayer::LoadFile(void)
{
	FILE *pFile;				//�t�@�C���|�C���^
	char aString[MAX_STR];		//�����ǂݍ���
	int nIndex = 0, nParent = 0;	//�p�[�cNo.,�e�ԍ�
	D3DXVECTOR3 pos, rot;

	//�t�@�C���J��
	pFile = fopen(FILE_HUMAN, "r");

	if (pFile != NULL)
	{//�t�@�C�����J�����ꍇ

		while (strcmp(&aString[0], "CHARACTERSET") != 0)
		{//[CHARACTERSET]����܂ł̊�

			fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
		}

		if (strcmp(&aString[0], "CHARACTERSET") == 0)
		{//[CHARACTERSET]��������

			while (strcmp(&aString[0], "END_CHARACTERSET") != 0)
			{//[END_CHARACTERSET]������܂ł̊�

				fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

				if (strcmp(&aString[0], "PARTSSET") == 0)
				{//[PARTSSET]��������

					while (strcmp(&aString[0], "END_PARTSSET") != 0)
					{//[END_PARTSSET]������܂ł̊�

						fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���

						if (strcmp(&aString[0], "INDEX") == 0)
						{//�p�[�cNo.

							fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
							fscanf(pFile, "%d", &nIndex);			//�p�[�cNo.�ǂݍ���

						}
						else if (strcmp(&aString[0], "PARENT") == 0)
						{//�e���

							fscanf(pFile, "%s", &aString[0]);		//�����ǂݍ���
							fscanf(pFile, "%d", &nParent);			//�e�ԍ��ǂݍ���

							if (nParent == -1)
							{//�e�����Ȃ�������

								m_apModel[nIndex]->SetParent(NULL);		//NULL������
							}
							else
							{//�e��������

								m_apModel[nIndex]->SetParent(m_apModel[nParent]);		//�e�ԍ������
							}
						}
						else if (strcmp(&aString[0], "POS") == 0)
						{//�ʒu���

							fscanf(pFile, "%s", &aString[0]);	//�����ǂݍ���

							fscanf(pFile, "%f", &pos.x);		//�ʒu�ǂݍ���
							fscanf(pFile, "%f", &pos.y);		//�ʒu�ǂݍ���
							fscanf(pFile, "%f", &pos.z);		//�ʒu�ǂݍ���

							m_apModel[nIndex]->SetPos(pos);		//�ʒu�ݒ�
							m_apModel[nIndex]->SetDefaultPos(pos);	//�����ʒu�ݒ�

						}
						else if (strcmp(&aString[0], "ROT") == 0)
						{//�������

							fscanf(pFile, "%s", &aString[0]);	//�����ǂݍ���

							fscanf(pFile, "%f", &rot.x);		//�����ǂݍ���
							fscanf(pFile, "%f", &rot.y);		//�����ǂݍ���
							fscanf(pFile, "%f", &rot.z);		//�����ǂݍ���

							m_apModel[nIndex]->SetRot(rot);		//�����ݒ�
							m_apModel[nIndex]->SetDefaultRot(rot);	//���������ݒ�
						}
					}
				}
			}
		}

		//�t�@�C������
		fclose(pFile);
	}
}