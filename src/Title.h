//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _TITLE_H_
#define _TITLE_H_

//�C���N���[�h
#include"main.h"
#include"Application.h"

//�O���錾
class CObject2D;

class CTitle
{
public:
	CTitle();
	~CTitle();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CObject2D* GetBg();

private:
	static CObject2D*m_pBg;
};

#endif


