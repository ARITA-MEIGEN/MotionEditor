//=============================================================================
//
// 3D�I�u�W�F�N�g
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include"DebugProc.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include"Application.h"
#include"renderer.h"

//�ÓI�����o�ϐ�
LPD3DXFONT CDebugProc::m_pFont = nullptr;
char CDebugProc::m_aStr[1024] = {};
//=============================================================================
// �R���X�g���N�^
//=============================================================================
CDebugProc::CDebugProc()
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CDebugProc::~CDebugProc()
{

}

//=============================================================================
// ������
//=============================================================================
void CDebugProc::Init()
{
	//�f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CApplication::GetRenderer()->GetDevice();

	// �f�o�b�O���\���p�t�H���g�̐���
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &m_pFont);
}

//=============================================================================
// �I��
//=============================================================================
void CDebugProc::Uninit()
{

}

//=============================================================================
// ������ǂݍ���
//=============================================================================
void CDebugProc::Print(const char * pFormat, ...)
{
	va_list args;
	va_start(args, pFormat);
	char aStr[128] = {};
	for (const char* p = pFormat; *p != '\0'; ++p)
	{
		if (*p == '%')
		{
			switch (*++p)
			{
			case 'd':
				sprintf(&aStr[0], "%d", va_arg(args, int));
				break;
			case 'f':
				sprintf(&aStr[0], "%.2f", va_arg(args, double));
				break;
			case 'c':
				sprintf(&aStr[0], "%c", va_arg(args, char));
				break;
			case 's':
				sprintf(&aStr[0], "%s", va_arg(args, const char*));
				break;
			default:
				assert(!"�s���ȕϊ��w��");
				break;
			}
			strcat(m_aStr, aStr);
		}
		else
		{
			sprintf(&aStr[0],"%c", *p);
			strcat(m_aStr, aStr);
		}
	}
	strcat(m_aStr,"\n");

	va_end(args);
}

//=============================================================================
// �`��
//=============================================================================
void CDebugProc::Draw()
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �e�L�X�g�`��
	m_pFont->DrawText(NULL, m_aStr, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

	//�e�L�X�g���Z�b�g
	m_aStr[0] = {};
}
