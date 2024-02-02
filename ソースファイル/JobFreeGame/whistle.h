//==============================================================
//
// �J�͈̔͏���[whistle.h]
// Author:����������
//
//==============================================================
#ifndef _WHISTLE_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _WHISTLE_H_		// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object3D.h"

//**************************************************************
// �}�N����`
//**************************************************************

//**************************************************************
// �O���錾
//**************************************************************

//**************************************************************
// �J�͈̔̓N���X�̒�`
//**************************************************************
class CWhistle : public CObject3D
{
public:

	// �J�͈̔͂̏��
	enum STATE
	{
		STATE_NONE = 0,		// �������Ȃ����
		STATE_PLANE,		// ���ʏ��
		STATE_CYLINDER,		// �~�����
		STATE_MAX
	};

	// �W���̎��
	enum TYPE
	{
		TYPE_PLANE = 0,		// ���ʏ�
		//TYPE_CYLINDER,		// �~��
		TYPE_MAX
	};

	CWhistle();		//�R���X�g���N�^
	CWhistle(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CWhistle();		//�f�X�g���N�^

	static CWhistle* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ݒ菈��
	void SetPos(D3DXVECTOR3 pos);		// �ʒu�ݒ�
	//void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	void SetState(STATE state) { m_state = state; }		// ��Ԑݒ�

	//�擾����
	D3DXVECTOR3 GetPos(void);			// �ʒu�擾
	//D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾

	STATE GetState(void) { return m_state; }			// ��Ԏ擾

private:

	// �֐�
	void UpdateState(void);			// ��Ԃ̍X�V����
	void Follow(void);					// �ǔ�����

	// �ϐ�
	static CObject3D* m_pObject3D[TYPE_MAX];	// 3D�I�u�W�F�N�g
	int m_nIdxTexture[TYPE_MAX];					// �e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_size;		// �傫��
	int m_nWhistleCounter;	// �J��炷����
	float m_fscale;			// �g�嗦

	STATE m_state;		// ���
	TYPE m_type;		// ���
};

#endif
