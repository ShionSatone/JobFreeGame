//==============================================================
//
// �M�~�b�N�̏���[gimmick.cpp]
// Author:����������
//
//==============================================================
#include "gimmick.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"
#include "material.h"
#include "debugproc.h"
#include "lucmin.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define PRIORITY		(3)		// �D�揇��
#define LIFE			(5)		// �M�~�b�N�̎���
#define DAMAGE_TIME		(10)	// �_���[�W�p������

//**************************************************************
//�ÓI�����o�ϐ��錾
//**************************************************************
int CGimmick::m_nNumAll = 0;			// �M�~�b�N�̑���

//==============================================================
// �R���X�g���N�^
//==============================================================
CGimmick::CGimmick()
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//����
	m_nIdxModel = -1;						// ���f���̔ԍ�
	m_nIndex = m_nNumAll;					// �M�~�b�N�̔ԍ�
	m_nNumAll++;							// �M�~�b�N�̑������Z
	m_type = TYPE_NONE;						// �M�~�b�N�̎��
	m_state = STATE_NONE;					// �M�~�b�N�̏��
	m_nLife = LIFE;			// ����
	m_nDamageCounter = 0;	// �_���[�W�J�E���^�[
}

//==============================================================
// �R���X�g���N�^
//==============================================================
CGimmick::CGimmick(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	m_pos = pos;		//�ʒu
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ړ���
	m_rot = rot;		//����
	m_nIdxModel = -1;						// ���f���̔ԍ�
	m_nIndex = m_nNumAll;					// �M�~�b�N�̔ԍ�
	m_nNumAll++;							// �M�~�b�N�̑������Z
	m_type = type;							// �M�~�b�N�̎��
	m_state = STATE_NONE;					// �M�~�b�N�̏��
	m_nLife = LIFE;			// ����
	m_nDamageCounter = 0;	// �_���[�W�J�E���^�[

}

//==============================================================
// �f�X�g���N�^
//==============================================================
CGimmick::~CGimmick()
{

}

//==============================================================
// ��������
//==============================================================
CGimmick* CGimmick::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type)
{
	CGimmick* pGimmick = NULL;

	if (pGimmick == NULL)
	{//���������g�p����ĂȂ�������

		//�M�~�b�N�̐���
		pGimmick = new CGimmick(pos, rot, type);
	}

	//����������
	pGimmick->Init();

	return pGimmick;
}

//==============================================================
// �M�~�b�N�̏���������
//==============================================================
HRESULT CGimmick::Init(void)
{
	CMaterial* pMaterial = CManager::GetInstance()->GetMaterial();

	//���f���̓ǂݍ���
	m_nIdxModel = pMaterial->Regist("data\\MODEL\\item_star.x");

	//�}�e���A���̊��蓖��
	CObjectX::BindMaterial(m_nIdxModel);

	//�I�u�W�F�N�gX�̏���������
	CObjectX::Init();

	//�ʒu�ݒ菈��
	CObjectX::SetPos(m_pos);

	// ��ނ̐ݒ�
	CObject::SetType(CObject::TYPE_GIMMICK);

	return S_OK;
}

//==============================================================
// �M�~�b�N�̏I������
//==============================================================
void CGimmick::Uninit(void)
{
	//�I�u�W�F�N�gX�̏I������
	CObjectX::Uninit();
}

//==============================================================
// �M�~�b�N�̍X�V����
//==============================================================
void CGimmick::Update(void)
{
	CDebugProc* pDebugProc = CManager::GetInstance()->GetDebugProc();

	// ��ԍX�V
	UpdateState();

	pDebugProc->Print("\n�M�~�b�N�̈ʒu (%f, %f, %f)\n", m_pos.x, m_pos.y, m_pos.z);

	//�I�u�W�F�N�gX�̍X�V����
	CObjectX::Update();
}

//==============================================================
// �M�~�b�N�̕`�揈��
//==============================================================
void CGimmick::Draw(void)
{
	//�I�u�W�F�N�gX�̕`�揈��
	CObjectX::Draw();
}

//==============================================================
// ��Ԃ̍X�V����
//==============================================================
void CGimmick::UpdateState(void)
{
	switch (m_state)
	{
	case CGimmick::STATE_NONE:		// �ʏ���
		break;
	case CGimmick::STATE_DAMAGE:	// �_���[�W���

		if (m_nDamageCounter >= DAMAGE_TIME)
		{ // ��莞�Ԍo�߂�����

			// �ʏ��Ԃɂ���
			m_state = STATE_NONE;
			CObjectX::SetState(CObject::STATE_NONE);

			m_nDamageCounter = 0;
		}
		else
		{
			// �_���[�W�J�E���^�[���Z
			m_nDamageCounter++;
		}

		break;
	case CGimmick::STATE_DEATH:		// ���S���

		// �M�~�b�N�̑��݊m�F����
		CheckExist();

		// �I������
		Uninit();

		break;

	default:

		// ��~����
		assert(false);

		break;
	}
}

//==============================================================
// �q�b�g����
//==============================================================
void CGimmick::Hit(void)
{
	m_nLife--;		// �������炷

	if (m_nLife > 0)
	{ // �������܂�����Ƃ�

		//��Ԑݒ�
		CObjectX::SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

		// �_���[�W��Ԃɂ���
		m_state = STATE_DAMAGE;
		CObjectX::SetState(CObject::STATE_DAMAGE);
	}
	else if(m_nLife <= 0)
	{ // �������Ȃ��Ƃ�

		// ���S��Ԃɂ���
		m_state = STATE_DEATH;
	}
}

//==============================================================
// �M�~�b�N�̑��݊m�F����
//==============================================================
void CGimmick::CheckExist(void)
{
	CLucmin* pLucmin = nullptr;

	for (int nCnt = 0; nCnt < MAX_LUCMIN; nCnt++)
	{
		// ���N�~���̏��擾
		pLucmin = CManager::GetInstance()->GetScene()->GetGame()->GetLucmin(nCnt);

		if (pLucmin != nullptr)
		{ // ���N�~���� NULL ����Ȃ��ꍇ

			if (pLucmin->GetGimmickObj() != nullptr && pLucmin->GetState() == CLucmin::STATE_ATTACK)
			{ // �M�~�b�N�̃I�u�W�F�N�g�� NULL ����Ȃ� && �U����Ԃ̏ꍇ

				// NULL �̏���n��
				pLucmin->SetGimmickObj(nullptr);

				// NULL �̏���n��
				pLucmin = nullptr;
			}
		}

	}
}
