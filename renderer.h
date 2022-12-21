//=============================================================================
//
// �����_�[
// Author : �L�c����
//
//=============================================================================

#ifndef _RENDERER_H_
#define _RENDERER_H_

//�C���N���[�h
#include"main.h"

//�O���錾
class CObject2D;
class CPlayer;

//�}�N����`
class CRenderer
{
public:
	//�����o�֐�
	CRenderer();
	~CRenderer();
	 HRESULT Init(HWND hWnd, bool bWindow);
	 void Uninit();
	 void Update();
	 void Draw();
	 void DrawFPS();
	 LPDIRECT3DDEVICE9 GetDevice(void);/* {return g_pD3DDevice}*/


private:
	//�����o�ϐ�
	LPDIRECT3D9 g_pD3D;								// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9 g_pD3DDevice;					// Device�I�u�W�F�N�g
	LPD3DXFONT g_pFont;								// �t�H���g

};

#endif // !_RENDERER_H_
