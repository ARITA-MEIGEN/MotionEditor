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

//�}�N����`
#define MAX_MOTION	(5)		//���[�V�����̐�
#define MAX_KEY		(60)	//�L�[�̑���
#define NUM_PARTS	(14)	//�p�[�c�̐�

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
	};

	//�L�[���
	struct KEY_SET 
	{
		KEY aKey[NUM_PARTS];		//���f���̐������W������
		int nFrame;					//�Đ�����
	};

	struct MOTION_SET
	{
		KEY_SET		aModelKey[NUM_PARTS];		//�L�[�̑���������
		int			nNumKey;					//�L�[�̑���(�t�@�C���œǂݍ���)
		bool		bLoop;						//���[�v���邩�ǂ���
	};

	enum PLAYER_STATE
	{
		P_NEUTRAL,
		P_MOVE,
		P_LATTACK_STAND,
		P_LATTACK_CROUCH,
		P_LATTACK_JUMP,
	};

	explicit CPlayer(int nPriority = 3);
	~CPlayer();
	//�v���g�^�C�v�錾
	HRESULT			Init()override;
	void			Uninit(void)override;
	void			Update(void)override;
	void			Draw(void)override;
	void			ControlPlayer(void);			//�v���C���[�̑���
	static CPlayer*	Create();
	D3DXMATRIX		GetMtx();
	void			ReadMotion();
	void			MotionPlayer(int nNumber);		//���[�V�����̍Đ��@�����͍Đ����郂�[�V�����̔ԍ�
	void			rolling();
	void			MotionManager();				//��Ԃɍ��킹�ă��[�V�����Đ�����
	void			PlayFirstMotion();				//�O�Ə�Ԃ��Ⴄ�ꍇ�̂ݍŏ��̃��[�V������ݒ肷��

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

};

#endif
