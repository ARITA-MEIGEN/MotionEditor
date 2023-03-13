//=================================================
// Content     (�Q�[���̐ݒ�)(light.cpp)
// Author     : �L�c����
//=================================================
#include"light.h"
#include"Application.h"
#include"renderer.h"



//�O���[�o���ϐ�
//===========================
//���C�g�̏���������
//===========================
void CLight::Init(void)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXVECTOR3 vecDir;	//���C�g�̕����x�N�g��
						//���C�g���N���A����
	ZeroMemory(&m_alight[0], sizeof(D3DLIGHT9));

	//���C�g�̎�ނ�ݒ�
	m_alight[0].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[1].Type = D3DLIGHT_DIRECTIONAL;
	m_alight[2].Type = D3DLIGHT_DIRECTIONAL;

	//���C�g�̊g�U����ݒ�
	m_alight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_alight[2].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	//���C�g�P
	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);

	//���K������
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_alight[0].Direction = vecDir;

	//���C�g2
	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(-0.5f, -0.8f, -0.4f);

	//���K������
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_alight[1].Direction = vecDir;

	//���C�g3
	//���C�g�̕�����ݒ�
	vecDir = D3DXVECTOR3(-0.4f, -0.8f, 0.2f);
	vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);


	//���K������
	D3DXVec3Normalize(&vecDir, &vecDir);
	m_alight[2].Direction = vecDir;

	//���C�g��ݒ肷��
	pDevice->SetLight(0, &m_alight[0]);
	pDevice->SetLight(1, &m_alight[1]);
	pDevice->SetLight(2, &m_alight[2]);

	//���C�g��L���ɂ���
	pDevice->LightEnable(0, TRUE);
	//���C�g��L���ɂ���
	pDevice->LightEnable(1, TRUE);
	//���C�g��L���ɂ���
	pDevice->LightEnable(2, TRUE);
}
//===========================
//���C�g�̏I������
//===========================
void  CLight::Uninit(void)
{

}
//===========================
//���C�g�̍X�V����
//===========================
void  CLight::Update(void)
{

}

//===========================
//���C�g�̃x�N�g���̎擾
//===========================
D3DXVECTOR3 CLight::GetVec(int number)
{
	return m_alight[number].Direction;
}