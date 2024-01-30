//==============================================================
//
// �|�C���g�̏���[point.h]
// Author:����������
//
//==============================================================
#ifndef _POINT_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _POINT_H_		// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object3D.h"

//**************************************************************
// �}�N����`
//**************************************************************

//**************************************************************
// �O���錾
//**************************************************************

//**************************************************************
// �|�C���g�N���X�̒�`
//**************************************************************
class CPoint : public CObject3D
{
public:

	// ���
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_LOCKON,		// ���b�N�I�����
		STATE_THROW,		// ��������
		STATE_MAX
	};

	// �F�̎��
	enum COLOR
	{
		COLOR_NONE = 0,		// �ʏ�F(�D�F)
		COLOR_SPRING,		// �t(�s���N)
		COLOR_SUMMER,		// ��(��)
		COLOR_FALL,			// �H(�I�����W)
		COLOR_WINTER,		// �~(��)
		COLOR_MAX
	};

	CPoint();		//�R���X�g���N�^
	CPoint(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CPoint();		//�f�X�g���N�^

	static CPoint* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//��������

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

	void ControlKeyboard(void);			// �L�[�{�[�h����
	//void UpdateState(void);			// ��Ԃ̍X�V����
	void Screen(void);					// ��ʊO����
	void UpdateState(void);				// ��ԍX�V����
	void UpdateColor(void);				// �F�X�V����
	void Follow(void);					// �ǔ�����

	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu

	STATE m_state;			// ���
	COLOR m_color;			// �F
};

#endif
