//=============================================================================
//
// X�t�@�C���ǂݍ���
// Author : �L�c����
//
//=============================================================================

#ifndef _MODEL_H_
#define _MODEL_H_

//�C���N���[�h
#include"main.h"
#include"ObjectX.h"

//�}�N����`
#define PLAYER_LEGPARTS	(13)
#define	NUM_PLAYERPARTS	(1+PLAYER_LEGPARTS)

//�O���錾
class CShadow;

class CModel
{
public:
	explicit CModel();
	~CModel();
	//�v���g�^�C�v�錾
	HRESULT		Init();
	void		Uninit(void);
	void		Update(void);
	void		Draw(D3DXMATRIX pMtx);
	static		CModel*Create();
	void		Shadow();						//�e(�g��Ȃ�)
	void		Siz();							//�T�C�Y�ݒ�
	void		SetModel(char* Filename);		//���f���̐ݒ�
	void		SetParent(CModel*pModel) ;
	CModel*		GetParent();
	D3DXMATRIX	GetMtx();
	void		SetRot(D3DXVECTOR3 rot);	
	D3DXVECTOR3 GetRot();
	void		SetPos(D3DXVECTOR3 pos);
	D3DXVECTOR3	GetPos();
	float		GetWidth();
	void		SetIdx(int Idx) { m_nIdx = Idx; };
	int			GetIdx() { return m_nIdx; };
	void		SetDPos(D3DXVECTOR3 pos) { m_DefaultPos = pos; };	//�����ʒu��ݒ�
	D3DXVECTOR3	GetDPos() { return m_DefaultPos; };					//�����ʒu���擾
	void		SetDRot(D3DXVECTOR3 rot) { m_DefaultRot = rot; };	//�����p�x��ݒ�
	D3DXVECTOR3	GetDRot() { return m_DefaultRot; };					//�����p�x���擾
	void		SetCol(D3DXCOLOR col);
	void		SetEmi(D3DXCOLOR col);								//�����̐ݒ�
	void		SetDP(int dp) { dp = m_nDamagePoint; };				//�_���[�W������ݒ�
	int			GetDP() { return m_nDamagePoint; };					//�_���[�W������ݒ�




private:
	LPD3DXBUFFER		m_pBuffMat;		//�}�e���A�����ւ̃|�C���^
	DWORD				m_nNumMat;		//�}�e���A�����̐�
	LPD3DXMESH			m_pMesh;		//���b�V���ւ̃|�C���^
	D3DXMATRIX			m_mtxParent;	//�e�̃��[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxWorld;		//�q�̃��[���h�}�g���b�N�X
	D3DXMATRIX			m_mtxShadow;	//�e�p���[���h�}�g���b�N�X
	D3DXVECTOR3			m_pos;			//�ʒu
	D3DXVECTOR3			m_posold;		//�O��̈ʒu
	D3DXVECTOR3			m_rotDest;		//�ړI�̊p�x�̕ۑ�
	D3DXVECTOR3			m_rot;			//����
	D3DXVECTOR3			m_vtxMax;		//���f���̍ő���W
	D3DXVECTOR3			m_vtxMin;		//���f���̍��W�̍ŏ��l
	int					m_nIdx;			//���g�̔ԍ�
	CModel*				m_pParent;		//�e�̃��f���ւ̃|�C���^
	D3DXVECTOR3			m_DefaultPos;	//�����ʒu
	D3DXVECTOR3			m_DefaultRot;	//�����ʒu
	D3DXCOLOR			m_col;
	D3DXCOLOR			m_Emi;

	D3DXVECTOR3			m_TempPos;						//�ҏW�p�̉����W
	D3DXVECTOR3			m_TempRot;						//�ҏW�p�̉����W
	int					m_nDamagePoint;					//�����蔻��̑���


};

#endif

