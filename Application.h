//=============================================================================
//
// アプリケーション
// Author : 有田明玄
//
//=============================================================================
#ifndef _APPLICATION_H_
#define _APPLICATION_H_

//*****************************************************************************
// インクルード
//*****************************************************************************
#include"main.h"

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CRenderer;
class CInputKeyboard;
class CInputJoyPad;
class CDebugProc;
class CTitle;
class CGame;
class CResult;
class CFade;

//マクロ定義
#define SCREEN_WIDTH	(1280)		// スクリーンの幅
#define SCREEN_HEIGHT	(720)		// スクリーンの高さ

// プロトタイプ宣言
class CApplication
{
public:
	enum MODE
	{//ゲームモード
		MODE_GAME,
		MODE_MAX
	};

	//メンバ関数
	CApplication();
	~CApplication();
	HRESULT Init(HWND hWnd, bool bWindow,HINSTANCE hInstance);
	void Uninit();
	void Update();
	void Draw();
	//ゲッター
	static CRenderer*GetRenderer();
	static CInputKeyboard*GetInputKeyboard();
	static CInputJoyPad*GetInputJoypad();
	static CDebugProc * GetDebugProc();
	//void DrawStatas();
	static void SetMode(MODE mode);


private:
	//メンバ変数
	static CRenderer*m_pRenderer;
	static CInputKeyboard*m_pInputKeyboard;
	static CInputJoyPad*m_pInputJoyPad;
	static CDebugProc*m_pDebugProc;
	LPD3DXFONT m_pFont;						//フォントへのポイント
	static MODE m_mode;

	//画面
	static CGame*m_pGame;
};

#endif // !_APPLICATION_H_
