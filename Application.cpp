//=============================================================================
//
// �A�v���P�[�V����
// Author : �L�c����
//
//=============================================================================

// �C���N���[�h
#include "Application.h"
#include"renderer.h"
#include"main.h"
#include"InputKeyBoard.h"
#include"InputJoyPad.h"
#include"DebugProc.h"
#include"Object.h"
#include"Fade.h"

#include"Title.h"
#include"Game.h"
#include"Result.h"

//====================================
//�ÓI�����o�ϐ�
//====================================
CRenderer*CApplication::m_pRenderer = nullptr;
CInputKeyboard*CApplication::m_pInputKeyboard = nullptr;
CInputJoyPad*CApplication::m_pInputJoyPad = nullptr;
CDebugProc*CApplication::m_pDebugProc = nullptr;

//��ʑJ�ڎ���
CApplication::MODE CApplication::m_mode;
CGame*CApplication::m_pGame = nullptr;
//====================================
//�R���X�g���N�^
//====================================
CApplication::CApplication()
{

}

//====================================
//�f�X�g���N�^
//====================================
CApplication::~CApplication()
{

}

//====================================
//������
//====================================
HRESULT CApplication::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	m_pRenderer = new CRenderer;
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputJoyPad = new CInputJoyPad;

	//����������
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//�����������Ɏ��s�����ꍇ
		return-1;
	}

	//�L�[�{�[�h�̐���
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return-1;
	}

	//�W���C�p�b�h�̐���(XInput)
	if (FAILED(m_pInputJoyPad->Init()))
	{
		return-1;
	}

	SetMode(MODE_GAME);

	//----------------------------
	// �f�o�b�O�p�����̐���
	//----------------------------
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();

	return S_OK;
}

//====================================
//�I��
//====================================
void CApplication::Uninit()
{
	//----------------------------
	//�����_�����O�N���X�̔j��
	//----------------------------
	if (m_pRenderer != nullptr)
	{
		// �I������
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//----------------------------
	//�L�[�{�[�h�̔j��
	//----------------------------
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//----------------------------
	//�W���C�p�b�h�̔j��
	//----------------------------
	if (m_pInputJoyPad != nullptr)
	{
		m_pInputJoyPad->Uninit();
		delete m_pInputJoyPad;
		m_pInputJoyPad = nullptr;
	}

	//----------------------------
	// �Q�[���̏I��
	//----------------------------
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}
	
	//----------------------------
	// �f�o�b�O�p�����̏I��
	//----------------------------
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}
}

//====================================
//�X�V
//====================================
void CApplication::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//�f�o�C�X�ւ̃|�C���^
	//���C���[�t���[��
	if (m_pInputKeyboard->GetTrigger(DIK_1))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (m_pInputKeyboard->GetTrigger(DIK_2))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	CDebugProc::Print("���C���[�t���[���̐ݒ�: F1/F2");


	//�����_���[�X�V
	m_pRenderer->Update();
	
	//�L�[�{�[�h�̍X�V
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Update();
	}

	//�W���C�p�b�h�̍X�V
	if (m_pInputJoyPad!=nullptr)
	{
		m_pInputJoyPad->Update();
	}

	//���[�h���Ƃ̍X�V
	switch (m_mode)
	{
	case MODE_GAME:
		if (m_pGame != nullptr)
		{
			m_pGame->Update();
		}
		break;

	default:
		break;
	}
}

//====================================
//�`��
//====================================
void CApplication::Draw()
{
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}

	//���[�h���Ƃ̕`��
	switch (m_mode)
	{

	case MODE_GAME:
		if (m_pGame != nullptr)
		{
			m_pGame->Draw();
		}
		break;

	default:
		break;
	}
}

//====================================
//�����_���[�̎擾
//====================================
CRenderer * CApplication::GetRenderer()
{
	return m_pRenderer;
}

//====================================
//�L�[�{�[�h�̎擾
//====================================
CInputKeyboard * CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//====================================
//�W���C�p�b�h�̎擾
//====================================
CInputJoyPad * CApplication::GetInputJoypad()
{
	return m_pInputJoyPad;
}

//====================================
//�f�o�b�O���̎擾
//====================================
CDebugProc * CApplication::GetDebugProc()
{
	return m_pDebugProc;
}

//======================================================
//���[�h�̐ݒ�
//======================================================
void CApplication::SetMode(MODE mode)
{
	//�V�������(���[�h)�̏���������
	switch (m_mode)
	{
	case MODE_GAME:		//�Q�[�����
		m_pGame->Uninit();
		break;

	}
	m_mode = mode;	//���݂̉��(���[�h)��؂�ւ���
	CObject::ReleaseAll();

	//�V�������(���[�h)�̏���������
	switch (m_mode)
	{
	case MODE_GAME:		//�Q�[�����
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
		break;
	}
}