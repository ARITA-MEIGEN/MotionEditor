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
		m_pGame->Update();
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
	m_pRenderer->Draw();

	//���[�h���Ƃ̕`��
	switch (m_mode)
	{

	case MODE_GAME:
		m_pGame->Draw();
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

////======================================================
////�X�e�[�^�X�̕\��
////======================================================
//void CApplication::DrawStatas(void)
//{
//	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[512];
//
//	//������ɑ��
//	//wsprintf(&aStr[0], "FPS:%3d\n", g_nCountFPS);
//
//	//���f���̑���L�[
//	strcat(aStr, "���f���̑���:��/��/��/��\n");
//
//	//���_�̐���L�[
//	strcat(aStr, "���_�̐���:Q/E/R/F\n");
//
//	//�����_�̐���L�[
//	strcat(aStr, "�����_�̐���:J/L/I/K\n");
//
//	//�J�����̈ʒu����L�[
//	strcat(aStr, "�J�����̈ʒu����:W/A/S/D\n");
//
//	////���_�̍��W
//	//char aStr2[400];
//	//sprintf(aStr2, "���_�̈ʒu:%3f/%3f/%3f\n", m_pCamera->posV.x, m_pCamera->posV.y, m_pCamera->posV.z);
//	//strcat(aStr, aStr2);
//
//	////�����_�̍��W
//	//char aStr3[400];
//	//sprintf(aStr3, "�����_�̈ʒu:%3f/%3f/%3f\n", m_pCamera->posR.x, m_pCamera->posR.y, m_pCamera->posR.z);
//	//strcat(aStr, aStr3);
//
//	////���C���[�t���[��
//	//strcat(aStr, "���C���[�t���[��ON/OFF:1/2\n");
//
//	////���f���̑���L�[
//	//strcat(aStr, "���˃@�I:SPACE�������񂾁I\n");
//
//	//�e�L�X�g�̕`��
//	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
//}

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