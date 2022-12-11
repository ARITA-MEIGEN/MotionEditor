//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_
                               
//�C���N���[�h
#include"main.h"

#define MAX_TYPE (OBJTYPE_MAX+1)

class CObject
{
public:
	//�\����
	enum EObjType
	{
		OBJTYPE_MAP,
		OBJTYPE_PLANET,
		OBJTYPE_GOAL,
		OBJTYPE_BLOCK,
		OBJTYPE_MODEL,
		OBJTYPE_GIMMICK,
		OBJTYPE_UI,
		OBJTYPE_FADE,
		OBJTYPE_MAX,
	};

	//�����o�֐�
	explicit CObject(int nPriority = 3);
	virtual ~CObject();
	virtual HRESULT Init() = 0;	//������
	virtual void Uninit() = 0;								//�I��
	virtual void Update() = 0;								//�X�V
	virtual void Draw() = 0;								//�`��
	static void ReleaseAll();								//�S�̂̔j��
	static void UpdateAll();								//�S�̂̍X�V
	static void DrawAll();									//�S�̂̕`��
	//static CObject* GetObj(int m_nNumber, int nType);		//�I�u�W�F�N�g�̎擾�@�����̓I�u�W�F�N�g�̔ԍ�
	//EObjType GetObjType(int m_nNumber, int nType);		//�I�u�W�F�N�g�̎�ނ̐ݒ�	
	static int NumAll();									//�S�̂̐��̎擾
	void Release();											//�j��
	//void SetObjType(EObjType Type);						//��ނ̐ݒ�
	//static void test();									//�e�X�g�p
	static void Flagcheck();								//���S�t���O�̊m�F
protected:
	int m_frame;											//��������Ă���̎���

private:
	//�����o�ϐ�
	static int m_nNumAll;									//�S�̂̐�
	int m_nPriority;										//�`��̗D�揇��
	static CObject*m_pTop[MAX_TYPE];						//�擪�̃I�u�W�F�N�g�ւ̃|�C���^
	static CObject*m_pCurrent[MAX_TYPE];					//����(��Ԍ��)�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject*m_pPrev;										//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject*m_pNext;										//���̃I�u�W�F�N�g�ւ�
	bool bDead;												//���S�t���O

};

#endif // !_OBJECT_H_
