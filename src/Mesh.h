//=================================================
// Content     (�Q�[���̐ݒ�)(.h)
// Author     : �L�c����
//=================================================
#ifndef _MESH_H_	//���̃}�N����`������ĂȂ�������
#define _MESH_H_	//��d�C���N���[�h�h�~�̃}�N����`
#include"main.h"
#include"Object.h"

//�}�N����`
#define POLIGON_SIZE				(20.0f)																	//���b�V���t�B�[���h�̑傫��
#define	MESHFIELD_VERTEX_NUM		((m_Xpolyogn+1)*(m_Zpolyogn+1))											//���_�o�b�t�@�̐�
#define MESHFIELD_INDEX_NUM			((m_Xpolyogn+1)*2*m_Zpolyogn+(m_Zpolyogn-1)*((m_Zpolyogn-1)*2))			//�C���f�b�N�X�o�b�t�@�̐�
#define MESHFIELD_PRIMITIVE_NUM		(m_Xpolyogn*m_Zpolyogn*2+(4*(m_Zpolyogn-1)))							//�|���S���̐�

class CMesh:CObject
{
public:
	explicit CMesh(int nPriority = 3);
	~CMesh();
	//�v���g�^�C�v�錾
	HRESULT			Init(void)override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	static CMesh*	Create(int x, int z, D3DXVECTOR3 pos);
	float			Collision(D3DXVECTOR3 pos);			//�Փ˔���

private:
	D3DXVECTOR3					m_pos;
	D3DXVECTOR3					m_rot;
	D3DXMATRIX					m_mtxWorld;
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;				//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;				//�C���f�b�N�X�o�b�t�@
	LPDIRECT3DTEXTURE9			m_pTexture;				//�e�N�X�`���ւ̃|�C���^
	int							m_Xpolyogn;				//X�����̃|���S���̐�
	int							m_Zpolyogn;				//X�����̃|���S���̐�
};

#endif


