//==============================================================
//
// �M�~�b�N�̏���[gimmick.h]
// Author:����������
//
//==============================================================
#ifndef _GIMMICK_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _GIMMICK_H_		// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include "objectX.h"

//**************************************************************
// �}�N����`
//**************************************************************

//**************************************************************
// �O���錾
//**************************************************************

//**************************************************************
// �M�~�b�N�N���X�̒�`
//**************************************************************
class CGimmick : public CObjectX
{
public:

	// �M�~�b�N�̏��
	enum STATE
	{
		STATE_NONE = 0,		// �ʏ���
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATHSTANDBY,	// ���S�������
		STATE_DEATH,		// ���S���

		STATE_MAX
	};

	// �M�~�b�N�̎��
	enum TYPE
	{
		TYPE_NONE = 0,				// �����Ȃ�
		TYPE_SOILWALL,				// �y�̕�
		TYPE_MAX
	};

	CGimmick();		//�R���X�g���N�^
	CGimmick(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);		//�R���X�g���N�^(�I�[�o�[���[�h)
	~CGimmick();		//�f�X�g���N�^

	static CGimmick* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, TYPE type);		//��������

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void Hit(void);		// �q�b�g����

	// �ݒ菈��
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		// �ʒu�ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		// �����ݒ�
	void SetState(STATE state) { m_state = state; }		// ��Ԑݒ�

	//�擾����
	D3DXVECTOR3 GetPos(void) { return m_pos; }			// �ʒu�擾
	D3DXVECTOR3 GetRot(void) { return m_rot; }			// �����擾

	//D3DXVECTOR3 GetSizeMin(void) { return m_min; }		// �傫���̍ő�l�擾
	//D3DXVECTOR3 GetSizeMax(void) { return m_max; }		// �傫���̍ŏ��l�擾
	STATE GetState(void) { return m_state; }			// ��Ԏ擾
	int GetLife(void) { return m_nLife; }				// �����擾

private:

	void UpdateState(void);			// ��Ԃ̍X�V����

	void CheckExist(void);			// �M�~�b�N�̑��݊m�F����

	void Screen(void);				// ��ʊO����
	void LoadFile(void);			// ���f���t�@�C���ǂݍ���

	static int m_nNumAll;			// �M�~�b�N�̑���
	int m_nIndex;					// �M�~�b�N�̔ԍ�
	int m_nIdxModel;				// ���f���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_posDest;	// �ړI�̈ʒu

	D3DXVECTOR3 m_move;		// �ړ���
	D3DXVECTOR3 m_rot;		// ����
	D3DXVECTOR3 m_rotDest;	// �ړI�̌���
	D3DXVECTOR3 m_max;		// �l�Ԃ̍ő�l
	D3DXVECTOR3 m_min;		// �l�Ԃ̍ŏ��l
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X

	int m_nLife;			// ����
	int m_nDamageCounter;	// �_���[�W�J�E���^�[
	int m_nDeathCounter;	// ���S��Ԃɂ���܂ł̃J�E���^�[

	STATE m_state;			// �M�~�b�N�̏��
	TYPE m_type;			// �M�~�b�N�̎��
};

#endif