//=================================================
// Content     (�Q�[���̐ݒ�)(Line.h)
// Author     : �L�c����
//=================================================
#ifndef _LINE_H_		//���̃}�N����`������ĂȂ�������
#define _LINE_H_		//��d�C���N���[�h�h�~�̃}�N����`
#include"main.h"
#include"Object.h"

//���f���̍\����
class CCollision :public CObject
{
public:
	explicit CCollision(int nPriority = 3);
	~CCollision();

	//�v���g�^�C�v�錾
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CCollision* Create(D3DXVECTOR3 pos);
	void SetSiz(D3DXVECTOR3 size) { m_width = size; };
	D3DXVECTOR3 GetSiz() {return m_width; };
	void SetPos(D3DXVECTOR3(pos)) { m_pos = pos; };
	D3DXVECTOR3 GetPos( ) { return m_pos; };
	void SwichUse() { bUse = !bUse; };	//�I���I�t�̐؂�ւ�
	void SetUse(bool use) { bUse = use; };	//�I���I�t�̐؂�ւ�
	
private:
	D3DXVECTOR3 m_pos;			//�ʒu
	D3DXVECTOR3 m_width;		//��
	D3DXVECTOR3 m_rot;			//����
	D3DXCOLOR	m_col;			//�F
	D3DXMATRIX m_mtxWorld;		//���[���h�}�g���b�N�X
	bool bUse;					//�g�p���Ă��邩�ǂ���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@

};

#endif



