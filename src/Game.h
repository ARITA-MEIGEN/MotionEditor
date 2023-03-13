//=============================================================================
//
// �I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================
#ifndef _GAME_H_
#define _GAME_H_

//�C���N���[�h
#include"Application.h"


//�O���錾
class CPlayer;
class CBg;
class CLife;				//�̗̓Q�[�W
class CMapdata;


class CCamera;
class CLight;
class CFloor;
class CMesh;

class CGame
{
public:
	enum GAME
	{
		GAME_NONE = 0,
		GAME_NORMAL,
		GAME_END
	};

	CGame();
	~CGame();

	HRESULT Init();
	void Uninit();
	void Update();
	void Draw();
	static CPlayer*GetPlayer();
	static void SetGame(GAME gamestate);

	//�Q�b�^�[
	static GAME GetGame();
	static CCamera * GetCamera();
	static CFloor*GetFloor();
	static CMesh*GetMesh();
	static CLight*GetLight();

private:
	static CPlayer*m_pPlayer;
	static CBg*m_pBg;
	static CLife*m_Life;				//�̗̓Q�[�W
	static CMapdata*m_pMap;
	static GAME m_gamestate;			//�Q�[���̏��

	static CCamera*m_pCamera;
	static CLight*m_pLight;
	static CFloor*m_pFloor;
	static CMesh*m_pMesh;
};

#endif