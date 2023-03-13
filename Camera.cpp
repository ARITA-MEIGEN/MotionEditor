//=================================================
// Content     (�Q�[���̐ݒ�)(camera.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
#include"camera.h"
#include "renderer.h"
#include"Application.h"
#include"InputKeyBoard.h"
#include"DebugProc.h"

//�}�N����`
#define	DISTANCE_X	((m_posV.x - m_posR.x)*(m_posV.x - m_posR.x))		//�����̌v�Z�pX
#define	DISTANCE_Y	((m_posV.y - m_posR.y)*(m_posV.y - m_posR.y))		//�����̌v�Z�pY
#define DISTANCE_Z	((m_posV.z - m_posR.z)*(m_posV.z - m_posR.z))		//�����̌v�Z�pZ
#define CAMERA_SPEED	(5.0f)		//�J�����̈ړ����x
#define CAMERA_FACTOR	(0.01f)		//�J�����̒Ǐ]�̌W��
#define CAMERA_ROTSPEED	(0.05f)		//�J�����̐��񑬓x

//===========================
//�R���X�g���N�^
//===========================
CCamera::CCamera()
{
}

//===========================
//�f�X�g���N�^
//===========================
CCamera::~CCamera()
{
}

//===========================
//�J�����̏���������
//===========================
void CCamera::Init(void)
{
	//���_�E�����_�E�������ݒ�
	m_posV = D3DXVECTOR3(0.0f, 100.0f, -100.0f);
	m_posR = D3DXVECTOR3(40.0f, 0.0f, 10.0f);
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot.x = atan2f((m_posV.y - m_posR.y), (m_posV.z - m_posR.z));
	//m_fDistance=sqrtf(m_fDistance*m_fDistance+DISTANCE_Y);	
	m_fDistance = sqrtf(DISTANCE_X + DISTANCE_Y + DISTANCE_Z);

	m_posV = D3DXVECTOR3(-8.0f, 100.0f, 160.0f);
	m_posR = D3DXVECTOR3(-20.0f, 58.0f, 15.0f);
	m_rot = D3DXVECTOR3(1.8f, -3.0f, 0.0f);

}

//===========================
//�J�����̏I������
//===========================
void  CCamera::Uninit(void)
{

}

//===========================
//�J�����̍X�V����
//===========================
void  CCamera::Update(void)
{
	ControlPos();		//���_����
	RadianPosR();		//�����_�̐���
	RadianPosV();		//���_�̐���
	//DestPos();		//�J�����̒Ǐ]
	NormalizeRadian();	//�p�x�̐��K��
}

//===========================
//�J�����̐ݒ菈��
//===========================
void  CCamera::Set(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�r���[�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxView);

	//�r���[�}�g���b�N�X�̍쐬
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);

	//�r���[�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//�v���W�F�N�V�����}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxProjection);

	//�v���W�F�N�V�����}�g���b�N�X�̍쐬
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),								//����p
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,			//�A�X�y�N�g��
		1.0f,												//�ǂ�����
		2000.0f);											//�ǂ��܂ŕ`�悷�邩�̐ݒ�

															//�v���W�F�N�V�����}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_PROJECTION,
		&m_mtxProjection);									//�v���W�F�N�V�������W�s��̐ݒ�
}

//===========================
//�����_�̐���
//===========================
void CCamera::RadianPosR(void)
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//�����_�̐���
	if (pKeyboard->GetPress(DIK_J) == true)
	{
		m_rot.y -= CAMERA_ROTSPEED;
	}
	else if (pKeyboard->GetPress(DIK_L) == true)
	{
		m_rot.y += CAMERA_ROTSPEED;
	}
	//�c
	if (pKeyboard->GetPress(DIK_I) == true)
	{//���� 
		m_rot.x -= CAMERA_ROTSPEED;
	}
	else if (pKeyboard->GetPress(DIK_K) == true)
	{
		m_rot.x += CAMERA_ROTSPEED;
	}
	//�����_�̌v�Z
	m_posR.z = m_posV.z + sinf(m_rot.x)*cosf(m_rot.y)*m_fDistance;
	m_posR.x = m_posV.x + sinf(m_rot.x)*sinf(m_rot.y)*m_fDistance;
	m_posR.y = m_posV.y +cosf(m_rot.x)*m_fDistance;

	CDebugProc::Print("�J�����̒����_�̐��� I/J/K/L:X:%f Y:%f Z:%f", m_posR.z, m_posR.x,m_posR.y);

}

