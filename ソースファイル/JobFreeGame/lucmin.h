//==============================================================
//
// ���N�~���̏���[lucmin.h]
// Author:����������
//
//==============================================================
#ifndef _LUCMIN_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _LUCMIN_H_		// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"
#include "game.h"

//**************************************************************
// �}�N����`
//**************************************************************

//**************************************************************
// �O���錾
//**************************************************************
class CModelHier;	//���f���̊K�w�\��
class CMotion;		//���[�V�����̊K�w�\��

//**************************************************************
// ���N�~���N���X�̒�`
//**************************************************************
class CLucmin : public CObject
{
public:

	// ���N�~���̏��
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_FOLLOW,		// �ǔ����
		STATE_THROW,		// �������Ă���
		STATE_SEARCH,		// �T�����
		STATE_ATTACK,		// �U�����
		STATE_WHISTLE,		// �Ă΂����
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATH,		// ���S���
		STATE_MAX
	};

	// ���N�~���̎��
	enum TYPE
	{
		TYPE_SPRING = 0,		// �t
		TYPE_SUMMER,			// ��
		TYPE_FALL,				// �H
		TYPE_WINTER,			// �~
		TYPE_MAX
	};

	CLucmin();		//�R���X�g���N�^
	CLucmin(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CLucmin();		//�f�X�g���N�^

	static CLucmin *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	// �ݒ菈��
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	void SetState(STATE state) { m_state = state; }		// ��Ԑݒ�
	void SetGimmickObj(CObject* pObject) { m_pObject = pObject; }		// �M�~�b�N�̃I�u�W�F�N�g���ݒ菈��

	//�擾����
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// �ʒu�擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾

	D3DXVECTOR3 GetSizeMin(void) { return m_min; }		// �傫���̍ő�l�擾
	D3DXVECTOR3 GetSizeMax(void) { return m_max; }		// �傫���̍ŏ��l�擾
	STATE GetState(void) { return m_state; }			// ��Ԏ擾
	CObject* GetGimmickObj(void) { return m_pObject; }	// �M�~�b�N�̃I�u�W�F�N�g�̏��擾

	int GetNumAll(void) { return m_nNumAll; }			// ���N�~���̑���

	bool CircleCollision(D3DXVECTOR3 pos0, D3DXVECTOR3 pos1, float fRadius0, float fRadius1);		// �~�̓����蔻��

private:

	//���N�~���̃��[�V�������
	enum MOTIONSTATE
	{
		MOTIONSTATE_NONE = 0,		// �������ĂȂ����
		MOTIONSTATE_MOVE,			// �����Ă���
		MOTIONSTATE_JUMP,			// �W�����v���
		MOTIONSTATE_LAND,			// ���n���
		MOTIONSTATE_MAX
	};

	// �Ă΂����
	enum WHISTLESTATE
	{
		WHISTLESTATE_NONE = 0,	// �����Ȃ����
		WHISTLESTATE_REACTION,	// ���A�N�V�������
		WHISTLESTATE_FOLLOW,	// �ǔ����
		WHISTLESTATE_MAX
	};

	//��������
	enum THROWSTATE
	{
		THROWSTATE_NONE = 0,		// �����Ȃ����
		THROWSTATE_STANDBY,			// �������
		THROWSTATE_THROW,			// ��������
		THROWSTATE_MAX
	};

	// �T�����
	enum SEARCHSTATE
	{
		SEARCHSTATE_NONE = 0,			// �����Ȃ����
		SEARCHSTATE_SEARCH,				// �T���Ă���
		SEARCHSTATE_FIND,				// ���������
		SEARCHSTATE_MAX
	};

	//���N�~���̃p�[�c
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

	// �֐�
	void UpdateState(void);			// ��Ԃ̍X�V����
	void UpdateWhistleState(void);	// �W����Ԃ̍X�V����
	void UpdateThrowState(void);	// ��������Ԃ̍X�V����
	void UpdateSearchState(void);	// �T����Ԃ̍X�V����

	void Search(void);				// �T������
	void Attack(void);				// �U������

	void MotionManager(void);		// ���[�V�����Ǘ�
	void FollowMove(D3DXVECTOR3 posDest);	// �v���C���[�ɂ��Ă�������

	void Screen(void);						// ��ʊO����
	void LoadFile(void);					// ���f���t�@�C���ǂݍ���
	void RotNormalize(void);				// �����̕␳����
					
	// �ϐ�
	static char *m_apFileName[PARTS_MAX];		// �t�@�C����
	static int m_nNumAll;						// ���N�~���̑���
	int m_nIndex;								// ���N�~���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_posDest;	// �ړI�̈ʒu
	D3DXVECTOR3 m_posDestSave;	// �ړI�̈ʒu�ۑ��p

	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_rotDest;	// �ړI�̌���
	D3DXVECTOR3 m_max;		// �l�Ԃ̍ő�l
	D3DXVECTOR3 m_min;		// �l�Ԃ̍ŏ��l
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	CModelHier *m_apModel[PARTS_MAX];		// ���f��(�p�[�c)�ւ̃|�C���^
	int m_nNumModel;		// ���f��(�p�[�c)�̑���
	int m_nCntDamage;		// �_���[�W�J�E���^�[

	int m_nReactionCounter;	// �W���̃��A�N�V�����J�E���^�[
	int m_nAttackCounter;	// �U���̊Ԋu�J�E���^�[

	float m_fRotDest;		// �ڕW
	float m_fRotDiff;		// ����
	float m_fGimmickRadius;	// �M�~�b�N�̔��a

	STATE m_state;			// ���N�~���̏��
	WHISTLESTATE m_whistleState;			// ���N�~���̌Ăт������
	THROWSTATE m_throwState;		// ���N�~���̓��������
	SEARCHSTATE m_searchState;		// ���N�~���̒T�����
	MOTIONSTATE m_MotionState;		// ���N�~���̓����̏��

	CMotion *m_pMotion;		// ���[�V�������
	CObject* m_pObject;		// �I�u�W�F�N�g�̏��

};

#endif
