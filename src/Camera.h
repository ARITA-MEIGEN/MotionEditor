//=================================================
// Content     (�Q�[���̐ݒ�)(camera.h)
// Author     : �L�c����
//=================================================
#ifndef _CAMERA_H_		//���̃}�N����`������ĂȂ�������
#define _CAMERA_H_		//��d�C���N���[�h�h�~�̃}�N����`
#include"main.h"

class CCamera
{
public:
	CCamera();
	~CCamera();

	//�v���g�^�C�v�錾
	void Init(void);				//������
	void Uninit(void);				//�I��
	void Update(void);				//�X�V
	void Set(void);					//�ݒ�
	void RadianPosR(void);			//�����_�̐���
	void RadianPosV(void);			//���_�̐���
	void NormalizeRadian(void);		//�p�x�̐��K��
	void ControlPos(void);			//�J�����̈ʒu�̑���
	//void DestPos(void);			//�J�����̒Ǐ]����
	//void LeavingCamera(void);		//�J�����̕��u����
	D3DXVECTOR3 Getrot(void);		//�����擾
	static CCamera* Create(void);	//����

private:
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posVDest;		//���_�̖ړI�n
	D3DXVECTOR3 m_posR;			//�����_
	D3DXVECTOR3 m_posRDest;		//�����_�̖ړI�n
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXVECTOR3 m_rot;			//����
	D3DXVECTOR3 m_rotDest;		//�����̖ړI�̊p�x
	float m_fDistance;			//����
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;			//�r���[�}�g���b�N�X

};

#endif
