//=================================================
// Content     (�Q�[���̐ݒ�)(Collision.cpp)
// Author     : �L�c����
//=================================================
//===========================
//�C���N���[�h
//===========================
#include"Collision.h"
#include"camera.h"
#include"shadow.h"
#include"Application.h"
#include"renderer.h"

//�}�N����`
#define NUM_BUFF		(24)		//1�̔���ɂ��K�v�ȃo�b�t�@�̐�
#define NUM_LINE		(16)	//1�̃{�b�N�X���`������̂ɕK�v�Ȑ�


//===========================
//�R���X�g���N�^
//===========================
CCollision::CCollision(int nPriority)
{
}

//===========================
//�f�X�g���N�^
//===========================
CCollision::~CCollision()
{
}

//===========================
//����������
//===========================
HRESULT CCollision::Init(void)
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
	m_rot = D3DXVECTOR3(0.0f,0.f,0.f);
	m_width = D3DXVECTOR3(50.0f, 50.f, 50.f);

	switch (m_Colltype)
	{//��ނɉ����ĐF�̐ݒ�
	case CCollision::COLLI_AXIS:
		m_col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;

	case CCollision::COLLI_DAMAGE:
		m_col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;

	case CCollision::COLLI_SLOW:
		m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;

	case CCollision::COLLI_HURT:
		m_col = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
		break;

	case CCollision::COLLI_HURTSLOW:
		m_col = D3DXCOLOR(0.0f, 1.0f, 1.0f, 1.0f);
		break;

	default:
		m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	}

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
		pVtx[i].col = m_col;
	}

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[1].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[2].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
	pVtx[3].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[4].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[6].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
	pVtx[7].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);

	pVtx[8].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[9].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[10].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
	pVtx[11].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[12].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[13].pos = D3DXVECTOR3(-m_width.x / 2,- m_width.y / 2, -m_width.z / 2);
	pVtx[14].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
	pVtx[15].pos = D3DXVECTOR3(m_width.x / 2,- m_width.y / 2, m_width.z / 2);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//==============================
//�I������
//==============================
void CCollision::Uninit(void)
{
	//�o�b�t�@�̔j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	CObject::Release();
}
//==============================
//�X�V����
//==============================
void CCollision::Update(void)
{
	if (m_pVtxBuff != NULL)
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
		pVtx[4].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[5].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[6].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[7].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);

		pVtx[8].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[9].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[10].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[11].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[12].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[13].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[14].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[15].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

		pVtx[16].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[17].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[18].pos = D3DXVECTOR3(m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[19].pos = D3DXVECTOR3(m_width.x / 2, -m_width.y / 2, -m_width.z / 2);
		pVtx[20].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, m_width.z / 2);
		pVtx[21].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, m_width.z / 2);
		pVtx[22].pos = D3DXVECTOR3(-m_width.x / 2, m_width.y / 2, -m_width.z / 2);
		pVtx[23].pos = D3DXVECTOR3(-m_width.x / 2, -m_width.y / 2, -m_width.z / 2);

		//���_�o�b�t�@���A�����b�N
		m_pVtxBuff->Unlock();
	}
}

//==============================
//�`�揈��
//==============================
void CCollision::Draw(void)
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
			0,									//���_�̊J�n�ꏊ
			NUM_LINE);							//�v���~�e�B�u�̐�
	}
}

//=====================
//����
//=====================
CCollision* CCollision::Create(D3DXVECTOR3 pos, COLLISION type)
{
	CCollision*pCollision = new CCollision();

	if (pCollision != nullptr)
	{
		pCollision->m_Colltype = type;
		pCollision->Init();
		pCollision->m_pos = pos;
		pCollision->bUse = true;
	}

	return pCollision;
}