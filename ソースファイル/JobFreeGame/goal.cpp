//==============================================================
//
//�S�[������[goal.cpp]
//Author:����������
//
//==============================================================
#include "goal.h"
#include "manager.h"
#include "renderer.h"
#include "material.h"
#include "player.h"
#include "game.h"
#include "sound.h"

//**************************************************************
//�}�N����`
//**************************************************************
#define NUM_MODEL			(2)			//�S�[���̎��
#define POS					(10.0f)		//pos�����l
#define MODEL_LEN			(3.0f)		//����
#define MOVE_DOWN			(-10.0f)		//�����ړ���

//**************************************************************
//�ÓI�����o�ϐ��錾
//**************************************************************
LPD3DXMESH CGoal::m_pMesh = NULL;						//���b�V���i���_���j�ւ̃|�C���^
LPD3DXBUFFER CGoal::m_pBuffMat = NULL;					//�}�e���A���ւ̃|�C���^
DWORD CGoal::m_dwNumMat = NULL;
const char* CGoal::m_pFilename = NULL;					//�t�@�C���̖��O

//==============================================================
//�R���X�g���N�^
//==============================================================
CGoal::CGoal()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�O��̈ʒu
	m_move = D3DXVECTOR3(0.0f, MOVE_DOWN, 0.0f);		//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����

	m_bActionDown = false;		//�u���b�N�𗎂Ƃ���
	m_bDown = false;			//���Ƃ���Ă邩
	m_bCollision = false;		//���̃u���b�N�Ɠ������Ă邩
}

//==============================================================
//�R���X�g���N�^(�I�[�o�[���[�h)
//==============================================================
CGoal::CGoal(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	m_pos = pos;		//�ʒu
	m_posOld = m_pos;	//�O��̈ʒu
	m_move = D3DXVECTOR3(0.0f, MOVE_DOWN, 0.0f);		//�ړ���
	m_rot = rot;		//����

	m_bActionDown = false;		//�u���b�N�𗎂Ƃ���
	m_bDown = false;			//���Ƃ���Ă邩
	m_bCollision = false;		//���̃u���b�N�Ɠ������Ă邩

}

//==============================================================
//�f�X�g���N�^
//==============================================================
CGoal::~CGoal()
{

}

//==============================================================
//X�t�@�C���̓ǂݍ���
//==============================================================
HRESULT CGoal::Load(const char* pFilename)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(pFilename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_dwNumMat,
		&m_pMesh);

	return S_OK;
}

//==============================================================
//�}�e���A���E���b�V���̔j��
//==============================================================
void CGoal::Unload(void)
{
	//���b�V���̔j��
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}

	//�}�e���A���̔j��
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//==============================================================
//�S�[���̐�������
//==============================================================
CGoal* CGoal::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char* pFilename)
{
	CGoal* pGoal = NULL;

	if (pGoal == NULL)
	{//���������g�p����ĂȂ�������

		//�S�[���̐���
		pGoal = new CGoal(pos, rot);
	}

	//�t�@�C�������
	m_pFilename = pFilename;

	//����������
	pGoal->Init();

	return pGoal;
}

//==============================================================
//�S�[���̏���������
//==============================================================
HRESULT CGoal::Init(void)
{
	CMaterial* pMaterial = CManager::GetInstance()->GetMaterial();

	//�S�[���̓ǂݍ���
	m_nIdxGoal = pMaterial->Regist(m_pFilename);

	//�}�e���A���̊��蓖��
	CObjectX::BindMaterial(m_nIdxGoal);

	//�I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	//�ʒu�ݒ菈��
	CObjectX::SetPos(m_pos);

	//��ނ̐ݒ�
	m_type = CObject::TYPE_GOAL;

	CObject::SetType(m_type);

	return S_OK;
}

//==============================================================
//�S�[���̏I������
//==============================================================
void CGoal::Uninit(void)
{
	//�I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//==============================================================
//�S�[���̍X�V����
//==============================================================
void CGoal::Update(void)
{

	//�I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}

//==============================================================
//�S�[���̕`�揈��
//==============================================================
void CGoal::Draw(void)
{
	//�I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}

//==============================================================
//�I�u�W�F�N�g�̎�ސݒ菈��
//==============================================================
void CGoal::SetType(TYPE type)
{
	m_type = type;

	//��ނ̐ݒ�
	CObject::SetType(m_type);
}

//==============================================================
//�S�[���̃q�b�g����
//==============================================================
void CGoal::Hit(void)
{
	/*CPlayer* pPlayer = CGame::GetPlayer();
	CSound* pSound = CManager::GetInstance()->GetSound();*/

	
}