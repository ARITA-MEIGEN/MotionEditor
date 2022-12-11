//=============================================================================
//
// X�t�@�C���ǂݍ���
// Author : �L�c����
//
//=============================================================================

#ifndef _SHADOW_H_
#define _SHADOW_H_

//�C���N���[�h
#include"main.h"
#include"Object.h"
#include"Object3D.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CApplication;

class CShadow : public CObject3D
{
public:
	//�����o�֐�
	explicit CShadow(int nPriority = 3);
	~CShadow()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CShadow*Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz);
	static void Load();
	static void Unload();

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;

	//�����o�ϐ�
};

#endif
