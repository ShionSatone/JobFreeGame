//==============================================================
//
//�t���N�~���̏���[lucmin_spring.h]
//Author:����������
//
//==============================================================
#include "lucmin_spring.h"
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
#include "whistle.h"
#include "point.h"
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
#define FILE_ENEMY			"data\\TXT\\motion_player.txt"		//�t���N�~�����f���̃e�L�X�g

#define HIT_CNT				(60 * 2)	//�U��������܂ł̃J�E���g��
#define DAMAGE_CNT			(9)			//�_���[�W�J�E���g��
#define APP_CNT				(100)		//�_�Ŏ���

//�ÓI�����o�ϐ��錾
int CLucminSpring::m_nNumAll = 0;						//�t���N�~���̑���
char *CLucminSpring::m_apFileName[PARTS_MAX] =
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
CLucminSpring::CLucminSpring()
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
		m_apModel[nCntEnemy] = NULL;		//�t���N�~��(�p�[�c)�ւ̃|�C���^
	}

	m_nNumModel = 0;		//�t���N�~��(�p�[�c)�̑���
	m_pMotion = NULL;

	m_fRotDest = 0.0f;		//�ڕW
	m_fRotDiff = 0.0f;		//����

	m_nCntDamage = 0;				//�_���[�W�J�E���^�[

	m_nIndex = m_nNumAll;

	m_nNumAll++;

}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CLucminSpring::CLucminSpring(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
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
		m_apModel[nCntEnemy] = NULL;		//�t���N�~��(�p�[�c)�ւ̃|�C���^
	}

	m_pMotion = NULL;		//���[�V�����̏��
	m_nNumModel = 0;		//�t���N�~��(�p�[�c)�̑���

	m_fRotDest = 0.0f;	//�ڕW
	m_fRotDiff = 0.0f;	//����

	m_nCntDamage = 0;			//�_���[�W�J�E���^�[

	m_nIndex = m_nNumAll;

	m_nNumAll++;
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CLucminSpring::~CLucminSpring()
{
	m_nNumAll--;
}

//==============================================================
//�t���N�~���̐�������
//==============================================================
CLucminSpring *CLucminSpring::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLucminSpring *pEnemyModel = NULL;

	if (pEnemyModel == NULL)
	{//���������g�p����ĂȂ�������

		//�t���N�~���̐���
		pEnemyModel = new CLucminSpring(pos, rot);
	}

	//����������
	pEnemyModel->Init();

	return pEnemyModel;
}

//==============================================================
//�t���N�~���̏���������
//==============================================================
HRESULT CLucminSpring::Init(void)
{
	m_fRotDest = m_rot.y;

	//�t���N�~���̐����i�S�p�[�c���j
	for (int nCntModel = 0; nCntModel < PARTS_MAX; nCntModel++)
	{
		m_apModel[nCntModel] = m_apModel[nCntModel]->Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_apFileName[nCntModel]);
	}

	//���[�V�����̏������E����
	m_pMotion = m_pMotion->Create(m_pMotion->MOTIONTEXT_PLAYER);
	m_pMotion->SetModel(&m_apModel[0], PARTS_MAX);
	m_pMotion->Init();

	//���f���̃t�@�C���ǂݍ���
	CLucminSpring::LoadFile();

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
//�t���N�~���̏I������
//==============================================================
void CLucminSpring::Uninit(void)
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
//�t���N�~���̍X�V����
//==============================================================
void CLucminSpring::Update(void)
{
	CDebugProc *pDebugProc = CManager::GetInstance()->GetDebugProc();
	CCamera *pCamera = CManager::GetInstance()->GetCamera();		//�J�����̏��擾
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();
	CPoint* pPoint = CManager::GetInstance()->GetScene()->GetGame()->GetPoint();

	//�O��̈ʒu�X�V
	m_posOld = m_pos;

	//�����蔻��
	//CObjectX::CollisionEnemy(&m_pos, &m_posOld, &m_move, m_min, m_max);

	// �ʒu���Z
	m_pos += m_move;

	// �����t�^
	m_move.x += (0.0f - m_move.x) * 0.1f;
	m_move.z += (0.0f - m_move.z) * 0.1f;

	/*if (pPoint->CircleCollision(pPoint->GetPos(), m_pos, 1000.0f, 50.0f) == true &&
		pPoint->GetState() == CPoint::STATE_WHISTLE &&
		m_state != STATE_WHISTLE && m_state != STATE_FOLLOW && m_state != STATE_THROW)
	{
		m_state = STATE_WHISTLE;
	}*/

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
	pDebugProc->Print("\n�t���N�~���̈ʒu (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
	pDebugProc->Print("�t���N�~���̈ړ��� (%f, %f, %f)\n", m_move.x, m_move.y, m_move.z);
	pDebugProc->Print("�t���N�~���̌���   (%f, %f, %f)\n", m_rot.x, m_rot.y, m_rot.z);
}

//==============================================================
//���[�V�����Ǘ�����
//==============================================================
void CLucminSpring::MotionManager(void)
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
//�t���N�~���̕`�揈��
//==============================================================
void CLucminSpring::Draw(void)
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
		//�t���N�~���̕`��
		m_apModel[nCntEnemy]->Draw();
	}
}

//==============================================================
//���f���t�@�C���ǂݍ��ݏ���
//==============================================================
void CLucminSpring::LoadFile(void)
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