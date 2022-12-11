//=============================================================================
//
// ��
// Author : �L�c����
//
//=============================================================================

#ifndef _FLOOR_H_
#define _FLOOR_H_

//�C���N���[�h
#include"main.h"
#include"Object3D.h"

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CApplication;

//*****************************************************************************
// �\���̒�`
//*****************************************************************************

class CFloor : public CObject3D
{
public:
	//�����o�֐�
	explicit CFloor(int nPriority = 3);
	~CFloor()override;
	HRESULT Init()override;
	void Uninit()override;
	void Update()override;
	void Draw()override;
	static CFloor*Create(D3DXVECTOR3 pos, D3DXVECTOR3 siz);
	static void Load();
	static void Unload();

private:
	//�����o�ϐ�
	static LPDIRECT3DTEXTURE9 m_pTexture;		//�|���S���̃e�N�X�`��
};
#endif // !_OBJECT_H_



