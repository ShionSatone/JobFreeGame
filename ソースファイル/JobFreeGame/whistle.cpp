//==============================================================
//
//�J�͈̔͏���[whistle.h]
//Author:����������
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
#include "player.h"
#include "texture.h"

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
CWhistle::CWhistle()
{
	for (int nCnt = 0; nCnt < MAX_WHISTLE_TEX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;

	}
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CWhistle::CWhistle(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int nCnt = 0; nCnt < MAX_WHISTLE_TEX; nCnt++)
	{
		m_nIdxTexture[nCnt] = -1;

	}
	m_pos = pos;									// �ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �O��̈ʒu
}

//==============================================================
//�f�X�g���N�^
//==============================================================
CWhistle::~CWhistle()
{

}

//==============================================================
//�|�C���g�̐�������
//==============================================================
CWhistle* CWhistle::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CWhistle* pWhistle = NULL;

	if (pWhistle == NULL)
	{//���������g�p����ĂȂ�������

		//�|�C���g�̐���
		pWhistle = new CWhistle(pos, rot);
	}

	//����������
	pWhistle->Init();

	return pWhistle;
}

//==============================================================
//�|�C���g�̏���������
//==============================================================
HRESULT CWhistle::Init(void)
{
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//�n��e�N�X�`���̓ǂݍ���
	m_nIdxTexture[0] = pTexture->Regist("data\\TEXTURE\\point000.png");
	m_nIdxTexture[1] = pTexture->Regist("data\\TEXTURE\\point000.png");

	for (int nCnt = 0; nCnt < MAX_WHISTLE_TEX; nCnt++)
	{
		// �e�N�X�`���̊��蓖��
		CObject3D::BindTexture(m_nIdxTexture[nCnt]);
	}
	

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
void CWhistle::Uninit(void)
{
	//�I�u�W�F�N�g3D�̏I������
	CObject3D::Uninit();
}

//==============================================================
//�|�C���g�̍X�V����
//==============================================================
void CWhistle::Update(void)
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

	//�f�o�b�O�\��
	pDebugProc->Print("\n�|�C���g�̈ʒu (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);
}

//==============================================================
// �ǔ�����
//==============================================================
void CWhistle::Follow(void)
{
	CPlayer* pPlayer = CManager::GetInstance()->GetScene()->GetGame()->GetPlayer();		// �v���C���[�̏��擾
	D3DXVECTOR3 posPlayer = pPlayer->GetPos();
	D3DXVECTOR3 rotPlayer = pPlayer->GetRot();

	m_pos.x = posPlayer.x + sinf(rotPlayer.y + D3DX_PI) * DISTANCE;
	m_pos.z = posPlayer.z + cosf(rotPlayer.y + D3DX_PI) * DISTANCE;

	// �ʒu�ݒ�
	CObject3D::SetPos(m_pos);
}

//==============================================================
//�|�C���g�̕`�揈��
//==============================================================
void CWhistle::Draw(void)
{
	// �I�u�W�F�N�g3D�̕`�揈��
	CObject3D::Draw();
}