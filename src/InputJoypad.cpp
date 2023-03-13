//=================================================
// Content     (�W���C�p�b�h�̐ݒ�)(InputJoyPad.cpp)
// Author     : �L�c����
//=================================================
#include "InputJoyPad.h"


//==========================================
//�W���C�p�b�h�̃R���X�g���N�^
//==========================================
CInputJoyPad::CInputJoyPad()
{

}

//==========================================
//�W���C�p�b�h�̃f�X�g���N�^
//==========================================
CInputJoyPad::~CInputJoyPad()
{

}
//=========================================
//�W���C�p�b�h�̏���������
//=========================================
HRESULT CInputJoyPad::Init(void)
{
	//�������̃N���A
	memset(&m_joyKeyState, 0, sizeof(XINPUT_STATE));
	memset(&m_joyKeyStateTrigger, 0, sizeof(XINPUT_STATE));

	//Xinput�̃X�e�[�g�̐ݒ�(�L���ɂ���)
	XInputEnable(true);

	//�W���C�p�b�h�̐U�������0�N���A
	ZeroMemory(&m_joyMoter, sizeof(XINPUT_VIBRATION));

	//�U������̏�����
	m_nStrength = 0;
	m_nTime = 0;

	return S_OK;
}

//=========================================
//�W���C�p�b�h�̏I������
//=========================================
void CInputJoyPad::Uninit(void)
{
	//Xinput�̃X�e�[�g�̐ݒ�(�����ɂ���)
	XInputEnable(false);
}

//=========================================
//�W���C�p�b�h�̍X�V����
//=========================================
void CInputJoyPad::Update(void)
{
	XINPUT_STATE joykeyState;	//�W���C�p�b�h�̓��͏��
								//�W���C�p�b�h�̏�Ԃ��擾
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		m_joyKeyStateTrigger.Gamepad.wButtons =
			(m_joyKeyState.Gamepad.wButtons ^ joykeyState.Gamepad.wButtons)&joykeyState.Gamepad.wButtons;	//�L�[�̃g���K�[���ۑ�
																											// �L�[�̃����[�X���ۑ�
		m_joyKeyStateRelease.Gamepad.wButtons = (m_joyKeyState.Gamepad.wButtons^ joykeyState.Gamepad.wButtons)&m_joyKeyState.Gamepad.wButtons;
		m_joyKeyState = joykeyState;	//�W���C�p�b�h�̃v���X����ۑ�
	}

	//�W���C�p�b�h�̐U��
	m_joyMoter.wLeftMotorSpeed = (WORD)m_nStrength;
	m_joyMoter.wRightMotorSpeed = (WORD)m_nStrength;
	XInputSetState(0, &m_joyMoter);

	if (m_nTime>0)
	{
		m_nTime--;
	}
	else
	{
		m_nStrength = 0;
		m_nTime = 0;
	}
}

//=========================================
//�W���C�p�b�h�̃v���X�����擾
//=========================================
bool CInputJoyPad::GetJoypadPress(JOYKEY key)
{
	return (m_joyKeyState.Gamepad.wButtons&(0x01 << key)) ? true : false;
}

//=========================================
//�W���C�p�b�h�̃g���K�[�����擾
//=========================================
bool CInputJoyPad::GetJoypadTrigger(JOYKEY key)
{
	return (m_joyKeyStateTrigger.Gamepad.wButtons&(0x01 << key)) ? true : false;
}

//=========================================
//�W���C�p�b�h�̃����[�X�����擾
//=========================================
bool CInputJoyPad::GetJoypadRelease(JOYKEY key)
{
	return (m_joyKeyStateRelease.Gamepad.wButtons&(0x01 << key)) ? true : false;;
}

//=========================================
//�W���C�p�b�h�̃X�e�B�b�N�����擾
//=========================================
D3DXVECTOR3 CInputJoyPad::GetJoypadStick(JOYKEY key)
{
	switch (key)
	{
	case JOYKEY_LSTICK:
		m_JoyStickPos = D3DXVECTOR3(m_joyKeyState.Gamepad.sThumbLX / DEADZONE, -m_joyKeyState.Gamepad.sThumbLY / DEADZONE, 0.0f);
		break;
	case JOYKEY_RSTICK:
		m_JoyStickPos = D3DXVECTOR3(m_joyKeyState.Gamepad.sThumbRX / DEADZONE, -m_joyKeyState.Gamepad.sThumbRY / DEADZONE, 0.0f);
		break;
	}
	return m_JoyStickPos;
}
//=========================================
//�R���g���[���[�̐U������
//=========================================
void CInputJoyPad::Joypadvibration(int nTime, int nStrength)
{
	m_nTime = nTime;			//�U����������
	m_nStrength = nStrength;	//�U���̋���
}
