//=============================================================================
//
// X�t�@�C���ǂݍ���
// Author : �L�c����
//
//=============================================================================

#ifndef _PLAYER_H_
#define _PLAYER_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"
#include"ObjectX.h"
#include"Model.h"

//�O���錾
class CShadow;
class CModel;
class CCollision;

//�}�N����`
#define MAX_MOTION		(20)	//���[�V�����̐�
#define MAX_KEY			(60)	//�L�[�̑���
#define MAX_FRAME		(120)	//�t���[���̍ő吔
#define NUM_PARTS		(14)	//�p�[�c�̐�
#define HOLD_TIME		(30)	//���������ď������ʂ�܂ł̎���
#define SINGLE_STA		(0.01f)	//�P�̂œ�������
#define HOLD_STA		(0.05f)	//�������œ�������
#define MAX_COLLISION	(20)	//1�̃p�[�c�ɂ��ݒ�ł��锻��̐�
#define	MAX_POS			(50.0f)	//imgui�œ�������ő吔


class CPlayer :public CObject
{
public:
	//�L�[�v�f
	struct KEY
	{
		float fPosX;
		float fPosY;
		float fPosZ;

		float fRotX;
		float fRotY;
		float fRotZ;

		D3DXVECTOR3 fTempPos;
		D3DXVECTOR3 fTempRot;

	};

	//�L�[���
	struct KEY_SET 
	{
		KEY aKey[NUM_PARTS];					//���f���̐������W������
		int nFrame;								//�Đ�����
		CCollision* Collision[MAX_COLLISION];	//�����蔻��
		int nNumCollision;						//1�̃L�[�̓����蔻��̐�
		CCollision* HurtCol[MAX_COLLISION];		//���ꔻ��
		int nNumHurtCol;						//1�̃L�[�̓����蔻��̐�
	};

	struct MOTION_SET
	{
		KEY_SET	aModelKey[MAX_KEY];		//�L�[�̑���������
		int		nNumKey;				//�L�[�̑���(�t�@�C���œǂݍ���)
		bool	bLoop;					//���[�v���邩�ǂ���
		bool	bControl;				//����\�ȃ��[�V�������ǂ���
		int		nCancelFrame;			//�Z�̃L�����Z���P�\
		bool	bAtk;					//�U�����[�V�������ǂ���(�Đ���Ƀj���[�g�����ɖ߂����ǂ���)
		int		nHitStopTimer;			//�q�b�g�X�g�b�v�̎���

	};

	enum PLAYER_MOTION
	{
		//�n��
		PM_ST_NEUTRAL,		//�j���[�g����
		PM_ST_MOVE,			//�ړ�(���Ⴊ�݂�������)
		PM_ST_DASH,			//�O�_�b�V��
		PM_ST_BACKSTEP,		//�o�b�N�X�e�b�v
		PM_ST_GUARD,		//�K�[�h
		PM_ST_HIT,			//��e
		PM_ST_LATTACK,		//��U��
		PM_ST_MATTACK,		//���U��
		PM_ST_HATTACK,		//���U��
		PM_ST_HURT,			//���ꃂ�[�V����

		//��
		PM_JP_NEUTRAL,		//�����W�����v
		PM_JP_MOVELEFT	,	//�ړ�(���Ⴊ�݂�������)
		PM_JP_MOVERIGHT,	//�ړ�(���Ⴊ�݂�������)
		PM_JP_DASH,			//�O�_�b�V��
		PM_JP_GUARD,		//�K�[�h
		PM_JP_HIT,			//��e
		PM_JP_LATTACK,		//��U��
		PM_JP_MATTACK,		//���U��
		PM_JP_HATTACK,		//���U��
		PM_JP_HURT,			//���ꃂ�[�V����

		//���Ⴊ��
		PM_CR_NEUTRAL,		//�j���[�g����
		PM_CR_MOVE,			//�ړ�(���Ⴊ�݂�������)
		PM_CR_GUARD,		//�K�[�h
		PM_CR_HIT,			//��e
		PM_CR_LATTACK,		//��U��
		PM_CR_MATTACK,		//���U��
		PM_CR_HATTACK,		//���U��
		PM_CR_HURT,			//���ꃂ�[�V����

		//�K�E�Z
		PM_236L,			//��g��
		PM_236M,			//���g��
		PM_236H,			//���g��

		PM_214L,			//�㗳��
		PM_214M,			//������
		PM_214H,			//������

		PM_623L,			//�㏸��
		PM_623M,			//������
		PM_623H,			//������

		PM_ST_DIE,			//��
		PM_CR_DIE,			//��
		PM_JP_DIE,			//��

