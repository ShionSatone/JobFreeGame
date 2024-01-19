//==============================================================
//
//�I�u�W�F�N�g3D����[object3D.h]
//Author:����������
//
//==============================================================
#ifndef _OBJECT3D_H_		//���̃}�N����`������Ă��Ȃ�������
#define _OBJECT3D_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//**************************************************************
//�I�u�W�F�N�g3D�N���X
//**************************************************************
class CObject3D : public CObject
{
public:
	CObject3D();	//�R���X�g���N�^
	~CObject3D();	//�f�X�g���N�^

	static CObject3D *Create(void);		//��������

	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��

	void BindTexture(int nIdxTexture);					//�O������e�N�X�`���w��

	// �Z�b�g�E�Q�b�g�֐�
	void SetType(TYPE type) { m_type = type; /*CObject::SetType(m_type);*/ }		//�I�u�W�F�N�g�̎�ނ̐ݒ�
	void SetSize(D3DXVECTOR3 m_size);							//�I�u�W�F�N�g�̃T�C�Y�ݒ�
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//�ʒu�ݒ�

	TYPE GetType(void) { return m_type; }					//��ގ擾
	D3DXVECTOR3 GetPos(void) { return m_pos; }				//�ʒu�擾
	D3DXVECTOR3 GetSize(void) { return m_size; }			//�T�C�Y�擾

private:
	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		//�ʒu
	D3DXVECTOR3 m_move;		//�ړ���
	D3DXVECTOR3 m_size;		//�T�C�Y
	D3DXVECTOR3 m_rot;		//����
	D3DXCOLOR m_col;		//�F
	float m_fHeight;		//����
	float m_fWidth;			//����
	TYPE m_type;			//���
};

#endif
