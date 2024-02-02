//==============================================================
//
// ���b�V���̉~���̏���[meshcylinder.cpp]
// Author:����������
//
//==============================================================
#include "meshcylinder.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//**************************************************************
// �}�N����`
//**************************************************************
#define NUM_VEC		(18)		// ���_��
#define LENGTH		(500.0f)	//����

//**************************************************************
// �ÓI�����o�ϐ��錾
//**************************************************************

//==============================================================
// �R���X�g���N�^
//==============================================================
CMesuCylinder::CMesuCylinder()
{
	m_nIdxTexture = -1;		// �e�N�X�`���̔ԍ�
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// �ʒu
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// ����
}
//
////==============================================================
//// �R���X�g���N�^
////==============================================================
//CMesuCylinder::CMesuCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
//{
//	m_nIdxTexture = -1;		// �e�N�X�`���̔ԍ�
//	m_pos = pos;		// �ʒu
//	m_rot = rot;		// ����
//}

//==============================================================
// �f�X�g���N�^
//==============================================================
CMesuCylinder::~CMesuCylinder()
{

}

//==============================================================
//�e�N�X�`�����蓖�ď���
//==============================================================
void CMesuCylinder::BindTexture(int nIdxTexture)
{
	m_nIdxTexture = nIdxTexture;		//�e�N�X�`�����蓖��
}

//==============================================================
// ���b�V���V�����_�[�̐�������
//==============================================================
CMesuCylinder* CMesuCylinder::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType)
{
	CMesuCylinder* pMesuCylinder = NULL;

	if (pMesuCylinder == NULL)
	{//���������g�p����ĂȂ�������

		//���b�V���V�����_�[�̐���
		pMesuCylinder = new CMesuCylinder;
	}

	//����������
	pMesuCylinder->Init();

	return pMesuCylinder;
}

//==============================================================
// ���b�V���V�����_�[�̏���������
//==============================================================
HRESULT CMesuCylinder::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_VEC,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * NUM_VEC,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D* pVtx;				// ���_���ւ̃|�C���^
	int nCntAngle = 0;				// �p�x�̃J�E���^�[
	int nCntTex = 0;				// �e�N�X�`���̃J�E���^�[
	float fAngle = 0.0f;			// �p�x
	float fPosY = 200.0f;			// ����

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < NUM_VEC; nCnt++)
	{
		nCntTex = (nCnt % NUM_VEC) * (1.0f / (NUM_VEC * 0.5f));

		// ���_���W�̐ݒ�
		pVtx[nCnt].pos.x = m_pos.x + sinf(m_rot.y + fAngle * D3DX_PI) * LENGTH;
		pVtx[nCnt].pos.y = fPosY;
		pVtx[nCnt].pos.z = m_pos.z + cosf(m_rot.y + fAngle * D3DX_PI) * LENGTH;

		// �@���x�N�g���̐ݒ�
		pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt].tex = D3DXVECTOR2(0.0f + nCntTex, 1.0f);

		if (fAngle >= 1.0f)
		{
			fAngle *= -1.0f;
		}
		
		if (fAngle != 0.0f)
		{
			fAngle += 0.25f;
		}
		else
		{
			if (2 <= nCntAngle)
			{
				fAngle += 0.25f;
			}
			else
			{
				nCntAngle++;

			}
		}
	}

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================================================
// ���b�V���V�����_�[�̏I������
//==============================================================
void CMesuCylinder::Uninit(void)
{
	//���_�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//�I�u�W�F�N�g�i�������g�̔j���j
	CObject::Release();
}

//==============================================================
// ���b�V���V�����_�[�̍X�V����
//==============================================================
void CMesuCylinder::Update(void)
{

}

//==============================================================
// ���b�V���V�����_�[�̕`�揈��
//==============================================================
void CMesuCylinder::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�̎擾
	CTexture* pTexture = CManager::GetInstance()->GetTexture();		//�e�N�X�`���̎擾
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

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, pTexture->GetAddress(m_nIdxTexture));

	//�I�u�W�F�N�g3D�̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,			//�v���~�e�B�u�̎��
		0,												//�v���~�e�B�u�i�I�u�W�F�N�g3D�̐��j
		2);												//�`�悷��v���~�e�B�u��
}