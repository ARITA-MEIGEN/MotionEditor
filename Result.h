//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _RESULT_H_
#define _RESULT_H_

//�C���N���[�h
#include"main.h"
#include"Application.h"

//�O���錾
class CBg;
class CScore;

class CResult
{
public:
	CResult();
	~CResult();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CBg*GetBg();
	static CScore*GetScore();

private:
	static CBg*m_pBg;
	static CScore*m_pScore;
};

#endif // !_OBJECT_H_



