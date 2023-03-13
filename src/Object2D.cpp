//=============================================================================
//
// 2D�I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include "Object2D.h"
#include "renderer.h"
#include"Application.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CObject2D::CObject2D(int nPriority) :CObject(nPriority)
{
	m_pTexture = nullptr;											//�|���S���̃e�N�X�`��
	m_pVtxBuff = nullptr;											//�|���S���̒��_�o�b�t�@
	m_fLength = 0.0f;												//�Ίp���̒���
	m_fAngle = 0.0f;												//�Ίp���̊p�x
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//�ʒu������������
	m_Rot = 0.0f;													//������������
	m_Col= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_UV[0] = D3DXVECTOR2(0.0f, 0.0f);
	m_UV[1] = D3DXVECTOR2(1.0f, 1.0f);
	m_nRControl = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject2D::~CObject2D()
{
	assert(m_pVtxBuff == nullptr);		//�|���S���̒��_�o�b�t�@
}

//=============================================================================
// ������
//=============================================================================
HRESULT  CObject2D::Init()
{
	LPDIRECT3DDEVICE9 pDevice;
	VERTEX_2D*pVtx;
	//�f�o�C�X�̎擾
	pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr);

	//�Ίp���̒������Z�o����
	m_fLength = sqrtf((m_Siz.y*m_Siz.y) + (m_Siz.x*m_Siz.x)) / 2.0f;

	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(m_Siz.x, m_Siz.y);//�G�̒e�Ƃ��Ďg���ꍇ��()�̒���G�ƃv���C���[�̊Ԃ̋����ɂ���

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos.x = m_Pos.x + sinf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.y = m_Pos.y + cosf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x =m_Pos.x + sinf(m_Rot + D3DX_PI -m_fAngle)*m_fLength;
	pVtx[1].pos.y =m_Pos.y + cosf(m_Rot + D3DX_PI -m_fAngle)*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.y = m_Pos.y + cosf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x =m_Pos.x + sinf(m_Rot +m_fAngle)*m_fLength;
	pVtx[3].pos.y =m_Pos.y + cosf(m_Rot +m_fAngle)*m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);	
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	return S_OK;
}

//=============================================================================
//�I��
//=============================================================================
void  CObject2D::Uninit()
{
	//���_�o�b�t�@��j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	CObject::Release();
}

//=============================================================================
// �X�V
//=============================================================================
void  CObject2D::Update()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	VERTEX_2D*pVtx;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
 	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_����ݒ�
	pVtx[0].pos.x = m_Pos.x + sinf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.y = m_Pos.y + cosf(m_Rot - D3DX_PI + m_fAngle)*m_fLength;
	pVtx[0].pos.z = 0.0f;

	pVtx[1].pos.x = m_Pos.x + sinf(m_Rot + D3DX_PI - m_fAngle)*m_fLength+m_nRControl;
	pVtx[1].pos.y = m_Pos.y + cosf(m_Rot + D3DX_PI - m_fAngle)*m_fLength;
	pVtx[1].pos.z = 0.0f;

	pVtx[2].pos.x = m_Pos.x + sinf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.y = m_Pos.y + cosf(m_Rot - m_fAngle)*m_fLength;
	pVtx[2].pos.z = 0.0f;

	pVtx[3].pos.x = m_Pos.x + sinf(m_Rot + m_fAngle)*m_fLength + m_nRControl;
	pVtx[3].pos.y = m_Pos.y + cosf(m_Rot + m_fAngle)*m_fLength;
	pVtx[3].pos.z = 0.0f;

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[�̐ݒ�
	pVtx[0].col = m_Col;
	pVtx[1].col = m_Col;
	pVtx[2].col = m_Col;
	pVtx[3].col = m_Col;

	//�e�N�X�`���̐ݒ�
	/*pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);*/

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// �`��
//=============================================================================
void  CObject2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = CApplication::GetRenderer()->GetDevice();

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	//�e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	
	//�|���S���̕`��       
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,	//�|���S���̌`
		0,										//���_�̊J�n�ꏊ
		2);										//�v���~�e�B�u�̐�
}
//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void  CObject2D::Setposition(D3DXVECTOR3 move)
{
	m_Pos += move;
}

