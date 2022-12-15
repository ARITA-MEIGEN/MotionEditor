//=================================================
// Content     (�Q�[���̐ݒ�)(Line.h)
// Author     : �L�c����
//=================================================
#ifndef _LINE_H_		//���̃}�N����`������ĂȂ�������
#define _LINE_H_		//��d�C���N���[�h�h�~�̃}�N����`
#include"main.h"
#include"Object.h"

//���f���̍\����
class CLine :public CObject
{
public:
	explicit CLine(int nPriority = 3);
	~CLine();

	//�v���g�^�C�v�錾
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CLine* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void SetAll(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 max, D3DXVECTOR3 min);

private:
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_width;		//��
	D3DXVECTOR3 m_rot;			//����
	D3DXCOLOR	m_col;			//�F
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩�ǂ���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@

};

#endif


