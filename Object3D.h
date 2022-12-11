//=============================================================================
//
// 3D�I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================

#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"

// ���_�t�H�[�}�b�g
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);
//*****************************************************************************
//�O���錾
//*****************************************************************************
class CApplication;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�f�[�^
struct VERTEX_3D
{
	D3DXVECTOR3 pos;	//���_���W
	D3DXVECTOR3 nor;	//�@���x�N�g��
	D3DCOLOR col;		//���_�J���[
	D3DXVECTOR2 tex;	//�e�N�X�`�����W;
};

class CObject3D : public CObject
{
public:
	//�����o�֐�
	explicit CObject3D(int nPriority = 3);
	~CObject3D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void SetPos(D3DXVECTOR3 pos);
	void SetSiz(D3DXVECTOR3 siz);
	void SetCol(D3DXCOLOR col);
	static CObject3D*Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz, int Priority);
	void BindTexture(LPDIRECT3DTEXTURE9 tex);

private:
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9 m_pTexture;		//�|���S���̃e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//�|���S���̒��_�o�b�t�@
	float m_fLength;					//�Ίp���̒���
	float m_fAngle;						//�Ίp���̊p�x
	D3DXVECTOR3 m_Rot;						//��]
	D3DXMATRIX m_mtxWorldPolygon;
	D3DXVECTOR3 m_Pos;					//�ʒu
	D3DXVECTOR3 m_Siz;					//�傫��
	D3DXCOLOR m_Col;					//�F
};
#endif // !_OBJECT_H_


