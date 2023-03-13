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
	enum COLLISION
	{
		COLLI_AXIS = 0,		//���̔���
		COLLI_DAMAGE,		//�_���[�W����
		COLLI_SLOW,			//��������
		COLLI_HURT,			//���ꔻ��
		COLLI_HURTSLOW,		//�����̂��ꔻ��
	};

	//�U���̑���
	enum EDAMAGE_POINT
	{
		DP_HIGH = 0,		//��i
		DP_MIDDLE,			//���i
		DP_LOW,				//���i
		DP_MAX,
	};

	explicit CCollision(int nPriority = 3);
	~CCollision();

	//�v���g�^�C�v�錾
	HRESULT Init(void)override;
	void Uninit(void)override;
	void Update(void)override;
	void Draw(void)override;
	static CCollision* Create(D3DXVECTOR3 pos, COLLISION type);

	//�Q�b�^�[�ƃZ�b�^�[
	void SetSiz(D3DXVECTOR3 size) { m_width = size; };
	D3DXVECTOR3 GetSiz() { return m_width; };
	void SetPos(D3DXVECTOR3(pos)) { m_pos = pos; };
	D3DXVECTOR3 GetPos() { return m_pos; };
	void SwichUse() { bUse = !bUse; };						//�I���I�t�̐؂�ւ�
	void SetUse(bool use) { bUse = use; };					//�I���I�t�̐؂�ւ�
	void SetStartf(int start) { m_nStart = start; };		//�J�n�t���[��
	int GetStartf() { return m_nStart; };					//�J�n�t���[��
	void SetEndf(int end) { m_nEnd = end; };				//�I���t���[��
	int GetEndf() { return m_nEnd; };						//�I���t���[��
	void SetDPos(D3DXVECTOR3 pos) { m_defaultpos = pos; };	//�����ʒu
	void SetDRot(D3DXVECTOR3 rot) { m_defaultrot = rot; };	//��������
	D3DXVECTOR3 GetDPos() { return m_defaultpos; };			//�����ʒu
	D3DXVECTOR3 GetDRot() { return m_defaultrot; };			//��������
	void SetDMG(int dmg) { m_nDamage = dmg; };				//�_���[�W
	int GetDMG() { return m_nDamage; };						//�_���[�W
	void SetHitRig(int nRig) { m_nHitrig = nRig; };			//�q�b�g�d��
	int GetHitRig() { return m_nHitrig; };					//�q�b�g�d��
	void SetGuardRig(int nRig) { m_nGuardrig = nRig; };		//�K�[�h�d��
	int GetGuardRig() { return m_nGuardrig; };				//�K�[�h�d��
	void SetDp(EDAMAGE_POINT nDp) { m_nDp = nDp; };			//�_���[�W�����ݒ�
	EDAMAGE_POINT GetDp() { return m_nDp; };				//�_���[�W�����ݒ�
	void SetDown(bool down) { m_bDown = down; };			//�_�E������U�����ǂ����̐ݒ�
	bool GetDown() { return m_bDown; };						//�_�E������U�����ǂ����̐ݒ�

private:
	D3DXVECTOR3				m_pos;			//�ʒu
	D3DXVECTOR3				m_width;		//��
	D3DXVECTOR3				m_rot;			//����
	D3DXCOLOR				m_col;			//�F
	D3DXMATRIX				m_mtxWorld;		//���[���h�}�g���b�N�X
	bool					bUse;			//�g�p���Ă��邩�ǂ���
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;		//���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	m_pIdxBuff;		//�C���f�b�N�X�o�b�t�@
	COLLISION				m_Colltype;		//�����蔻��̎��
	int						m_nStart;		//�J�n�t���[��
	int						m_nEnd;			//�I���t���[��
	D3DXVECTOR3				m_defaultrot;	//����
	D3DXVECTOR3				m_defaultpos;	//�ʒu
	int						m_nDamage;		//�_���[�W��
	int						m_nHitrig;		//�q�b�g�d��
	int						m_nGuardrig;	//�K�[�h�d��
	EDAMAGE_POINT			m_nDp;			//�_���[�W����
	bool					m_bDown;		//�_�E��������U�����ǂ���
};

#endif



