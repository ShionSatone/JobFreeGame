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
#define MAX_WHISTLE_TEX		(2)		// �W���摜�̐�

//**************************************************************
// �O���錾
//**************************************************************

//**************************************************************
// �J�͈̔̓N���X�̒�`
//**************************************************************
class CWhistle : public CObject3D
{
public:	

	// �W���̎��
	enum CALL
	{
		CALL_PLANE = 0,		// ���ʏ�
		CALL_CYLINDER,		// �~��
		CALL_MAX
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
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�ݒ�
	//void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	//void SetState(STATE state) { m_state = state; }		// ��Ԑݒ�

	//�擾����
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// �ʒu�擾
	//D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾

	//STATE GetState(void) { return m_state; }			// ��Ԏ擾

private:

	//void UpdateState(void);			// ��Ԃ̍X�V����
	void Follow(void);					// �ǔ�����

	CObject3D m_pObject3D[MAX_WHISTLE_TEX];

	int m_nIdxTexture[MAX_WHISTLE_TEX];		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
};

#endif