//===========================
//���_�̐���
//===========================
void CCamera::RadianPosV(void)
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//���_�̐���
	if (pKeyboard->GetPress(DIK_Q) == true)
	{
		m_rot.y -= CAMERA_ROTSPEED;
	}
	else if (pKeyboard->GetPress(DIK_E) == true)
	{
		m_rot.y += CAMERA_ROTSPEED;
	}
	if (pKeyboard->GetPress(DIK_R) == true)
	{
		m_rot.x += CAMERA_ROTSPEED;
	}
	else if (pKeyboard->GetPress(DIK_F) == true)
	{
		m_rot.x -= CAMERA_ROTSPEED;
	}

	//���_�̐���
	m_posV.z = m_posR.z - sinf(m_rot.x)*cosf(m_rot.y)*m_fDistance;
	m_posV.x = m_posR.x - sinf(m_rot.x)*sinf(m_rot.y)*m_fDistance;
	m_posV.y = m_posR.y - cosf(m_rot.x)*m_fDistance;

	CDebugProc::Print("�J�����̎��_�̐��� Q/E/R/F x:%f y:%f z:%f", m_posV.x, m_posV.y, m_posV.z);
}

//===========================
//�p�x�̐��K��
//===========================
void CCamera::NormalizeRadian(void)
{
	//�p�x�̐��K��
	if (m_rot.y >= D3DX_PI)
	{//y
		m_rot.y = -D3DX_PI;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y = D3DX_PI;
	}

	if (m_rot.x >= D3DX_PI)
	{//x
		m_rot.x = D3DX_PI;
	}
	else if (m_rot.x <= -D3DX_PI)
	{
		m_rot.z = -D3DX_PI;
	}

	if (m_rot.z > D3DX_PI)
	{//z
		m_rot.z = -D3DX_PI;
	}
	else if (m_rot.z < -D3DX_PI)
	{
		m_rot.z = D3DX_PI;
	}
}

//===========================
//�J�����̈ʒu�̑���
//===========================
void CCamera:: ControlPos(void)
{
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//�J�����̈ʒu�̑���
	if (pKeyboard->GetPress(DIK_W) == true)
	{//�O
	 //���sinf/cosf�Ŋp�x�����߂Ă��瑬�x��������
		m_posV.x += sinf(D3DX_PI*0.0f + m_rot.y) *CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI*0.0f + m_rot.y) *CAMERA_SPEED;
		m_posR.x = m_posV.x + sinf(m_rot.y) *m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.y) *m_fDistance;
	}
	else if (pKeyboard->GetPress(DIK_S) == true)
	{//���
		m_posV.x += sinf(D3DX_PI*1.0f + m_rot.y)*CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI*1.0f + m_rot.y)*CAMERA_SPEED;
		m_posR.x = m_posV.x + sinf(m_rot.y)*m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.y)*m_fDistance;
	}
	if (pKeyboard->GetPress(DIK_D) == true)
	{//�E
		m_posV.x += sinf(D3DX_PI*0.5f + m_rot.y)*CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI*0.5f + m_rot.y)*CAMERA_SPEED;
		m_posR.x = m_posV.x + sinf(m_rot.y)*m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.y)*m_fDistance;
	}
	else if (pKeyboard->GetPress(DIK_A) == true)
	{//��
		m_posV.x += sinf(D3DX_PI*-0.5f + m_rot.y)*CAMERA_SPEED;
		m_posV.z += cosf(D3DX_PI*-0.5f + m_rot.y)*CAMERA_SPEED;
		m_posR.x = m_posV.x + sinf(m_rot.y)*m_fDistance;
		m_posR.z = m_posV.z + cosf(m_rot.y)*m_fDistance;
	}
	//else if (pKeyboard->GetPress(DIK_Z) == true)
	//{//��

	//}
	//else if (pKeyboard->GetPress(DIK_X) == true)
	//{//��

	//}

	CDebugProc::Print("�J�����̎��_�̈ړ� W/A/S/D");
}

//===========================
//�����̎擾
//===========================
D3DXVECTOR3 CCamera::Getrot(void)
{
	return m_rot;
}

//===========================
//����
//===========================
CCamera * CCamera::Create(void)
{
	CCamera* pCamera = new CCamera;
	if (pCamera != nullptr)
	{
		pCamera->Init();
	}

	return pCamera;
}
