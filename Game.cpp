//=============================================================================
//
// オブジェクト
// Author : 有田明玄
//
//=============================================================================
//インクルード
#include"main.h"
#include"Application.h"
#include"Game.h"
#include"Player.h"
#include"InputKeyBoard.h"
#include"InputJoyPad.h"
#include"Floor.h"
#include"Player.h"
#include"Mesh.h"
#include"Camera.h"
#include"Light.h"
#include"Shadow.h"
#include"Fade.h"
#include"Collision.h"

//静的メンバ変数
CPlayer*CGame::m_pPlayer = nullptr;
CBg*CGame::m_pBg = nullptr;
//CLife*CGame::m_Life = nullptr;				//体力ゲージ
CGame::GAME CGame::m_gamestate;

CCamera*CGame::m_pCamera = nullptr;
CLight*CGame::m_pLight = nullptr;
CFloor*CGame::m_pFloor = nullptr;
CMesh*CGame::m_pMesh = nullptr;
//====================================
//コンストラクタ
//====================================
CGame::CGame()
{
	
}

//====================================
//デストラクタ
//====================================
CGame::~CGame()
{

}

//====================================
//初期化
//====================================
HRESULT CGame::Init()
{
	//テクスチャの読み込み
	CShadow::Load();
	CFloor::Load();

	//マップデータ読み込み
	//プレイヤーの生成
	m_pPlayer=CPlayer::Create();

	//背景の生成
	//メッシュフィールドの設定
	//m_pMesh = CMesh::Create(20, 20,D3DXVECTOR3 (-200.0f, 0.0f, 200.0f));

	//カメラの設定
	m_pCamera = CCamera::Create();

	//ライトの設定
	m_pLight = new CLight;
	m_pLight->Init();

	CFloor::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(500.0f, 500.0f,500.0f));

	return S_OK;
}

//====================================
//終了
//====================================
void CGame::Uninit()
{
	//テクスチャの破棄
	CShadow::Unload();
	CFloor::Unload();

	//カメラの設定
	if (m_pCamera != nullptr)
	{
		m_pCamera->Uninit();
		delete m_pCamera;
		m_pCamera=nullptr;

	}

	//ライトの設定
	if (m_pLight != nullptr)
	{
		m_pLight->Uninit();
		delete m_pLight;
		m_pLight = nullptr;
	}
}

//====================================
//更新
//====================================
void CGame::Update()
{
	m_pCamera->Update();
	m_pLight->Update();
}

//====================================
//描画
//====================================
void CGame::Draw()
{
	m_pCamera->Set();
}

//====================================
//プレイヤーの取得
//====================================
CPlayer * CGame::GetPlayer()
{
	return m_pPlayer;
}

//====================================
//ゲームの状態設定
//====================================
void CGame::SetGame(GAME gamestate)
{
	m_gamestate = gamestate;
}

//====================================
//ゲームの状態獲得
//====================================
CGame::GAME CGame::GetGame()
{
	return m_gamestate;
}

//====================================
//カメラの取得
//====================================
CCamera * CGame::GetCamera()
{
	return m_pCamera;
}

//====================================
//床の情報
//====================================
CFloor * CGame::GetFloor()
{
	return m_pFloor;
}

//====================================
//床の情報
//====================================
CMesh * CGame::GetMesh()
{
	return m_pMesh;
}

//====================================
//ライトの情報
//====================================
CLight * CGame::GetLight()
{
	return m_pLight;
}


////======================================================
////ステータスの表示
////======================================================
//void CApplication::DrawStatas(void)
//{
//	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
//	char aStr[512];
//
//	//文字列に代入
//	//wsprintf(&aStr[0], "FPS:%3d\n", g_nCountFPS);
//
//	//モデルの操作キー
//	strcat(aStr, "モデルの操作:↑/↓/←/→\n");
//
//	//視点の旋回キー
//	strcat(aStr, "視点の旋回:Q/E/R/F\n");
//
//	//注視点の旋回キー
//	strcat(aStr, "注視点の旋回:J/L/I/K\n");
//
//	//カメラの位置操作キー
//	strcat(aStr, "カメラの位置操作:W/A/S/D\n");
//
//	////視点の座標
//	//char aStr2[400];
//	//sprintf(aStr2, "視点の位置:%3f/%3f/%3f\n", m_pCamera->posV.x, m_pCamera->posV.y, m_pCamera->posV.z);
//	//strcat(aStr, aStr2);
//
//	////注視点の座標
//	//char aStr3[400];
//	//sprintf(aStr3, "注視点の位置:%3f/%3f/%3f\n", m_pCamera->posR.x, m_pCamera->posR.y, m_pCamera->posR.z);
//	//strcat(aStr, aStr3);
//
//	////ワイヤーフレーム
//	//strcat(aStr, "ワイヤーフレームON/OFF:1/2\n");
//
//	////モデルの操作キー
//	//strcat(aStr, "発射ァ！:SPACEを押すんだ！\n");
//
//	//テキストの描画
//	m_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 255, 255, 255));
//}