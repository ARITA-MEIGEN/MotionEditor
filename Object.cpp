//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include "Object.h"
#include"Application.h"
#include"renderer.h"
#include"InputKeyBoard.h"

//�ÓI�����o�ϐ��錾
 int CObject::m_nNumAll=0;
 CObject*CObject::m_pTop[MAX_TYPE] = {nullptr};
 CObject*CObject::m_pCurrent[MAX_TYPE] = { nullptr };
//=============================================================================
// �R���X�g���N�^
//=============================================================================
 CObject::CObject(int nPriority /*=3*/)
 {
	 CObject*pObject;
	 pObject = this;

	 //���g����
	 if (m_pTop[nPriority] == nullptr)
	 {//�ŏ��̈��ڂ̏ꍇ
		 m_pTop[nPriority] = pObject;
	 }

	 if (m_pCurrent[nPriority] != nullptr)
	 {//�Ō���̐ݒ�
		 m_pCurrent[nPriority]->m_pNext = pObject;
		 pObject->m_pPrev = m_pCurrent[nPriority];
	 }
	 pObject->bDead = false;
	 m_pCurrent[nPriority] = pObject;			//�Ō���Ɏ�����ݒ�
	 m_nNumAll++;
 }

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObject::~CObject()
{
	m_nNumAll--;
}

//=============================================================================
//�S�Ă̏I��
//=============================================================================
void CObject::ReleaseAll()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		if (!m_pTop[i])
		{//Top��null�Ȃ�
			continue;
		}

		CObject*pObject = m_pTop[i];

		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			pObject->Uninit();
			pObject = pObjectNext;
		}
	}
	Flagcheck();
}

//=============================================================================
//�S�Ă̍X�V
//=============================================================================
void CObject::UpdateAll()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		if (!m_pTop[i])
		{//Top��null�Ȃ�
			continue;
		}

		CObject*pObject = m_pTop[i];

		while (pObject!=nullptr)
		{
			CObject*pObjectNext = pObject->m_pNext;
			if (pObject->bDead == false)
			{
				pObject->Update();
			}
			pObject = pObjectNext;
		}
	}

	Flagcheck();
}

//=============================================================================
// �S�Ă̕`��
//=============================================================================
void CObject::DrawAll()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		CObject*pObject = m_pTop[i];
		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			pObject->Draw();
			pObject = pObjectNext;
		}
	}
}

////=============================================================================
//// �I�u�W�F�N�g�̎擾
////=============================================================================
//CObject* CObject::GetObj(int m_nNumber,int nType)
//{
//	return m_pObject[m_nNumber];
//}

////=============================================================================
//// �I�u�W�F�N�g�̎�ނ̎擾
////=============================================================================
//CObject::EObjType CObject::GetObjType(int m_nNumber, int nType)
//{
//	return m_pObject[m_nNumber]->m_ObjType;
//}

//=============================================================================
//�S�̂̎擾
//=============================================================================
int CObject::NumAll()
{
	return m_nNumAll;
}

//=============================================================================
// �P�̂̏I������
//=============================================================================
void CObject::Release()
{
	CObject*pObject = this;
	if (pObject != nullptr)
	{
		pObject->bDead = true;
	}
}

//=============================================================================
// ���S�t���O�̊m�F
//=============================================================================
void CObject::Flagcheck()
{
	for (int i = 0; i < MAX_TYPE; i++)
	{
		CObject*pObject = m_pTop[i];
		while (pObject)
		{
			CObject*pObjectNext = pObject->m_pNext;
			if (pObject->bDead == true)
			{//���S�t���O�����Ă�ꍇ
				//�����̃I�u�W�F�N�g�̑O���A��������
				if (m_pTop[i] == m_pCurrent[i])
				{
					m_pTop[i] = nullptr;
					m_pCurrent[i] = nullptr;
				}
				else if (m_pTop[i] == pObject)
				{//�g�b�v���폜����ꍇ
					m_pTop[i] = pObjectNext;
					m_pTop[i]->m_pPrev = nullptr;
				}
				else if (m_pCurrent[i] == pObject)
				{//�Ō���̃I�u�W�F�N�g���폜����ꍇ
					m_pCurrent[i] = pObject->m_pPrev;
					pObject->m_pPrev->m_pNext = nullptr;
					pObject->m_pPrev = nullptr;
				}
				else
				{//���X�g����j��
					pObject->m_pPrev->m_pNext = pObject->m_pNext;
					pObject->m_pNext->m_pPrev = pObject->m_pPrev;
				}
				//���g�����폜
				delete pObject;
				pObject = nullptr;
			}
			//���̃I�u�W�F�N�g�ֈڍs
			pObject = pObjectNext;
		}
	}
}

//
////=============================================================================
//// ��ނ̐ݒ�
////=============================================================================
//void CObject::SetObjType(EObjType Type)
//{
//	m_ObjType = Type;
//}
