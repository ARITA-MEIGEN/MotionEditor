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
	};

	struct MOTION_SET
	{
		KEY_SET	aModelKey[MAX_KEY];		//�L�[�̑���������
		int		nNumKey;				//�L�[�̑���(�t�@�C���œǂݍ���)
		bool	bLoop;					//���[�v���邩�ǂ���
	};

	enum PLAYER_STATE
	{
		P_STAND,
		P_CROUCH,
		P_MOVE,
		P_LATTACK_STAND,
		P_LATTACK_CROUCH,
		P_LATTACK_JUMP,
		P_MATTACK_STAND,
		P_MATTACK_CROUCH,
		P_MATTACK_JUMP,
		P_HATTACK_STAND,
		P_HATTACK_CROUCH,
		P_HATTACK_JUMP,
		P_MAX
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//�v���g�^�C�v�錾
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	bool			ControlPlayer(void);			//�v���C���[�̑���
	static CPlayer*	Create();
	D3DXMATRIX		GetMtx();
	void			ReadMotion();					//���[�V�����ǂݍ���
	void			WriteMotion(int nowmotion);		//���[�V���������o��
	void			MotionPlayer(int nNumber);		//���[�V�����̍Đ��@�����͍Đ����郂�[�V�����̔ԍ�
	void			rolling();
	void			MotionManager();				//��Ԃɍ��킹�ă��[�V�����Đ�����
	void			PlayFirstMotion();				//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��
	void			EditMode();						//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��
	void			Normalize(int number);			//�p�x�̐��K��
	void			PlayerStateDraw();				//�v���C���[�̏��̕ҏW�ƕ\��(IMGUI)
	void			DrawCollision();				//�v���C���[�̏��̕ҏW�ƕ\��(IMGUI)
	void			SetFrame();						//�t���[�������ă��[�V�����̈ʒu��\������



private:
	CModel*			m_apModel[NUM_PLAYERPARTS];		//���f���̃C���X�^���X
	MOTION_SET		m_apMotion[MAX_MOTION];			//���[�V�����̐������������[�V�����̐�->�L�[�̑���->���f���̐�
	D3DXMATRIX		m_mtxWorld;						//���[���h�}�g���b�N�X
	D3DXVECTOR3		m_pos;							//�ʒu
	D3DXVECTOR3		m_rot;							//����
	D3DXVECTOR3		m_move;							//�ړ���
	D3DXVECTOR3		m_posold;						//�O��̈ʒu
	//CShadow*		m_Shadow;						//�e
	D3DXVECTOR3		m_rotDest;						//�ړI�̊p�x�̕ۑ�
	int				m_MotionCnt;					//���[�V�����J�E���^�[
	int				m_nNumKey;						//�L�[�̑���
	int				m_nCurrentKey;					//���݂̃L�[�ԍ�
	D3DXMATRIX		m_mtxRot;						//��]�}�g���b�N�X(�ۑ��p)
	D3DXQUATERNION	m_quat;							//�N�H�[�^�j�I��
	D3DXVECTOR3		m_vecAxis;						//��]��
	float			m_fRolling;						//��]�ʁ@(��]�p)
	int				m_nNumModel;
	char			m_nModelpass[255];
	D3DXVECTOR3		m_movepos;
	D3DXVECTOR3		m_moverot;
	PLAYER_STATE	m_pState;
	PLAYER_STATE	m_pStateOld;					//�ЂƂO�̃��[�V����
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
	int				m_nSelectCollison;
	CCollision* 	m_AxisBox;						//�����o������(�v���C���[�̎�)



	//�e���v���[�g�֐��錾
	template <typename T, typename U>
	T State(T tar,T Tri ,T Hol, U Key,U KeyDown)
	{//���l�̏㉺��ݒ肷��֐�
		if (CApplication::GetInputKeyboard()->GetTrigger(Key))
		{
			tar += Tri;
		}
		else if (CApplication::GetInputKeyboard()->GetPress(Key))
		{
			m_nHold++;
			if (m_nHold >= HOLD_TIME)
			{
				tar += Hol;
			}
		}
		else if (CApplication::GetInputKeyboard()->GetTrigger(KeyDown))
		{
			tar -= Tri;
		}
		else if (CApplication::GetInputKeyboard()->GetPress(KeyDown))
		{
			m_nHold++;
			if (m_nHold >= HOLD_TIME)
			{
				tar -= Hol;
			}
		}

		return tar;
	}
};

#endif
