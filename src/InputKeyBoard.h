#ifndef _INPUTKEYBOARD_H_
#define _INPUTKEYBOARD_H_	//��d�C���N���[�h�h�~�p

#include "main.h"
//�}�N����`
#define NUM_KEY_MAX		(256)					//�L�[�̍ő吔

class CInputKeyboard
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//������
	void Uninit();									//�I��
	void Update();									//�X�V
	bool GetPress(int nKey);						//�L�[�{�[�h�v���X�����擾
	bool GetTrigger(int nKey);						//�L�[�{�[�h�g���K�[�����擾
	bool GetRelease(int nKey);						//�L�[�{�[�h�����[�X�����擾

private:
	//�����o�ϐ�
	BYTE m_aKeyState[NUM_KEY_MAX];					//�L�[�{�[�h�̃v���X���
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];			//�L�[�{�[�h�̃g���K�[���
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];			//�L�[�{�[�h�̃����[�X���
	BYTE m_aKeyStateRepeat[NUM_KEY_MAX];			//�L�[�{�[�h�̃��s�[�g���

protected:
	//�����o�ϐ�
	static LPDIRECTINPUT8 m_pInput;						//DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_pDevice;						//���̓f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
};

#endif