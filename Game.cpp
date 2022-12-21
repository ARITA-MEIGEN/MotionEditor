//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
//�C���N���[�h
#include"main.h"
#include"Application.h"
#include"Game.h"
#include"Player.h"
#include"InputKeyBoard.h"
#include"InputJoyPad.h"
#include"Floor.h"
#include"Player.h"
#include"Mesh.h"
#include"Camera.h"
#include"Light.h"
#include"Shadow.h"
#include"Fade.h"
#include"Collision.h"

//�ÓI�����o�ϐ�
CPlayer*CGame::m_pPlayer = nullptr;
CBg*CGame::m_pBg = nullptr;
//CLife*CGame::m_Life = nullptr;				//�̗̓Q�[�W
CGame::GAME CGame::m_gamestate;

CCamera*CGame::m_pCamera = nullptr;
CLight*CGame::m_pLight = nullptr;
CFloor*CGame::m_pFloor = nullptr;
CMesh*CGame::m_pMesh = nullptr;
//====================================
//�R���X�g���N�^
//====================================
CGame::CGame()
{
	
}

//====================================
//�f�X�g���N�^
//====================================
CGame::~CGame()
{

}

//====================================
//������
//====================================
HRESULT CGame::Init()
{
	//�e�N�X�`���̓ǂݍ���
	CShadow::Load();
	CFloor::Load();

	//�}�b�v�f�[�^�ǂݍ���
	//�v���C���[�̐���
	m_pPlayer=CPlayer::Create();

	//�w�i�̐���
	//���b�V���t�B�[���h�̐ݒ�
	//m_pMesh = CMesh::Create(20, 20,D3DXVECTOR3 (-200.0f, 0.0f, 200.0f));

	//�J�����̐ݒ�
	m_pCamera = CCamera::Create();

	//���C�g�̐ݒ�
	m_pLight = new CLight;
	m_pLight->Init();

	CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f,500.0f));

	return S_OK;
}

//====================================
//�I��
//====================================
void CGame::Uninit()
{
	//�e�N�X�`���̔j��
	CShadow::Unload();
	CFloor::Unload();

	//�J�����̐ݒ�
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera=nullptr;

	}

	//���C�g�̐ݒ�
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
}

//====================================
//�X�V
//====================================
void CGame::Update()
{
	m_pCamera->Update();
	m_pLight->Update();
}

//====================================
//�`��
//====================================
void CGame::Draw()
{
	m_pCamera->Set();
}

//====================================
//�v���C���[�̎擾
//====================================
CPlayer * CGame::GetPlayer()
{
	return m_pPlayer;
}

//====================================
//�Q�[���̏�Ԑݒ�
//====================================
void CGame::SetGame(GAME gamestate)
{
	m_gamestate = gamestate;
}

//====================================
//�Q�[���̏�Ԋl��
//====================================
CGame::GAME CGame::GetGame()
{
	return m_gamestate;
}

//====================================
//�J�����̎擾
//====================================
CCamera * CGame::GetCamera()
{
	return m_pCamera;
}

//====================================
//���̏��
//====================================
CFloor * CGame::GetFloor()
{
	return m_pFloor;
}

//====================================
//���̏��
//====================================
CMesh * CGame::GetMesh()
{
	return m_pMesh;
}

//====================================
//���C�g�̏��
//====================================
CLight * CGame::GetLight()
{
	return m_pLight;
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