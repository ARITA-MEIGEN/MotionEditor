//=================================================
// Content     (�Q�[���̐ݒ�)(light.h)
// Author     : �L�c����
//=================================================
#ifndef _LIGHT_H_		//���̃}�N����`������ĂȂ�������
#define _LIGHT_H_		//��d�C���N���[�h�h�~�̃}�N����`
#include"main.h"

//�}�N����`
#define MAX_LIGHT	(3)	//���C�g�̐�

class CLight
{
public:
	//�v���g�^�C�v�錾
	void Init(void);
	void Uninit(void);
	void Update(void);
	D3DXVECTOR3 GetVec(int number);

private:
	D3DLIGHT9 m_alight[MAX_LIGHT];	//���C�g���

};

#endif

