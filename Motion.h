//=============================================================================
//
// ���[�V�����ǂݍ���
// Author : �L�c����
//
//=============================================================================
#ifndef _MOTION_H_
#define _MOTION_H_
                               
//�C���N���[�h

#define MAX_PARTS	(14)	//�p�[�c�̐�

class CMotion
{
private:
	static const D3DXVECTOR3 INIT_POS;
	static const int MAX_MOTION = 320;
	static const int MAX_KEY = 240;
	static const int MAX_MOTION_ALL = 160;

public:
	//�\����
	struct KEY
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	};

	//�L�[���
	struct KEY_SET 
	{
		int nFrame;				//���̃��[�V�����܂ł̎���
		KEY aKey[MAX_PARTS];
	};

	//�����o�֐�
	 CMotion(int nPriority = 3);
	~CMotion();
	HRESULT Init();						//������
	void Uninit();						//�I��
	void Update();						//�X�V
	void Read(char* Filename, D3DXVECTOR3 pos, D3DXVECTOR3 rot);			//�ǂݍ���
	KEY_SET m_MotionKey[MAX_MOTION];	//�o�^���������̏��
protected:
private:
	int m_NowKey;	//���݂̃L�[

	//�����o�ϐ�

};

#endif // !_OBJECT_H_

