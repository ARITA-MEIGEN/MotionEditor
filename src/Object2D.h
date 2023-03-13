//=============================================================================
//
// 2D�I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================

#ifndef _OBJECT2D_H_
#define _OBJECT2D_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"

// ���_�t�H�[�}�b�g
const DWORD FVF_VERTEX_2D = (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CApplication;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************
// ���_�f�[�^
struct VERTEX_2D
{
	D3DXVECTOR3 pos;
	float rhw;
	D3DCOLOR col;
	D3DXVECTOR2 tex;	//�e�N�X�`�����W
};

class CObject2D : public CObject
{
public:
	//�����o�֐�
	explicit CObject2D(int nPriority=3);
	~CObject2D()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	void Setposition(D3DXVECTOR3 move);
	void BindTexture(LPDIRECT3DTEXTURE9 tex);
	static bool Hit(D3DXVECTOR3 pos, D3DXVECTOR3 TargetPos, D3DXVECTOR2 Siz, D3DXVECTOR2 TargetSiz);
	D3DXVECTOR3 GetPos();
	D3DXVECTOR2 GetSiz();
	D3DXCOLOR GetCol();
	float GetRot();
	D3DXVECTOR2 GetUV(int number);
	void SetPos(D3DXVECTOR3 pos);
	void SetPosR(float posR);
	void SetSiz(D3DXVECTOR2 Siz);
	void SetCol(D3DXCOLOR col);
	void SetRot(float Rot);
	void SetUV(float U1, float U2, float V1, float V2);
	static CObject2D*Create(D3DXVECTOR3 pos, D3DXVECTOR2 siz, int Priority);

private:
	//�����o�ϐ�
	LPDIRECT3DTEXTURE9 m_pTexture;		//�|���S���̃e�N�X�`��
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//�|���S���̒��_�o�b�t�@
	float m_fLength;					//�Ίp���̒���
	float m_fAngle;						//�Ίp���̊p�x
	float m_Rot;						//��]
	D3DXVECTOR3 m_Pos;					//�ʒu
	D3DXVECTOR2 m_Siz;					//�傫��
	D3DXCOLOR m_Col;					//�F
	D3DXVECTOR2 m_UV[2];				//�e�N�X�`�����W
	float m_nRControl;					//���W�̉E���̐���
};
#endif // !_OBJECT_H_

