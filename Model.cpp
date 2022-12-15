//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
#include"Model.h"
#include"Game.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Light.h"

//�}�N����`
#define PLAYER_SPEED	(2.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x
#define NUM_MODELPARTS	(1)		//���f���̃p�[�c��

//�ÓI�����o�ϐ��錾

//===========================
//�R���X�g���N�^
//===========================
CModel::CModel() 
{
	m_col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
}

//===========================
//�f�X�g���N�^
//===========================
CModel::~CModel()
{

}

//===========================
//����������
//===========================
HRESULT CModel::Init()
{
	return S_OK;
}

//===========================
//�I������
//===========================
void CModel::Uninit(void)
{

}

//===========================
//�X�V����
//===========================
void CModel::Update(void)
{
	//�p�x�̐��K��
	if (m_rot.x >= D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2;
	}
	else if (m_rot.x <= -D3DX_PI)
	{
		m_rot.x+= D3DX_PI * 2;
	}
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.z >= D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2;
	}
	else if (m_rot.z <= -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2;
	}
}

//===========================
//�`�揈��
//===========================
void CModel::Draw(D3DXMATRIX pMtx)
{
	if (m_pMesh != nullptr)
	{
		LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
		pDevice = CApplication::GetRenderer()->GetDevice();

		D3DXMATRIX mtxRot, mtxTrans;				//�v�Z�p�}�g���b�N�X
		D3DMATERIAL9 matDef;						//���݂̃}�e���A���ۑ��p
		D3DXMATERIAL *pMat;							//�}�e���A���̏��
		m_mtxParent = pMtx;

		//���݂̃}�e���A�����ێ�
		pDevice->GetMaterial(&matDef);

		//�p�[�c�̃��[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//�p�[�c�̃��f���̌����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�p�[�c�̃��f���̈ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//�e�̃}�g���b�N�X�Ɗ|�����킹��
		if (m_pParent != nullptr)
		{
			m_mtxParent = m_pParent->GetMtx();
		}
		else
		{//����(�ŐV)��Mtx���擾(Player��Mtx)�e�̂��Ȃ����f������������ꍇ�͕ʑΉ����K�v
			pDevice->GetTransform(D3DTS_WORLD, &m_mtxParent);
		}

		//�e�̍��W�Ƃ������킹��
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxParent);

		//�e�̐���
		//Shadow()

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

		//�}�e���A���̕`��
		for (int nCnt2 = 0; nCnt2 < (int)m_nNumMat; nCnt2++)
		{
			pMat[nCnt2].MatD3D.Diffuse = m_col;
			pMat[nCnt2].MatD3D.Emissive = m_Emi;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&pMat[nCnt2].MatD3D);

			//�v���C���[�p�[�c�̕`��
			m_pMesh->DrawSubset(nCnt2);
		}
		//�ێ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}
}

//===========================
//����
//===========================
CModel * CModel::Create()
{
	CModel*pModel;
	pModel = new CModel();
	pModel->Init();
	pModel->m_nIdx = 0;
	return pModel;
}

//===========================
//�e�̍쐬
//===========================
void CModel::Shadow()
{
	D3DXMATERIAL *pMat;			//�}�e���A���̂Ȃ�
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
		col[0] = pMat[i].MatD3D.Diffuse;			//�F�̕ۑ�
		col[1] = pMat[i].MatD3D.Emissive;			//�����̕ۑ�

		pMat[i].MatD3D.Diffuse = m_col;
		pMat[i].MatD3D.Emissive = m_Emi;
		//�}�e���A���̐ݒ�
		pDevice->SetMaterial(&pMat[i].MatD3D);

		//�v���C���[�p�[�c�̕`��
		m_pMesh->DrawSubset(i);

		pMat[i].MatD3D.Diffuse = col[0];
		pMat[i].MatD3D.Emissive = col[1];
	}
}

//===========================
//�ʒu�̎擾
//===========================
void CModel::Siz()
{
	BYTE*pVtxBuff;		//���_�o�b�t�@�ւ̃|�C���^
	int nNumVtx;		//���_��
	DWORD sizeFVF;		//���_�t�H�[�}�b�g�̃T�C�Y

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

	m_vtxMax = vtxMax;							//���_���W�̍ő�l
	m_vtxMin = vtxMin;							//���_���W�̍ŏ��l
}

//===========================
//�e���f���̐ݒ�
//===========================
void CModel::SetParent(CModel * pModel)
{
	m_pParent = pModel;
}

//===========================
//�e���f���̎擾
//===========================
CModel * CModel::GetParent()
{
	return m_pParent;
}

//===========================
//�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CModel::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//�����̐ݒ�
//===========================
void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
}

//===========================
//�����̉��Z
//===========================
D3DXVECTOR3 CModel::GetRot()
{
	return m_rot;
}

//===========================
//�ʒu�̐ݒ�
//===========================
void CModel::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//===========================
//�ʒu�̎擾
//===========================
D3DXVECTOR3 CModel::GetPos()
{
	return m_pos;
}

//===========================
//X�̕��̎擾
//===========================
float CModel::GetWidth()
{
	 return m_vtxMax.x - m_vtxMin.x ;
}

//===========================
//�F�̐ݒ�
//===========================
void CModel::SetCol(D3DXCOLOR col)
{
	m_col = col;
}

void CModel::SetEmi(D3DXCOLOR col)
{
	m_Emi = col;
}

//===========================
//���f���̐ݒ�
//===========================
void CModel::SetModel(char* Filename)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXLoadMeshFromX(Filename,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
}
