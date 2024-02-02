//==============================================================
//
// ���b�V���̉~���̏���[meshcylinder.h]
// Author:����������
//
//==============================================================
#ifndef _CYLINDER_H_		// ���̃}�N����`������Ă��Ȃ�������
#define _CYLINDER_H_		// 2�d�C���N���[�h�h�~�̃}�N�����`����

#include "object.h"

//**************************************************************
// ���b�V���V�����_�[�N���X�̒�`
//**************************************************************
class CMesuCylinder : public CObject
{
public:
	CMesuCylinder();		//�R���X�g���N�^
	//CMesuCylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// �R���X�g���N�^(�I�[�o�[���[�h)
	~CMesuCylinder();		//�f�X�g���N�^

	static CMesuCylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nType);		// ��������

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);

	void BindTexture(int nIdxTexture);					//�O������e�N�X�`���w��

private:

	LPDIRECT3DTEXTURE9 m_pTexture;			//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@�̃|�C���^
	D3DXMATRIX m_mtxWorld;					//���[���h�}�g���b�N�X
	int m_nIdxTexture;		//�e�N�X�`���̔ԍ�

	D3DXVECTOR3 m_pos;		// �ʒu
	D3DXVECTOR3 m_rot;		// ����

};

#endif
