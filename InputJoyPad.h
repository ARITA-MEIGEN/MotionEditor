#ifndef _INPUTJOYPAD_H_
#define _INPUTJOYPAD_H_	//��d�C���N���[�h�h�~�p

#include "main.h"

//�}�N����`
#define NUM_KEY_MAX		(256)					//�L�[�̍ő吔
#define DEADZONE		(20000.0f)				//�X�e�B�b�N�̔F�������


class CInputJoyPad
{
public:
	//�L�[�̎��
	typedef enum
	{
		JOYKEY_UP = 0,	//�\���L�[(��)
		JOYKEY_DOWN,	//�\���L�[(��)
		JOYKEY_LEFT,	//�\���L�[(��)
		JOYKEY_RIGHT,	//�\���L�[(�E)
		JOYKEY_START,	//�X�^�[�g�{�^��
		JOYKEY_BACK,	//�o�b�N�{�^��
		JOYKEY_LPUSH,	//L�X�e�B�b�N��������
		JOYKEY_RPUSH,	//R�X�e�B�b�N��������
		JOYKEY_LB,		//L�{�^��
		JOYKEY_RB,		//R�{�^��
		JOYKEY_NONE1,	//�����Ȃ�
		JOYKEY_NONE2,	//�����Ȃ�
		JOYKEY_A,		//A�{�^��
		JOYKEY_B,		//B�{�^��
		JOYKEY_X,		//X�{�^��
		JOYKEY_Y,		//Y�{�^��
		JOYKEY_LSTICK,	//���X�e�B�b�N
		JOYKEY_RSTICK,	//�E�X�e�B�b�N
	}JOYKEY;

	CInputJoyPad();
	~CInputJoyPad();
	HRESULT Init();	//������
	void Uninit();									//�I��
	void Update();									//�X�V

	bool GetJoypadPress(JOYKEY key);						//�L�[�{�[�h�v���X�����擾
	bool GetJoypadTrigger(JOYKEY key);						//�L�[�{�[�h�g���K�[�����擾
	bool GetJoypadRelease(JOYKEY key);						//�L�[�{�[�h�����[�X�����擾
	D3DXVECTOR3 GetJoypadStick(JOYKEY key);					//�W���C�p�b�h�̃X�e�B�b�N���
	void Joypadvibration(int nTime, int nStrength);			//�W���C�p�b�h�̐U���ݒ�

private:
	//�����o�ϐ�
	XINPUT_STATE m_joyKeyState;						//�W���C�p�b�h�̃v���X���
	XINPUT_STATE m_joyKeyStateTrigger;				//�W���C�p�b�h�̃g���K�[���
	XINPUT_STATE m_joyKeyStateRelease;				//�W���C�p�b�h�̃����[�X���
	D3DXVECTOR3 m_JoyStickPos;						//�W���C�X�e�B�b�N
	XINPUT_VIBRATION m_joyMoter;					//�U���̐ݒ�
	int m_nTime;									//�U���̎���
	int m_nStrength;								//�U���̋���
};

#endif
