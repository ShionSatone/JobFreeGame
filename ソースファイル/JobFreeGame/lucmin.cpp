//==============================================================
//
//���N�~���̏���[lucmin.h]
//Author:����������
//
//==============================================================
#include "lucmin.h"
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
#include "player.h"
//#include "effect.h"

//�}�N����`
#define PRIORITY			(3)			//�D�揇��
#define POS					(10.0f)		//pos�����l
#define CURVE_RL			(0.5f)		//���E�̊p�x
#define CURVE_UP			(0.0f)		//��̊p�x
#define CURVE_DOWN			(1.0f)		//���̊p�x

#define GRAVITY				(-1.0f)		//�d��
#define THROW_HEIGHT		(15.0f)		//�������鍂��
#define MAX_STR				(128)		//�����̍ő吔
#define MOVE				(6.0f)		//�ړ���
#define MOVE_DISTANCE		(200.0f)	//�ړ�����
#define FILE_ENEMY			"data\\TXT\\motion_player.txt"		//���N�~�����f���̃e�L�X�g

#define HIT_CNT				(60 * 2)	//�U��������܂ł̃J�E���g��
#define DAMAGE_CNT			(9)			//�_���[�W�J�E���g��
#define APP_CNT				(100)		//�_�Ŏ���

//�ÓI�����o�ϐ��錾
int CLucmin::m_nNumAll = 0;						//���N�~���̑���
char *CLucmin::m_apFileName[PARTS_MAX] =
{
	"data\\MODEL\\enemy\\00_body.x",
	"data\\MODEL\\enemy\\01_head.x",
	"data\\MODEL\\enemy\\02_hair.x",
	"data\\MODEL\\enemy\\03_LU_arm.x",
	"data\\MODEL\\enemy\\04_LD_arm.x",
	"data\\MODEL\\enemy\\05_L_hand.x",
	"data\\MODEL\\enemy\\06_RU_arm.x",
	"data\\MODEL\\enemy\\07_RD_arm.x",
	"data\\MODEL\\enemy\\08_R_arm.x",
	"data\\MODEL\\enemy\\09_waist.x",
	"data\\MODEL\\enemy\\10_LU_leg.x",
	"data\\MODEL\\enemy\\11_LD_leg.x",
	"data\\MODEL\\enemy\\12_L_shoe.x",
	"data\\MODEL\\enemy\\13_RU_leg.x",
	"data\\MODEL\\enemy\\14_RD_leg.x",
	"data\\MODEL\\enemy\\15_R_shoe.x",

};

//==============================================================
//�R���X�g���N�^
//==============================================================
CLucmin::CLucmin()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̌���
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���f���̍ő�l
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���f���̍ŏ��l

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//���N�~��(�p�[�c)�ւ̃|�C���^
	}

	m_nNumModel = 0;		//���N�~��(�p�[�c)�̑���
	m_pMotion = NULL;

	m_fRotDest = 0.0f;		//�ڕW
	m_fRotDiff = 0.0f;		//����

	m_state = STATE_FOLLOW;			//���
	m_throwState = THROWSTATE_NONE;		// ���������
	m_nCntDamage = 0;				//�_���[�W�J�E���^�[

	m_nIndex = m_nNumAll;

	m_nNumAll++;

}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CLucmin::CLucmin(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;									// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
	m_posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̈ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ړ���
	m_max = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���f���̍ő�l
	m_min = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// ���f���̍ŏ��l
	m_rot = rot;		//����
	m_rotDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̌���

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy] = NULL;		//���N�~��(�p�[�c)�ւ̃|�C���^
	}

	m_pMotion = NULL;		//���[�V�����̏��
	m_nNumModel = 0;		//���N�~��(�p�[�c)�̑���

	m_fRotDest = 0.0f;	//�ڕW
	m_fRotDiff = 0.0f;	//����

	m_state = STATE_FOLLOW;		//���
	m_throwState = THROWSTATE_NONE;		// ���������

	m_nCntDamage = 0;			//�_���[�W�J�E���^�[

	m_nIndex = m_nNumAll;

	m_nNumAll++;
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CLucmin::~CLucmin()
{
	m_nNumAll--;
}

//==============================================================
//���N�~���̐�������
//==============================================================
CLucmin *CLucmin::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLucmin *pEnemyModel = NULL;

	if (pEnemyModel == NULL)
	{//���������g�p����ĂȂ�������

		//���N�~���̐���
		pEnemyModel = new CLucmin(pos, rot);
	}

	//����������
	pEnemyModel->Init();

	return pEnemyModel;
}

