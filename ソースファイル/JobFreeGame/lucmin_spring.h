//==============================================================
//
// �t�t���N�~���̏���[lucmin_spring.h]
// Author:����������
//
//==============================================================
#ifndef _LUCMIN_SPRING_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _LUCMIN_SPRING_H_		// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include "lucmin.h"

//**************************************************************
// �}�N����`
//**************************************************************

//**************************************************************
// �O���錾
//**************************************************************
class CModelHier;	//���f���̊K�w�\��
class CMotion;		//���[�V�����̊K�w�\��

//**************************************************************
// �t���N�~���N���X�̒�`
//**************************************************************
class CLucminSpring : public CLucmin
{
public:

	CLucminSpring();		//�R���X�g���N�^
	CLucminSpring(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CLucminSpring();		//�f�X�g���N�^

	static CLucminSpring *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	int GetNumAll(void) { return m_nNumAll; }			// �t���N�~���̑���

private:

	//�t���N�~���̃��[�V�������
	enum MOTIONSTATE
	{
		MOTIONSTATE_NONE = 0,		// �������ĂȂ����
		MOTIONSTATE_MOVE,			// �����Ă���
		MOTIONSTATE_JUMP,			// �W�����v���
		MOTIONSTATE_LAND,			// ���n���
		MOTIONSTATE_MAX
	};

	//�t���N�~���̃p�[�c
	enum PARTS
	{
		PARTS_BODY = 0,		// [0]��
		PARTS_HEAD,			// [1]��
		PARTS_HAIR,			// [2]��
		PARTS_LU_ARM,		// [3]���r��
		PARTS_LD_ARM,		// [4]���r��
		PARTS_L_HAND,		// [5]����
		PARTS_RU_ARM,		// [6]�E�r��
		PARTS_RD_ARM,		// [7]�E�r��
		PARTS_R_HAND,		// [8]�E��
		PARTS_WAIST,		// [9]��
		PARTS_LU_LEG,		// [10]��������
		PARTS_LD_LEG,		// [11]���ӂ���͂�
		PARTS_L_SHOE,		// [12]���C
		PARTS_RU_LEG,		// [13]�E������
		PARTS_RD_LEG,		// [14]�E�ӂ���͂�
		PARTS_R_SHOE,		// [15]�E�C
		PARTS_MAX
	};

	void MotionManager(void);		// ���[�V�����Ǘ�

	void LoadFile(void);					// ���f���t�@�C���ǂݍ���
											   
	static char *m_apFileName[PARTS_MAX];		// �t�@�C����
	static int m_nNumAll;						// �t���N�~���̑���
	int m_nIndex;								// �t���N�~���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_posDest;	// �ړI�̈ʒu

	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_rotDest;	// �ړI�̌���
	D3DXVECTOR3 m_max;		// �l�Ԃ̍ő�l
	D3DXVECTOR3 m_min;		// �l�Ԃ̍ŏ��l
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	CModelHier *m_apModel[PARTS_MAX];		// ���f��(�p�[�c)�ւ̃|�C���^
	int m_nNumModel;		// ���f��(�p�[�c)�̑���
	int m_nCntDamage;		// �_���[�W�J�E���^�[

	float m_fRotDest;		// �ڕW
	float m_fRotDiff;		// ����

	MOTIONSTATE m_MotionState;		// �t���N�~���̓����̏��
	CMotion *m_pMotion;		// ���[�V�������
};

#endif