		PM_DOWN,			//���S&�_�E�������[�V����
		PM_STANDUP,			//�N���オ��
		PM_FORWARDSLOW,		//�O����
		PM_BACKSLOW,		//��듊��
		PM_MAX
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//�v���g�^�C�v�錾
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	bool			ControlPlayer(void);			//�v���C���[�̑���
	static CPlayer*	Create();						//�v���C���[����
	D3DXMATRIX		GetMtx();						//�}�g���b�N�X�̎擾
	void			ReadMotion();					//���[�V�����ǂݍ���
	void			WriteMotion(int nowmotion);		//���[�V���������o��
	void			rolling();						//��]�̃e�X�g
	void			MotionManager();				//��Ԃɍ��킹�ă��[�V�����Đ�����
	void			PlayFirstMotion();				//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��
	void			EditMode();						//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��
	void			Normalize(int number);			//�p�x�̐��K��
	void			PlayerStateDraw();				//�v���C���[�̏��̕ҏW�ƕ\��(IMGUI)
	void			DrawCollision();				//�v���C���[�̏��̕ҏW�ƕ\��(IMGUI)
	void			SetFrame();						//�t���[�������ă��[�V�����̈ʒu��\������
	void			SaveAxis();						//���̔����ۑ�
	void			SaveCollision();				//�����蔻��̕ۑ�
	void			SetHitBox();					//���ꔻ��̐ݒ�


private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//���f���̃C���X�^���X
	MOTION_SET		m_apMotion[PM_MAX];			//���[�V�����̐������������[�V�����̐�->�L�[�̑���->���f���̐�
	D3DXMATRIX		m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;							//�ʒu
	D3DXVECTOR3		m_rot;							//����
	D3DXVECTOR3		m_move;							//�ړ���
	D3DXVECTOR3		m_posold;						//�O��̈ʒu
	//CShadow*		m_Shadow;						//�e
	D3DXVECTOR3		m_rotDest;						//�ړI�̊p�x�̕ۑ�
	int				m_nNumKey;						//�L�[�̑���
	D3DXMATRIX		m_mtxRot;						//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION	m_quat;							//�N�H�[�^�j�I��
	D3DXVECTOR3		m_vecAxis;						//��]��
	float			m_fRolling;						//��]�ʁ@(��]�p)
	int				m_nNumModel;
	char			m_nModelpass[255];
	D3DXVECTOR3		m_movepos;
	D3DXVECTOR3		m_moverot;
	PLAYER_MOTION	m_pMotion;						//���[�V����
	PLAYER_MOTION	m_pMotionOld;					//�ЂƂO�̃��[�V����
	int				m_nowmotion;					//���݂̃��[�V����
	int				m_nKEYData;						//���݃f�[�^��\������p�[�c
	bool			m_bPlay;						//�Đ����[�h
	int				m_nEditModel;					//�ҏW���郂�f��
	int				m_nHold;						//����������
	int				m_nHoldold;						//����������
	D3DXVECTOR3		m_TempPos;						//�ҏW�p�̉����W
	D3DXVECTOR3		m_TempRot;						//�ҏW�p�̉����W
	D3DXVECTOR3		m_ColiPos;						//�ҏW�p�̉����W
	D3DXVECTOR3		m_siz;
	int				m_nSelectCollison;				//�I�����Ă��铖���蔻��̔ԍ�
	int				m_nSelectHurt;					//�I�����Ă�����ꔻ��̔ԍ�
	CCollision* 	m_AxisBox;						//�����o������(�v���C���[�̎�)
	static int		m_snPlayernumber;				//�v���C���[�̔ԍ�
	int				m_nPlayernumber;				//�v���C���[�̔ԍ�
	KEY_SET			m_CopyKey;						//�L�[�̃R�s�[
	CCollision*		m_CopyCollision[MAX_COLLISION];	//���ꔻ��̃R�s�[
	int				m_CopyColNumber;				//�R�s�[������ꔻ��̔ԍ�


	//�g��˂��z

	////�e���v���[�g�֐��錾
	//template <typename T, typename U>
	//T State(T tar,T Tri ,T Hol, U Key,U KeyDown)
	//{//���l�̏㉺��ݒ肷��֐�
	//	if (CApplication::GetInputKeyboard()->GetTrigger(Key))
	//	{
	//		tar += Tri;
	//	}
	//	else if (CApplication::GetInputKeyboard()->GetPress(Key))
	//	{
	//		m_nHold++;
	//		if (m_nHold >= HOLD_TIME)
	//		{
	//			tar += Hol;
	//		}
	//	}
	//	else if (CApplication::GetInputKeyboard()->GetTrigger(KeyDown))
	//	{
	//		tar -= Tri;
	//	}
	//	else if (CApplication::GetInputKeyboard()->GetPress(KeyDown))
	//	{
	//		m_nHold++;
	//		if (m_nHold >= HOLD_TIME)
	//		{
	//			tar -= Hol;
	//		}
	//	}
	//	return tar;
	//}
};

#endif
