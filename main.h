//=============================================================================
//
// ���C��
// Author : �L�c����
//
//=============================================================================
#ifndef _MAIN_H_
#define _MAIN_H_						//���d�C���N���[�h�h�~
#define DIRECTINPUT_VERSION     0x0800	// DirectInput�̃o�[�W�����w��

//*****************************************************************************
// �C���N���[�h
//*****************************************************************************
#include "dinput.h"			//���͏����ɕK�v



//*****************************************************************************
// ���C�u�����[�����N
//*****************************************************************************
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"dInput8.lib")		//���͏����ɕK�v
#pragma comment (lib, "xinput.lib")		//Xinput�����ɕK�v


//�}�N����`
#define MAX_POLYGON		(1920)			//�|���S���̍ő吔
#define SCREEN_WIDTH	(1280)			// �X�N���[���̕�
#define SCREEN_HEIGHT	(720)			// �X�N���[���̍���
#define IMGUI_FONT_PASS	("c:\\Windows\\Fonts\\meiryo.ttc")	//IGMUI�p�̃t�H���g�p�X

//�O���錾
class CApplication;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int GetFPS(void);


#endif // !_MAIN_H_


