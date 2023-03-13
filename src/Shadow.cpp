//=============================================================================
//
// 3D�I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include "Object3D.h"
#include "renderer.h"
#include"Application.h"
#include"Shadow.h"

//�ÓI�����o�ϐ�
 LPDIRECT3DTEXTURE9 CShadow::m_pTexture;

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CShadow::CShadow(int nPriority) :CObject3D(nPriority)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CShadow::~CShadow()
{

}

//=============================================================================
// ������
//=============================================================================
HRESULT  CShadow::Init()
{
	CObject3D::Init();
	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	return S_OK;
}

//=============================================================================
//�I��
//=============================================================================
void  CShadow::Uninit()
{
	CObject3D::Uninit();
}

//=============================================================================
// �X�V
//=============================================================================
void  CShadow::Update()
{
	CObject3D::Update();
}

//=============================================================================
// �`��
//=============================================================================
void  CShadow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//���Z�����̐ݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);
	CObject3D::Draw();
	pDevice->SetTexture(0, NULL);

	//�ݒ�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================================
// ����
//=============================================================================
CShadow * CShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz)
{
	CShadow*pShadow;
	pShadow = new CShadow(0);
	pShadow->Init();
	pShadow->SetPos(pos);
	pShadow->SetSiz(siz);

	return pShadow;
}


//=============================================================================
// �f�[�^��ǂݍ���
//=============================================================================
void CShadow::Load()
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	//�e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\shadow000.jpg",
		&m_pTexture);
}

//=============================================================================
// �f�[�^��j��
//=============================================================================
void CShadow::Unload()
{
	if (m_pTexture != nullptr)
	{//�e�N�X�`���̔j��
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}