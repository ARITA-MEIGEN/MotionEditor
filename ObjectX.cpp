//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
#include"ObjectX.h"
#include"Game.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Light.h"

//�}�N����`
#define PLAYER_SPEED	(2.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x
#define NUM_MODELPARTS	(1)		//���f���̃p�[�c��

static int g_nIdxShadow;		//�e��ID

//===========================
//�R���X�g���N�^
//===========================
CObjectX::CObjectX(int nPriority) :CObject(nPriority)
{

}

//===========================
//�f�X�g���N�^
//===========================
CObjectX::~CObjectX()
{

}

//===========================
//����������
//===========================
HRESULT CObjectX::Init()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	
	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Yint nNumVtx;		//���_��
	BYTE*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^

	//X�t�@�C���̓ǂݍ���
	D3DXLoadMeshFromX(m_modelname,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);

		//���_�o�b�t�@�̃��b�N
		m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		//���_���̎擾
		nNumVtx = m_pMesh->GetNumVertices();

		//���_�t�H�[�}�b�g�̃T�C�Y���擾
		sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());

		//���_���W�̑��
		//���ׂĂ̒��_��pos���擾����
		D3DXVECTOR3 vtxMax = D3DXVECTOR3(-1000.0f, -1000.0f, -1000.0f);	//�ő�l�̕ۑ��p
		D3DXVECTOR3 vtxMin = D3DXVECTOR3(1000.0f, 1000.0f, 1000.0f);	//�ŏ��l�̕ۑ��p
		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;
			//���_���W���r���ă��f���̍ŏ��l�ő�l���擾
			if (vtx.x > vtxMax.x)
			{//X�̍ő�l���擾
				vtxMax.x = vtx.x;
			}
			if (vtx.x < vtxMin.x)
			{//X�̍ŏ��l���擾
				vtxMin.x = vtx.x;
			}
			if (vtx.y > vtxMax.y)
			{//Y�̍ő�l���擾
				vtxMax.y = vtx.y;
			}
			if (vtx.y < vtxMin.y)
			{//Y�̍ŏ��l���擾
				vtxMin.y = vtx.y;
			}
			if (vtx.z > vtxMax.z)
			{//Z�̍ő�l���擾
				vtxMax.z = vtx.z;
			}
			if (vtx.z < vtxMin.z)
			{//Z�̍ŏ��l���擾
				vtxMin.z = vtx.z;
			}

			//���_�t�H�[�}�b�g�̃T�C�Y���|�C���^��i�߂�
			pVtxBuff += sizeFVF;
		}
		//���_�o�b�t�@�̃A�����b�N
		m_pMesh->UnlockVertexBuffer();

		//���̏�����
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//���W
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//����
		m_vtxMax = vtxMax;							//���_���W�̍ő�l
		m_vtxMin = vtxMin;							//���_���W�̍ŏ��l
		m_size = vtxMax - vtxMin;					//�p�[�c�̃T�C�Y
	
	//���̏�����
	m_pos = D3DXVECTOR3(0.0f, 10.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�̐���
//	m_Shadow = CShadow::Create(m_pos, D3DXVECTOR3(50.0f,0.0f,50.0f));

	return S_OK;
}

//===========================
//�I������
//===========================
void CObjectX::Uninit(void)
{
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
	CObject::Release();
}

//===========================
//�X�V����
//===========================
void CObjectX::Update(void)
{
	m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
	m_pos += m_move;		//�ʒu�̍X�V

//	m_Shadow->SetPos(m_pos);

	//�ړ��ʍX�V(����������)
	m_move.x += (0.0f - m_move.x)*MIN_SPEED;
	m_move.z += (0.0f - m_move.z)*MIN_SPEED;

}

//===========================
//�`�揈��
//===========================
void CObjectX::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;							//�}�e���A���̂Ȃ�
	D3DXMATRIX mtxParent;						//�����v�Z�p�̃}�g���b�N�X

	//���݂̃}�e���A�����ێ�
	pDevice->GetMaterial(&matDef);

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	////�e�̌����𔽉f
	//D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	////�e�̈ʒu�𔽉f	
	//D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	for (int nCnt = 0; nCnt < NUM_MODELPARTS; nCnt++)
	{
		//�p�[�c�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//�p�[�c�̃��f���̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�p�[�c�̃��f���̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		Shadow();

		//�e�̃}�g���N�X����
		mtxParent = m_mtxWorld;

		//���f���̃}�g���b�N�X�Ƃ̊|���Z�@�e�q�֌W�̊|���Z
		//D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		//�}�e���A���̕`��
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		for (int nCnt2 = 0; nCnt2 < (int)m_nNumMat; nCnt2++)
		{
			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCnt2].MatD3D);

			//�v���C���[�p�[�c�̕`��
			m_pMesh->DrawSubset(nCnt2);
		}
	}
	//�ێ����Ă����}�e���A����߂�
	pDevice->SetMaterial(&matDef);
}

//===========================
//����
//===========================
CObjectX * CObjectX::Create(D3DXVECTOR3 pos,int Priority, LPCTSTR name)
{
	CObjectX*pObjectX;
	pObjectX = new CObjectX(Priority);
	pObjectX->m_modelname = name;
	pObjectX->Init();
	pObjectX->SetPos(pos);

	return pObjectX;
}

//===========================
//�ʒu�ݒ�
//===========================
void CObjectX::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
//�����ݒ�
//===========================
void CObjectX::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
//���f���ǂݍ���
//===========================
void CObjectX::BindModel(LPD3DXMESH pMesh, LPD3DXBUFFER pBuff, DWORD pNumMat)
{
	m_pBuffMat = pBuff;
	m_pMesh = pMesh;
	m_nNumMat = pNumMat;
}

//===========================
//�e�̍쐬
//===========================
void CObjectX::Shadow()
{
	D3DXMATERIAL *pMat;							//�}�e���A���̂Ȃ�
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXCOLOR col[2];			//�F�Ɣ����̐ݒ�

	//�e��`�悷�鏈��
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;
	//�V���h�E�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxShadow);

	//���C�g�̋t�����Ō�̐��l�̓f�B���N�V���i�����C�g�ȊO�̏ꍇ�������炵���H
	D3DXVECTOR3 lightvec = CGame::GetLight()->GetVec(2);		//���C�g�̎擾
	vecLight = D3DXVECTOR4(-lightvec.x, -lightvec.y, -lightvec.z, 0.0f);

	pos = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);
	D3DXMatrixShadow(&m_mtxShadow, &vecLight, &planeField);

	//���f���̃}�g���b�N�X�Ƃ̊|���Z
	D3DXMatrixMultiply(&m_mtxShadow, &m_mtxShadow, &m_mtxWorld);

	//�e�̕`��
	//�}�e���A���̕`��
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	pDevice->SetTransform(D3DTS_WORLD, &m_mtxShadow);

	for (int i = 0; i < (int)m_nNumMat; i++)
	{
		col[0] = pMat[i].MatD3D.Diffuse;
		col[1] = pMat[i].MatD3D.Emissive;

		pMat[i].MatD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		pMat[i].MatD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//�v���C���[�p�[�c�̕`��
		m_pMesh->DrawSubset(i);

		pMat[i].MatD3D.Diffuse = col[0];
		pMat[i].MatD3D.Emissive = col[1];
	}
}