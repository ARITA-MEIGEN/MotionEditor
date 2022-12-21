//=============================================================================
//
// アプリケーション
// Author : 有田明玄
//
//=============================================================================

// インクルード
#include "Application.h"
#include"renderer.h"
#include"main.h"
#include"InputKeyBoard.h"
#include"InputJoyPad.h"
#include"DebugProc.h"
#include"Object.h"
#include"Fade.h"

#include"Title.h"
#include"Game.h"
#include"Result.h"

//====================================
//静的メンバ変数
//====================================
CRenderer*CApplication::m_pRenderer = nullptr;
CInputKeyboard*CApplication::m_pInputKeyboard = nullptr;
CInputJoyPad*CApplication::m_pInputJoyPad = nullptr;
CDebugProc*CApplication::m_pDebugProc = nullptr;

//画面遷移周り
CApplication::MODE CApplication::m_mode;
CGame*CApplication::m_pGame = nullptr;
//====================================
//コンストラクタ
//====================================
CApplication::CApplication()
{

}

//====================================
//デストラクタ
//====================================
CApplication::~CApplication()
{

}

//====================================
//初期化
//====================================
HRESULT CApplication::Init(HWND hWnd, bool bWindow, HINSTANCE hInstance)
{
	m_pRenderer = new CRenderer;
	m_pInputKeyboard = new CInputKeyboard;
	m_pInputJoyPad = new CInputJoyPad;

	//初期化処理
	if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
	{//初期化処理に失敗した場合
		return-1;
	}

	//キーボードの生成
	if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
	{
		return-1;
	}

	//ジョイパッドの生成(XInput)
	if (FAILED(m_pInputJoyPad->Init()))
	{
		return-1;
	}

	SetMode(MODE_GAME);

	//----------------------------
	// デバッグ用文字の生成
	//----------------------------
	m_pDebugProc = new CDebugProc;
	m_pDebugProc->Init();

	return S_OK;
}

//====================================
//終了
//====================================
void CApplication::Uninit()
{
	//----------------------------
	//レンダリングクラスの破壊
	//----------------------------
	if (m_pRenderer != nullptr)
	{
		// 終了処理
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	//----------------------------
	//キーボードの破棄
	//----------------------------
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Uninit();
		delete m_pInputKeyboard;
		m_pInputKeyboard = nullptr;
	}

	//----------------------------
	//ジョイパッドの破棄
	//----------------------------
	if (m_pInputJoyPad != nullptr)
	{
		m_pInputJoyPad->Uninit();
		delete m_pInputJoyPad;
		m_pInputJoyPad = nullptr;
	}

	//----------------------------
	// ゲームの終了
	//----------------------------
	if (m_pGame != nullptr)
	{
		m_pGame->Uninit();
		delete m_pGame;
		m_pGame = nullptr;
	}
	
	//----------------------------
	// デバッグ用文字の終了
	//----------------------------
	if (m_pDebugProc != nullptr)
	{
		m_pDebugProc->Uninit();
		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}
}

//====================================
//更新
//====================================
void CApplication::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CApplication::GetRenderer()->GetDevice();	//デバイスへのポインタ
	//ワイヤーフレーム
	if (m_pInputKeyboard->GetTrigger(DIK_1))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	if (m_pInputKeyboard->GetTrigger(DIK_2))
	{
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
	CDebugProc::Print("ワイヤーフレームの設定: F1/F2");


	//レンダラー更新
	m_pRenderer->Update();
	
	//キーボードの更新
	if (m_pInputKeyboard != nullptr)
	{
		m_pInputKeyboard->Update();
	}

	//ジョイパッドの更新
	if (m_pInputJoyPad!=nullptr)
	{
		m_pInputJoyPad->Update();
	}

	//モードごとの更新
	switch (m_mode)
	{
	case MODE_GAME:
		if (m_pGame != nullptr)
		{
			m_pGame->Update();
		}
		break;

	default:
		break;
	}
}

//====================================
//描画
//====================================
void CApplication::Draw()
{
	if (m_pRenderer != nullptr)
	{
		m_pRenderer->Draw();
	}

	//モードごとの描画
	switch (m_mode)
	{

	case MODE_GAME:
		if (m_pGame != nullptr)
		{
			m_pGame->Draw();
		}
		break;

	default:
		break;
	}
}

//====================================
//レンダラーの取得
//====================================
CRenderer * CApplication::GetRenderer()
{
	return m_pRenderer;
}

//====================================
//キーボードの取得
//====================================
CInputKeyboard * CApplication::GetInputKeyboard()
{
	return m_pInputKeyboard;
}

//====================================
//ジョイパッドの取得
//====================================
CInputJoyPad * CApplication::GetInputJoypad()
{
	return m_pInputJoyPad;
}

//====================================
//デバッグ情報の取得
//====================================
CDebugProc * CApplication::GetDebugProc()
{
	return m_pDebugProc;
}

//======================================================
//モードの設定
//======================================================
void CApplication::SetMode(MODE mode)
{
	//新しい画面(モード)の初期化処理
	switch (m_mode)
	{
	case MODE_GAME:		//ゲーム画面
		m_pGame->Uninit();
		break;

	}
	m_mode = mode;	//現在の画面(モード)を切り替える
	CObject::ReleaseAll();

	//新しい画面(モード)の初期化処理
	switch (m_mode)
	{
	case MODE_GAME:		//ゲーム画面
		m_pGame = nullptr;
		m_pGame = new CGame;
		m_pGame->Init();
		break;
	}
}