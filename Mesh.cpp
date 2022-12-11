//=================================================
// Content     (�Q�[���̐ݒ�)(.cpp)
// Author     : �L�c����1
//=================================================

//�C���N���[�h
#include"mesh.h"
#include"Application.h"
#include"renderer.h"
#include"Object3D.h"

//===========================
//�R���X�g���N�^
//===========================
CMesh::CMesh(int nPriorty):CObject(nPriorty)
{

}

//===========================
//�f�X�g���N�^
//===========================
CMesh::~CMesh()
{

}

//===========================
//����������
//===========================
HRESULT CMesh::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	////�e�N�X�`���̓ǂݍ��ݏ���
	//D3DXCreateTextureFromFile(pDevice,
	//	"data\\TEXTURE\\block000.jpg",
	//	&m_pTexture);

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) *MESHFIELD_VERTEX_NUM,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//�C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD)*MESHFIELD_INDEX_NUM,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	VERTEX_3D*pVtx = NULL;

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���S�_
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�e�N�X�`�����W�̐ݒ�
	int nCnt = 0;
	int nCnt2 = 0;
	for (nCnt = 0; nCnt < m_Zpolyogn + 1; nCnt++)
	{
		for (nCnt2 = 0; nCnt2 < m_Xpolyogn + 1; nCnt2++)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(POLIGON_SIZE*nCnt2,/*rand()%20*/sinf(nCnt*0.62f)*10.0f, -POLIGON_SIZE*nCnt);

			//�@���x�N�g���̐ݒ�
			pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//���_�J���[�ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//�e�N�X�`���ݒ�
			pVtx[0].tex = D3DXVECTOR2(5.0f*nCnt2, 5.0f*nCnt);

			pVtx++;
		}
	}

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	//�C���f�b�N�X�o�b�t�@�̃��b�N
	WORD*pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//�C���f�b�N�X�̐ݒ�
	for (nCnt2 = 0; nCnt2 < m_Zpolyogn; nCnt2++)
	{
		if (nCnt2 > 0)
		{//�k�ރ|���S��
			nCnt = 0;
			pIdx[0] = (WORD)(nCnt + (m_Xpolyogn + 1) + (m_Xpolyogn + 1)*nCnt2);
			pIdx++;
		}

		for (nCnt = 0; nCnt < m_Xpolyogn + 1; nCnt++)
		{
			pIdx[0] = (WORD)(nCnt + (m_Xpolyogn + 1) + (m_Xpolyogn + 1)*nCnt2);
			pIdx[1] = (WORD)(nCnt + (m_Xpolyogn + 1)*nCnt2);
			pIdx += 2;
		}

		if (nCnt2 < m_Zpolyogn)
		{//�k�ރ|���S��
			pIdx[0] = (WORD)(nCnt - 1 + (m_Xpolyogn + 1)*nCnt2);
			pIdx++;
		}
	}

	/*pIdx[0] = 3;
	pIdx[1] = 0;
	pIdx[2] = 4;
	pIdx[3] = 1;
	pIdx[4] = 5;
	pIdx[5] = 2;
	pIdx[6] = 2;
	pIdx[7] = 6;
	pIdx[8] = 6;
	pIdx[9] = 3;
	pIdx[10] = 7;
	pIdx[11] = 4;
	pIdx[12] = 8;
	pIdx[13] = 5;*/

	//�C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
	return S_OK;
}

//===========================
//�I������
//===========================
void CMesh::Uninit(void)
{
	//���_�o�b�t�@��j��
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//�e�N�X�`����j��
	if (m_pTexture != NULL)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
	//�C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	CObject::Release();
}
//===========================
//�X�V����
//===========================
void  CMesh::Update(void)
{

}

//===========================
//�`�揈��
//===========================
void CMesh::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
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

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//���b�V���t�B�[���h�̕`��       
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,				//���b�V���t�B�[���h�̌`
		0, 0, MESHFIELD_VERTEX_NUM, 0, MESHFIELD_PRIMITIVE_NUM);	//���_�̊J�n�ꏊ

	//�e�N�X�`���̉���
	pDevice->SetTexture(0, NULL);
}

//===========================
//����
//===========================
CMesh * CMesh::Create(int x,int z)
{
	CMesh*pMesh;
	pMesh = new CMesh(CObject::OBJTYPE_MAP);
	pMesh->m_Xpolyogn = x;
	pMesh->m_Zpolyogn = z;
	pMesh->Init();

	return pMesh;
}

//===========================
//����
//===========================
float CMesh::Collision(D3DXVECTOR3 pos)
{
	VERTEX_3D*pVtx = NULL;
	D3DXVECTOR3 V1, V2, VecA, VecB, nor, P1, P2, P3;
	D3DXVECTOR3	aNor[2560] = {};
	memset(&aNor[0], 0, sizeof(aNor));
	float fVec[3];
	bool bReturn = false;
	float nSave;

	// �C���f�b�N�X�o�b�t�@�̃��b�N
	WORD*pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int max = MESHFIELD_INDEX_NUM - 2;
	//XZ�̊O�ςŔ���
	for (int i = 0; i < max; i++)
	{
		P1 = pVtx[pIdx[i]].pos;
		P2 = pVtx[pIdx[i + 1]].pos;
		P3 = pVtx[pIdx[i + 2]].pos;

		if (P1 != P2 && P2 != P3 &&P1 != P3)
		{
			VecA = P2 - P1;
			VecB = pos - P1;

			fVec[0] = (VecA.x*VecB.z - VecA.z*VecB.x);

			VecA = P3 - P2;
			VecB = pos - P2;

			fVec[1] = (VecA.x*VecB.z - VecA.z*VecB.x);

			VecA = P1 - P3;
			VecB = pos - P3;

			fVec[2] = (VecA.x*VecB.z - VecA.z*VecB.x);

			//�O�ς̌���
			V1 = P2 - P1;
			V2 = P3 - P1;

			//�ʖ@���x�N�g���̍쐬
			D3DXVec3Cross(&nor, &V1, &V2);
			D3DXVec3Normalize(&nor, &nor);
			VecA = pos - (P1);
			VecB = nor;

			//�ʖ@���x�N�g���̉��Z
			if (i % 2 == 0)
			{
				aNor[pIdx[i]] += nor;
				aNor[pIdx[i + 1]] += nor;
				aNor[pIdx[i + 2]] += nor;
			}
			else
			{
				aNor[pIdx[i]] -= nor;
				aNor[pIdx[i + 1]] -= nor;
				aNor[pIdx[i + 2]] -= nor;
			}

			for (int j = 0; j < 3; j++)
			{//���K��
				D3DXVec3Normalize(&aNor[pIdx[i + j]], &aNor[pIdx[i + j]]);

				pVtx[pIdx[i + j]].nor = aNor[pIdx[i + j]];
			}

			if (bReturn==false&&(fVec[0] > 0 && fVec[1] > 0 && fVec[2] > 0) || (fVec[0] < 0 && fVec[1] < 0 && fVec[2] < 0))
			{
				//���ς̌���
				nSave = ((P1.y) - ((pos.x - (P1.x))*VecB.x + (pos.z - (P1.z))*VecB.z) / VecB.y);
				bReturn = true;
			}
		}
	}
	//�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();
	m_pVtxBuff->Unlock();

	
	if (bReturn == true)
	{//�|���S���̒��ɂ����ꍇ
		return nSave;
	}
	return pos.y;
}