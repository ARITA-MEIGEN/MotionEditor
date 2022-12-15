//=================================================
// Content     (�Q�[���̐ݒ�)(line.cpp)
// Author     : �L�c����
//=================================================
//===========================
//�C���N���[�h
//===========================
#include"line.h"
#include"camera.h"
#include"shadow.h"
#include"Application.h"
#include"renderer.h"

//�}�N����`
#define NUM_BUFF	(8)		//1�̔���ɂ��K�v�ȃo�b�t�@�̐�
#define NUM_LINE	(4)		//1�̒i�̃o�b�t�@�̐�

CLine::CLine(int nPriority)
{
}

CLine::~CLine()
{
}

//===========================
//����������
//===========================
HRESULT CLine::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * NUM_BUFF,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//���̏�����
	bUse = false;

	//���_���W�ւ̃|�C���^
	VERTEX_3D*pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < NUM_BUFF; i++)	//��i
	{
		//���_���W�̐ݒ�
		//�@���x�N�g���̐ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		//�F�̐ݒ�
		pVtx[i].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[2].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[3].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);

	pVtx[4].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[6].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[7].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================
//�I������
//==============================
void CLine::Uninit(void)
{
	//�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}
//==============================
//�X�V����
//==============================
void CLine::Update(void)
{
	//���_���W�ւ̃|�C���^
	VERTEX_3D*pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[2].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[3].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);

	pVtx[4].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[6].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[7].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//==============================
//�`�揈��
//==============================
void CLine::Draw(void)
{
	if (bUse == true)
	{
		LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
		pDevice = CApplication::GetRenderer()->GetDevice();

		D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

		//���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		//�e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, NULL);

		//�|���S���̕`��       
		pDevice->DrawPrimitive(D3DPT_LINELIST,	//�|���S���̌`(��)
			0,							//���_�̊J�n�ꏊ
			NUM_LINE*3);						//�v���~�e�B�u�̐�
	}
}

//=====================
//����
//=====================
CLine* CLine::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLine*pLine = new CLine();

	if (pLine != nullptr)
	{
		pLine->Init();
		pLine->m_pos = pos;
		pLine->m_rot = pos;

	}

	return pLine;
}

