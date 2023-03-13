//=================================================
// Content     (�L�[�{�[�h�̐ݒ�)(inputKeyBoard.cpp)
// Author     : �L�c����
//=================================================
#include "InputKeyBoard.h"

//�}�N����`
#define NUM_KEY_MAX		(256)	//�L�[�̍ő吔

//�ÓI�����o�ϐ��錾
 LPDIRECTINPUT8 CInputKeyboard::m_pInput;						//DirectInput�I�u�W�F�N�g�ւ̃|�C���^

//==========================================
//�L�[�{�[�h�̃R���X�g���N�^
//==========================================
CInputKeyboard::CInputKeyboard()
{

}

//==========================================
//�L�[�{�[�h�̃f�X�g���N�^
//==========================================
CInputKeyboard::~CInputKeyboard()
{

}

//==========================================
//�L�[�{�[�h�̏������ݒ�
//==========================================
HRESULT CInputKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInput�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)& m_pInput, NULL)))
	{
		return E_FAIL;
	}

	//���̓f�o�C�X(�L�[�{�[�h)�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}

	//�f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	//�������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}


	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��
	m_pDevice->Acquire();

	return S_OK;
}

//==========================================
//�L�[�{�[�h�̏I������
//==========================================
void CInputKeyboard::Uninit(void)
{
	//���̓f�o�C�X(�L�[�{�[�h)�̔j��
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();		//�L�[�{�[�h�ւ̃A�N�Z�X�������
		m_pDevice->Release();
		m_pDevice = NULL;
	}

	//DirectInput�I�u�W�F�N�g�̔j��
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}

//==========================================
//�L�[�{�[�h�̍X�V����
//==========================================
void CInputKeyboard::Update(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	//�L�[�{�[�h�̓��͏��
	int nCntKey;
	//���̓f�o�C�X����f�[�^���l��
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			//�L�[�̃g���K�[���ۑ�
			m_aKeyStateTrigger[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&aKeyState[nCntKey];						
			//�L�[�̃����[�X���ۑ�
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey])&m_aKeyState[nCntKey];
			//�L�[�̃v���X���ۑ�
			m_aKeyState[nCntKey] = aKeyState[nCntKey];																			
		}
	}
	else
	{
		m_pDevice->Acquire();	//�L�[�̃A�N�Z�X���l��
	}
}

//==========================================
//�L�[�{�[�h�v���X�����擾
//==========================================
bool  CInputKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}
//=========================================
//�L�[�{�[�h�g���K�[�����擾
//=========================================
bool CInputKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;
}

//=========================================
//�L�[�{�[�h�����[�X�����擾
//=========================================
bool CInputKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

////=========================================
////�L�[�{�[�h���s�[�g�����擾
////=========================================
//bool CInputKeyboard::GetRepeat(int nKey)
//{
//	return (m_aKeyStateRepeat[nKey] & 0x80) ? true : false;
//}