//=============================================================================
// �e�N�X�`���̐ݒ�
//=============================================================================
void CObject2D::BindTexture(LPDIRECT3DTEXTURE9 tex)
{
	m_pTexture = tex;
}

//=============================================================================
// �����蔻��
//=============================================================================
bool CObject2D::Hit(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, D3DXVECTOR2 Siz, D3DXVECTOR2 TargetSiz)
{
	//�����蔻��̎�
	if (pos.x - Siz.x / 2 <= TargetPos.x + TargetSiz.x/2
		&& pos.x + Siz.x / 2 >= TargetPos.x - TargetSiz.x / 2
		&& pos.y - Siz.y / 2 <= TargetPos.y + TargetSiz.y / 2
		&& pos.y + Siz.y / 2 >= TargetPos.y - TargetSiz.y / 2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================================
// �ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CObject2D::GetPos()
{
	return m_Pos;
}

//=============================================================================
// �T�C�Y�̎擾
//=============================================================================
D3DXVECTOR2 CObject2D::GetSiz()
{
	return m_Siz;
}

//=============================================================================
//�F�̎擾
//=============================================================================
D3DXCOLOR CObject2D::GetCol()
{
	return m_Col;
}

//=============================================================================
//�p�x�̎擾
//=============================================================================
float CObject2D::GetRot()
{
	return m_Rot;
}

//=============================================================================
//UV���W�̎擾
//=============================================================================
D3DXVECTOR2 CObject2D::GetUV(int number)
{
	return m_UV[number];
}

//=============================================================================
// �ʒu�̐ݒ�
//=============================================================================
void CObject2D::SetPos(D3DXVECTOR3 pos)
{
	m_Pos = pos;
}

//=============================================================================
// �ʒu�̏ڍאݒ�
//=============================================================================
void CObject2D::SetPosR(float PosR)
{
	m_nRControl = PosR;					//���W�̉E���̐���
}

//=============================================================================
// �T�C�Y�̐ݒ�
//=============================================================================
void CObject2D::SetSiz(D3DXVECTOR2 Siz)
{
	m_Siz = Siz;

	m_fLength = sqrtf((m_Siz.y*m_Siz.y) + (m_Siz.x*m_Siz.x)) / 2.0f;

	//�Ίp���̊p�x���Z�o����
	m_fAngle = atan2f(m_Siz.x, m_Siz.y);
}

//=============================================================================
// �F�̐ݒ�
//=============================================================================
void CObject2D::SetCol(D3DXCOLOR col)
{
	m_Col = col;
}

//=============================================================================
// �p�x�̐ݒ�
//=============================================================================
void CObject2D::SetRot(float Rot)
{
	m_Rot = Rot;
}

//=============================================================================
// UV���W�̐ݒ�
//=============================================================================
void CObject2D::SetUV(float U1, float U2, float V1, float V2)
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();
	VERTEX_2D*pVtx;

	m_UV[0] = D3DXVECTOR2(U1, V1);
	m_UV[1] = D3DXVECTOR2(U2, V2);

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//�e�N�X�`���̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(m_UV[0].x, m_UV[0].y);
	pVtx[1].tex = D3DXVECTOR2(m_UV[1].x, m_UV[0].y);
	pVtx[2].tex = D3DXVECTOR2(m_UV[0].x, m_UV[1].y);
	pVtx[3].tex = D3DXVECTOR2(m_UV[1].x, m_UV[1].y);

	//���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ����
//=============================================================================
CObject2D * CObject2D::Create(D3DXVECTOR3 pos, D3DXVECTOR2 siz,int Priority)
{
	CObject2D*pObject2D;
	pObject2D = new CObject2D(Priority);
	pObject2D->SetSiz(siz);
	pObject2D->Init();
	pObject2D->SetPos(pos);
	return pObject2D;
}
