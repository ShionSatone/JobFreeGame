//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : ����������
//
//=============================================================================
#ifndef _SOUND_H_		//���̃}�N����`������Ă��Ȃ�������
#define _SOUND_H_		//2�d�C���N���[�h�h�~�̃}�N�����`����

#include "main.h"

//�T�E���h�N���X�̒�`
class CSound
{
public:
	//*****************************************************************************
	// �T�E���h�ꗗ
	//*****************************************************************************
	enum SOUND_LABEL
	{
		SOUND_LABEL_BGM000 = 0,			// �^�C�g��
		//SOUND_LABEL_SE_DAMAGE000,		// �_���[�W��0

		SOUND_LABEL_MAX,
	};

	//*****************************************************************************
	// �T�E���h���̍\���̒�`
	//*****************************************************************************
	struct SOUNDINFO
	{
		char* m_pFilename;	// �t�@�C����
		int m_nCntLoop;		// ���[�v�J�E���g
	};

	CSound();		//�R���X�g���N�^
	~CSound();		//�f�X�g���N�^

	HRESULT Init(HWND hWnd);
	void Uninit(void);
	static HRESULT Play(SOUND_LABEL label);
	static void Stop(SOUND_LABEL label);
	static void Stop(void);

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);

private:

	IXAudio2 *m_pXAudio2;									// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice *m_pMasteringVoice;				// �}�X�^�[�{�C�X
	static IXAudio2SourceVoice *m_apSourceVoice[SOUND_LABEL_MAX];	// �\�[�X�{�C�X
	static BYTE *m_apDataAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^
	static DWORD m_aSizeAudio[SOUND_LABEL_MAX];					// �I�[�f�B�I�f�[�^�T�C�Y
	static SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX];
};


#endif