//==============================================================
//���N�~���̏���������
//==============================================================
HRESULT CLucmin::Init(void)
{
	m_fRotDest = m_rot.y;

	//���N�~���̐����i�S�p�[�c���j
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//���[�V�����̏������E����
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//���f���̃t�@�C���ǂݍ���
	CLucmin::LoadFile();

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

	CObject::SetType(CObject::TYPE_ENEMY);

	return S_OK;
}

//==============================================================
//���N�~���̏I������
//==============================================================
void CLucmin::Uninit(void)
{
	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		if (m_apModel[nCntEnemy] != NULL)
		{//�g�p����Ă�Ƃ�

			//�I������
			m_apModel[nCntEnemy]->Uninit();
			m_apModel[nCntEnemy] = NULL;
		}
	}

	if (m_pMotion != NULL)
	{//�g�p����Ă�Ƃ�

		//���[�V�����̔j��
		delete m_pMotion;
		m_pMotion = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
//���N�~���̍X�V����
//==============================================================
void CLucmin::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CInputKeyboard *pInputKeyboard = CManager::GetInstance()->GetInputKeyboard();		//�L�[�{�[�h�̏��擾
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	//�����蔻��
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	// �ʒu���Z
	m_pos += m_move;

	// ��ʊO����
	Screen();

	// �����t�^
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	//��ԍX�V
	UpdateState();

	//�����̕␳
	RotNormalize();

	//���[�V�����Ǘ�
	MotionManager();

	//���[�V�����̍X�V����
	m_pMotion->Update();

	//��Ԑݒ�
	/*for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		m_apModel[nCntEnemy]->SetState(m_state);

	}*/

	//�f�o�b�O�\��
	pDebugProc->Print("\n���N�~���̈ʒu (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("���N�~���̈ړ��� (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("���N�~���̌���   (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);
}

//==============================================================
//��ԍX�V����
//==============================================================
void CLucmin::UpdateState(void)
{
	switch (m_state)
	{
	case STATE_NONE:		// �������ĂȂ����
		break;

	case STATE_FOLLOW:		// �ǔ����

		// �ǔ�����
		FollowMove();

		break;

	case STATE_THROW:		// �������Ă���

		if (m_throwState == THROWSTATE_NONE)
		{ // �������ĂȂ���Ԃ̏ꍇ

			m_throwState = THROWSTATE_STANDBY;		// ������ꏀ���̏�Ԃɂ���
		}

		// ��������Ԃ̍X�V
		UpdateThrowState();

		break;

	case STATE_SEARCH:		// �T�����
		break;

	case STATE_ATTACK:		// �U�����
		break;

	case STATE_WHISTLE:		// �Ăі߂������

		// �ǔ�����
		FollowMove();

		break;

	case STATE_DAMAGE:		// �_���[�W���
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
// ��������Ԃ̍X�V����
//==============================================================
void CLucmin::UpdateThrowState(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// �v���C���[�̏��擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();
	D3DXVECTOR3 posDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ړI�̈ʒu
	float fMoveSpeedX = 0.12f;
	float fMoveSpeedZ = 0.12f;

	switch (m_throwState)
	{
	case CLucmin::THROWSTATE_NONE:		// �������ĂȂ����

		break;

	case CLucmin::THROWSTATE_STANDBY:	// �������

		// �v���C���[�̈ʒu���瓊������悤�ɂ���
		m_pos.x = posPlayer.x;
		m_pos.z = posPlayer.z;

		// ���N�~���������鍂�����Z
		m_move.y += THROW_HEIGHT;

		// �ړI�̈ʒu�ݒ�
		m_posDest.x = posPlayer.x - (posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * 100.0f);
		m_posDest.z = posPlayer.z - (posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * 100.0f);

		m_rotDest.y = atan2f(m_posDest.x, m_posDest.z);

		// ���������Ԃɂ���
		m_throwState = THROWSTATE_THROW;

		break;

	case CLucmin::THROWSTATE_THROW:		// ����������

		if (m_posDest.x < 0.0f)
		{
			fMoveSpeedX *= -1.0f;
		}

		if (m_posDest.z < 0.0f)
		{
			fMoveSpeedZ *= -1.0f;
		}


		// �ړ��ʉ��Z
		m_move.x = sinf(m_rotDest.y + D3DX_PI) * (MOVE + (m_posDest.x * fMoveSpeedX));
		m_move.z = cosf(m_rotDest.y + D3DX_PI) * (MOVE + (m_posDest.z * fMoveSpeedZ));

		// �d��
		m_move.y += GRAVITY;

		break;

	default:

		// ��~����
		assert(false);

		break;
	}


}

//==============================================================
//���[�V�����Ǘ�����
//==============================================================
void CLucmin::MotionManager(void)
{
	CPlayer* pPlayer = CGame::GetPlayer();
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾

	//switch (m_MotionState)
	//{
	//case MOTIONSTATE_NONE:		//�������ĂȂ�(�~�܂��Ă���)

	//	break;

	//case MOTIONSTATE_MOVE:		//�����Ă���

	//	break;

	//case MOTIONSTATE_JUMP:		//�W�����v���Ă���

	//	break;

	//case MOTIONSTATE_LAND:		//���n�������

	//	break;

	//default:

	//	// ��~����
	//	assert(false);

	//	break;
	//}

	//if (m_bMove == true && m_pMotion->GetType() == m_pMotion->MOTIONTYPE_NEUTRAL)
	//{//�����Ă� && �ҋ@���

	//	//��������
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_MOVE);
	//}
	//else if ((m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && m_pMotion->IsFinish() == true) ||
	//	(m_pMotion->GetType() != m_pMotion->MOTIONTYPE_NEUTRAL && 
	//		m_bMove == false && m_bJump == false && m_bLand == true))
	//{//���[�V�������I��������

	//	//�ҋ@��Ԃɖ߂�
	//	m_pMotion->Set(m_pMotion->MOTIONTYPE_NEUTRAL);
	//}
}

//==============================================================
//�����̕␳����
//==============================================================
void CLucmin::RotNormalize(void)
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
//��ʊO���菈��
//==============================================================
void CLucmin::Screen(void)
{
	if (m_pos.y <= 0.0f)
	{
		m_pos.y = 0.0f;
		m_move.y = 0.0f;

		if (m_throwState == THROWSTATE_THROW)
		{ // ��������Ԃ̎�

			m_move.x = 0.0f;
			m_move.z = 0.0f;

			// �ʏ��Ԃɖ߂�
			m_throwState = THROWSTATE_NONE;

			// �T����Ԃɂ���
			m_state = STATE_SEARCH;
		}
	}
}

//==============================================================
//���N�~���̕`�揈��
//==============================================================
void CLucmin::Draw(void)
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

	for (int nCntEnemy = 0; nCntEnemy < PARTS_MAX; nCntEnemy++)
	{
		//���N�~���̕`��
		m_apModel[nCntEnemy]->Draw();
	}
}

//==============================================================
// �v���C���[�ɂ��Ă�������
//==============================================================
void CLucmin::FollowMove(void)
{
	CCamera* pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// �v���C���[�̏��擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();
	float fRotDest = 0.0f;

	// �ړI�̈ʒu
	m_posDest.x = m_pos.x - posPlayer.x;
	m_posDest.z = m_pos.z - posPlayer.z;

	// �ړI�̌���
	m_fRotDest = atan2f(m_posDest.x, m_posDest.z);

	if (((m_pos.x - posPlayer.x) > MOVE_DISTANCE || (m_pos.x - posPlayer.x) < -MOVE_DISTANCE) ||
		((m_pos.z - posPlayer.z) > MOVE_DISTANCE || (m_pos.z - posPlayer.z) < -MOVE_DISTANCE))
	{ // ��苗�����痣�ꂽ��

		if (m_state == STATE_WHISTLE)
		{ // �Ăі߂���Ԃ̎�

			// �ړ��ʉ��Z
			m_move.x = sinf(m_fRotDest + D3DX_PI) * MOVE * 1.5f;
			m_move.z = cosf(m_fRotDest + D3DX_PI) * MOVE * 1.5f;
		}
		else
		{
			// �ړ��ʉ��Z
			m_move.x = sinf(m_fRotDest + D3DX_PI) * MOVE;
			m_move.z = cosf(m_fRotDest + D3DX_PI) * MOVE;
		}
		
	}
	else
	{ // ��苗���ȓ���������

		if (m_state == STATE_WHISTLE)
		{ // �Ăі߂���Ԃ̎�

			m_state = STATE_FOLLOW;		// �ǔ���Ԃɂ���
		}

		
	}
}

//==============================================================
//���f���t�@�C���ǂݍ��ݏ���
//==============================================================
void CLucmin::LoadFile(void)
{
	FILE *pFile;				//�t�@�C���|�C���^
	char aString[MAX_STR];		//�����ǂݍ���
	int nIndex = 0, nParent = 0;	//�p�[�cNo.,�e�ԍ�
	D3DXVECTOR3 pos, rot;

	//�t�@�C���J��
	pFile = fopen(FILE_ENEMY, "r");

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