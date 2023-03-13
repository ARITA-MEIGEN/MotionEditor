//=================================================
// Content     (ゲームの設定)(player.cpp)
// Author     : 有田明玄
//=================================================

//インクルード
#include"Player.h"
#include"Application.h"
#include"renderer.h"
#include"Camera.h"
#include"InputKeyBoard.h"
#include"Shadow.h"
#include"Mesh.h"
#include"Model.h"
#include"Game.h"
#include"Collision.h"

//マクロ定義
#define PLAYER_SPEED	(2.0f)	//移動速度
#define MIN_SPEED		(0.1f)	//摩擦による最低速度

int CPlayer::m_snPlayernumber = 0;

//===========================
//コンストラクタ
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	m_nPlayernumber = m_snPlayernumber;
	m_snPlayernumber++;
}

//===========================
//デストラクタ
//===========================
CPlayer::~CPlayer()
{

}

//===========================
//初期化処理
//===========================
HRESULT CPlayer::Init()
{
	m_pMotion = PM_ST_NEUTRAL;
	m_nKEYData = 0;
	m_bPlay = false;

	for (int i = 0; i < NUM_PARTS; i++)
	{//プレイヤーの生成
		m_apModel[i] = CModel::Create();
	}

	//エラー防止用に全ての数値に0を入れる
	for (int i = 0; i < MAX_MOTION; i++)
	{//モーションの初期化
		for (int j = 0; j < MAX_KEY; j++)
		{
			for (int k = 0; k < NUM_PARTS; k++)
			{
				m_apMotion[i].aModelKey[j].aKey[k].fPosX = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fPosY = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fPosZ = 0.0f;

				m_apMotion[i].aModelKey[j].aKey[k].fRotX = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fRotY = 0.0f;
				m_apMotion[i].aModelKey[j].aKey[k].fRotZ = 0.0f;
			}
			m_apMotion[i].aModelKey[j].nNumCollision = 0;	//ダメージ判定の数を初期化
			m_apMotion[i].aModelKey[j].nNumHurtCol = 0;		//やられ判定の数を初期化
			m_apMotion[i].aModelKey[j].nFrame = 1;			//キーの再生時間を1に設定
		}
		m_apMotion[i].nHitStopTimer = 0;	//ヒットストップ初期化

	}

	m_AxisBox = CCollision::Create(m_pos,CCollision::COLLI_AXIS);						//押し出し判定(プレイヤーの軸)
	
	//モデルとモーションの読み込み
	ReadMotion();

	for (int i = 0; i < NUM_PARTS; i++)
	{
		//プレイヤーの生成
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fPosX,
			m_apMotion[0].aModelKey[0].aKey[i].fPosY,
			m_apMotion[0].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fRotX,
			m_apMotion[0].aModelKey[0].aKey[i].fRotY,
			m_apMotion[0].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//差分の取得
	}

	//回転マトリックスの初期化
	//D3DXMatrixIdentity(&m_mtxRot);

	//判定の座標設定
	for (int i = 0; i < PM_MAX; i++)
	{
		for (int k = 0; k < m_apMotion[i].nNumKey; k++)
		{
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumCollision; j++)
			{//違うモーションの当たり判定をオフにする
				if (m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
				{//キーとモーションが一致している場合のみ表示{//当たり判定
					m_apMotion[i].aModelKey[k].Collision[j]->SetPos(m_pos + m_apMotion[i].aModelKey[k].Collision[j]->GetDPos());
				}
			}
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumHurtCol; j++)
			{//やられ判定
				if (m_apMotion[i].aModelKey[k].HurtCol[j] != nullptr)
				{
					m_apMotion[i].aModelKey[k].HurtCol[j]->SetPos(m_apMotion[i].aModelKey[k].HurtCol[j]->GetDPos() + m_pos);
				}
			}
		}
	}

	m_rot.y = D3DX_PI*0.5f;

	return S_OK;
}

//===========================
//終了処理
//===========================
void CPlayer::Uninit(void)
{

	for (int i = 0; i < NUM_PARTS; i++)
	{//プレイヤーの生成
		m_apModel[i]->Uninit();
		delete m_apModel[i];
		m_apModel[i] = nullptr;
	}

	CObject::Release();
}

//===========================
//更新処理
//===========================
void CPlayer::Update(void)
{
	DrawCollision();

	if (m_bPlay == true)
	{
		m_posold = m_pos;		//前回の位置の保存
		m_pos += m_move;		//位置の更新
		m_AxisBox->SetPos(m_move + m_AxisBox->GetPos());
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の更新

		if (ControlPlayer() == true)				//操作
		{
			//m_pMotion = PM_STAND;	//操作されていない場合ニュートラルに
		}

		CDebugProc::Print("現在のモード:PLAY");

		MotionManager();

		CDebugProc::Print("現在のモード:%d　", (int)m_pMotion);
	}
	else 
	{
		EditMode();
		PlayerStateDraw();
	}
	SetHitBox();

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_F2))
	{//モード切り替え
		m_bPlay = !m_bPlay;
		m_pMotion = PM_ST_NEUTRAL;
		m_frame = 0;
		m_nKEYData = 0;
	}

	CDebugProc::Print("現在のプレイヤーの座標:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
}

//===========================
//描画処理
//===========================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//デバイスへのポインタ
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用

	//現在のマテリアルを維持
	pDevice->GetMaterial(&matDef);

	//パーツのワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//パーツのモデルの向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y+ m_TempRot.y, m_rot.x + m_TempRot.x, m_rot.z + m_TempRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//パーツのモデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_TempPos.x, m_pos.y + m_TempPos.y, m_pos.z + m_TempPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//Shadow();

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int i = 0; i < NUM_PARTS; i++)
	{
		m_apModel[i]->Draw(m_mtxWorld);
	}
}

//===========================
//操作
//===========================
bool CPlayer::ControlPlayer(void)
{
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	bool bNeutral = true;

	//移動
	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_LCONTROL))
	{
		m_pMotion = PM_CR_NEUTRAL;
	}
		if (pKeyboard->GetPress(DIK_UP))
		{
			m_move.x = sinf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pMotion = PM_ST_MOVE;
			//m_rotDest.y = D3DX_PI*1.0f + pCamera->Getrot().y;

			bNeutral = false;
		}
		if (pKeyboard->GetPress(DIK_DOWN))
		{
			m_move.x = sinf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pMotion = PM_ST_MOVE;
			//m_rotDest.y = D3DX_PI*0.0f + pCamera->Getrot().y;
			bNeutral = false;

		}
		if (pKeyboard->GetPress(DIK_LEFT))
		{
			m_move.x = sinf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pMotion = PM_ST_MOVE;
			//m_rotDest.y = D3DX_PI*0.5f + pCamera->Getrot().y;
			bNeutral = false;

		}
		if (pKeyboard->GetPress(DIK_RIGHT))
		{
			m_move.x = sinf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pMotion = PM_ST_MOVE;
			//m_rotDest.y = D3DX_PI*-0.5f + pCamera->Getrot().y;
			bNeutral = false;

		}
		if (pKeyboard->GetPress(DIK_K))
		{
			m_move.x = sinf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pMotion = PM_ST_MOVE;
			//m_rotDest.y = D3DX_PI*-0.5f + pCamera->Getrot().y;
			bNeutral = false;

		}

	 if (pKeyboard->GetTrigger(DIK_U))
	 {
		 m_pMotion = PM_ST_LATTACK;
		 bNeutral = false;
	 }
	 if (pKeyboard->GetTrigger(DIK_I))
	 {
		 m_pMotion = PM_ST_MATTACK;
		 bNeutral = false;
	 }
	 if (pKeyboard->GetTrigger(DIK_O))
	 {
		 m_pMotion = PM_ST_HATTACK;
		 bNeutral = false;
	 }

	//角度の正規化
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	return bNeutral;
}

//===========================
//操作
//===========================
CPlayer * CPlayer::Create()
{
	CPlayer*pPlayer;
	pPlayer = new CPlayer(CObject::OBJTYPE_MODEL);
	pPlayer->Init();

	return pPlayer;
}

//===========================
//マトリックスの取得
//===========================
D3DXMATRIX CPlayer::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//モーション読み込み
//===========================
void CPlayer::ReadMotion()
{
	//ファイル読み込み
	const int lenLine = 2048;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	char Read[lenLine];			//読み取る用
	int	m_pEnemynumber = 0;		//モデルの番号
	int motionnumber = 0;		//モーションの番号
	int key = 0;
	int m_pEnemy = 0;
	int Idx = 0;

	//ファイル読み込み
	FILE*fp = fopen("data/TXT/ReadEditMotion.txt","r");		//ファイル読み込み
	if (fp == nullptr)
	{//開けなかった時用
		assert(false);
	}
	if (fp != NULL)
	{
		while (fgets(Read, lenLine, fp) != nullptr)
		{
			//文字列の分析
			sscanf(Read, "%s", &strLine);
			if (strcmp(&strLine[0], "SCRIPT") == 0)	//比較して一致するかどうか調べる
			{
				while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ(一行読み込み)
				{//モデルの初期設定
					ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																	//文字列の分析
					sscanf(Read, "%s", &strLine);

					if (strcmp(&strLine[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else if (strcmp(&strLine[0], "NUM_MODEL") == 0)
					{
						sscanf(Read, "%s = %d", &strLine, &m_nNumModel);	//読み込んだ文字ごとに設定する
					}
					else if (strcmp(&strLine[0], "MODEL_FILENAME") == 0)
					{
						sscanf(Read, "%s = %s", &strLine, &m_nModelpass[0]);	//モデルのパスの設定

						m_apModel[m_pEnemynumber]->SetModel(&m_nModelpass[0]);
						m_pEnemynumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//初期位置の設定
						while (fgets(Read, lenLine, fp) != nullptr)//一行読み込み
						{//キャラクターの設定
						 //文字列の分析
							ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//読み込みループ //一行読み込み
								{//パーツの設定	
									ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																					//文字列の分析
									sscanf(Read, "%s", &strLine);
									if (strcmp(&strLine[0], "END_PARTSSET") == 0)
									{//パーツの設定終了
										Idx++;
										break;
									}
									else if (strcmp(&strLine[0], "INDEX") == 0)
									{//インデックスの設定
										sscanf(Read, "%s = %d", &strLine, &Idx);	//モデルのパスの設定
									}
									else if (strcmp(&strLine[0], "PARENT") == 0)
									{//親モデルの設定
										int Parent;
										sscanf(Read, "%s = %d", &strLine, &Parent);	//モデルのパスの設定
										m_apModel[Idx]->SetParent(m_apModel[Parent]);
									}
									else if (strcmp(&strLine[0], "POS") == 0)
									{//位置
										D3DXVECTOR3 pos;
										sscanf(Read, "%s = %f%f%f", &strLine, &pos.x, &pos.y, &pos.z);	//座標の取得
										m_apModel[Idx]->SetPos(pos);
										m_apModel[Idx]->SetDPos(pos);
									}
									else if (strcmp(&strLine[0], "ROT") == 0)
									{//向き
										D3DXVECTOR3 rot;
										sscanf(Read, "%s = %f%f%f", &strLine, &rot.x, &rot.y, &rot.z);
										m_apModel[Idx]->SetRot(rot);
										m_apModel[Idx]->SetDRot(rot);
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "MOTIONSET") == 0)
					{//モーションの設定
						while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																			//文字列の分析
							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
							{
								if (motionnumber > PM_MAX)
								{
									assert(false);
								}
								break;
							}
							else if (strcmp(&strLine[0], "MOTION") == 0)
							{
								const char cFilename[255] = {};
								sscanf(Read, "%s = %s", &strLine, &cFilename[0]);	//ファイル読み込み

																					//ファイル読み込み
								FILE* sta = fopen(cFilename, "r");		//ファイル読み込み
								if (sta == nullptr)
								{//開けなかった時用
									assert(false);
								}
								if (sta != NULL)
								{
									while (fgets(Read, lenLine, sta) != nullptr)	//読み込み用ループ
									{
										ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																						//文字列の分析
										sscanf(Read, "%s", &strLine);

										if (strcmp(&strLine[0], "MOTIONSET") == 0)
										{
											while (fgets(Read, lenLine, sta) != nullptr)	//読み込み用ループ
											{
												ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																								//文字列の分析
												sscanf(Read, "%s", &strLine);
												if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
												{
													//モーションの番号繰り上げ
													motionnumber++;
													key = 0;
													fclose(sta);
													break;
												}
												if (strcmp(&strLine[0], "NUM_KEY") == 0)
												{
													sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//キーの総数
												}
												else if (strcmp(&strLine[0], "LOOP") == 0)
												{//ループするかしないか
													sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//ループするかどうか
												}
												else if (strcmp(&strLine[0], "KEYSET") == 0)
												{
													while (fgets(Read, lenLine, sta) != nullptr)
													{
														ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット
	
														sscanf(Read, "%s", &strLine);	//文字列の分析

														//keyはモデルのキーの番号
														if (strcmp(&strLine[0], "END_KEYSET") == 0)
														{
															key++;
															m_pEnemy = 0;	//番号リセット
															break;
														}
														else if (strcmp(&strLine[0], "FRAME") == 0)
														{//キーの再生時間の設定
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[key].nFrame);	//再生時間の設定
														}
														else if (strcmp(&strLine[0], "COLLISIONSET") == 0)
														{
															m_apMotion[motionnumber].aModelKey[key].Collision[m_apMotion[motionnumber].aModelKey[key].nNumCollision]
																= CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																												//文字列の分析
																sscanf(Read, "%s", &strLine);

																//keyはモデルのキーの番号
																if (strcmp(&strLine[0], "END_COLLISIONSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumCollision++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//判定の開始時間
																	int start, select;
																	select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &start);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//判定の開始時間
																	int end;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &end);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	D3DXVECTOR3 dpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &dpos.x, &dpos.y, &dpos.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetDPos(dpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  dsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %f%f%f", &strLine, &dsiz.x, &dsiz.y, &dsiz.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetSiz(dsiz);
																}
															}
														}
														else if (strcmp(&strLine[0], "HURTSET") == 0)
														{
															m_apMotion[motionnumber].aModelKey[key].HurtCol[m_apMotion[motionnumber].aModelKey[key].nNumHurtCol]
																= CCollision::Create(m_pos, CCollision::COLLI_HURT);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																												//文字列の分析
																sscanf(Read, "%s", &strLine);

																//keyはモデルのキーの番号
																if (strcmp(&strLine[0], "END_HURTSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumHurtCol++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//判定の開始時間
																	int start, select;
																	select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &start);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//判定の開始時間
																	int end;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %d", &strLine, &end);	//ループするかどうか
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//現在の当たり判定の番号
																	D3DXVECTOR3 hpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &hpos.x, &hpos.y, &hpos.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetDPos(hpos);
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetPos(hpos);


																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  hsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//現在の当たり判定の番号
																	sscanf(Read, "%s = %f%f%f", &strLine, &hsiz.x, &hsiz.y, &hsiz.z);	//キーの総数
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetSiz(hsiz);
																}
															}
														}
														else if (strcmp(&strLine[0], "KEY") == 0)
														{//キー設定
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																												//文字列の分析
																sscanf(Read, "%s", &strLine);

																if (strcmp(&strLine[0], "END_KEY") == 0)
																{
																	m_pEnemy++;
																	break;
																}
																else if (strcmp(&strLine[0], "POS") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosX,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosY,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosZ);	//再生時間の設定
																}
																else if (strcmp(&strLine[0], "ROT") == 0)
																{
																	sscanf(Read, "%s = %f%f%f", &strLine,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fRotX,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fRotY,
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fRotZ);
																}
															}
														}
														else if (strcmp(&strLine[0], "HITSTOP") == 0)
														{//ヒットストップの時間設定
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nHitStopTimer);
														}
													}
												}
											}
											break;	//読み込みが終わったらループ脱出
										}
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "AXISSET") == 0)
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//文字列リセット

																			//文字列の分析
							sscanf(Read, "%s", &strLine);

							if (strcmp(&strLine[0], "END_AXIS") == 0)
							{
								break;
							}
							else if ((strcmp(&strLine[0], "POS") == 0))
							{
								D3DXVECTOR3 axpos;
								sscanf(Read, "%s = %f%f%f", &strLine, &axpos.x, &axpos.y, &axpos.z);	//キーの総数
								m_AxisBox->SetPos(axpos + m_pos);
								m_AxisBox->SetDPos(axpos);

							}
							else if ((strcmp(&strLine[0], "SIZ") == 0))
							{
								D3DXVECTOR3  axsiz;
								sscanf(Read, "%s = %f%f%f", &strLine, &axsiz.x, &axsiz.y, &axsiz.z);	//キーの総数
								m_AxisBox->SetSiz(axsiz);
							}
						}
					}

					else if (strcmp(&strLine[0], "#") == 0)
					{
						continue;
					}
				}
			}
			else if (strcmp(&strLine[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
		fclose(fp);
	}
}

//===========================
//モーション書き出し
//===========================
void CPlayer::WriteMotion(int nowmotion)
{
	const int lenLine = 2048;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	int nNumKey = 0;			//1モーションのキーの総数


	//ファイル読み込み
	FILE*fp = fopen("data/TXT/SaveMotion.txt", "w");		//ファイル読み込み
	if (fp == nullptr)
	{//開けなかった時用
		assert(false);
	}
	if (fp != NULL)
	{
		//文字列の分析
		fprintf(fp, "#========================================\n");
		fprintf(fp, "#  モーション情報\n");
		fprintf(fp, "#  [ モーション番号 : %d ]\n", (int)nowmotion);
		fprintf(fp, "#\n");
		fprintf(fp, "#========================================\n");
		fprintf(fp, "\n");
		fprintf(fp, "\n");
		fprintf(fp, "MOTIONSET\n");
		fprintf(fp, "	LOOP = %d		# ループするかどうか[0:ループしない / 1:ループする]\n", (int)m_apMotion[nowmotion].bLoop);
		fprintf(fp, "	NUM_KEY = %d		# キー数\n", (int)m_apMotion[nowmotion].nNumKey);
		fprintf(fp, "	HITSTOP = %d		# ヒットストップの時間\n", (int)m_apMotion[nowmotion].nHitStopTimer);
		fprintf(fp, "\n");

		for (int i = 0; i < (int)m_apMotion[nowmotion].nNumKey; i++)
		{
			fprintf(fp, "	KEYSET			# --- << KEY : %d / %d >> ---\n", i, m_apMotion[nowmotion].nNumKey);
			fprintf(fp, "		FRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].nFrame);
			//当たり判定の設定
			for (int j = 0; j < m_apMotion[nowmotion].aModelKey[i].nNumCollision; j++)
			{//当たり判定の出力
				fprintf(fp, "\n		#%d番目の当たり判定\n", j + 1);
				fprintf(fp, "			COLLISIONSET\n");
				fprintf(fp, "				STARTFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetStartf());				//発生
				fprintf(fp, "				ENDFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetEndf());					//終了
				fprintf(fp, "				DAMAGE = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetDMG());						//ダメージ量
				fprintf(fp, "				HIT = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetHitRig());						//ヒット硬直
				fprintf(fp, "				GUARD = %d\n", m_apMotion[nowmotion].aModelKey[m_nKEYData].Collision[j]->GetGuardRig());		//ガード硬直
				fprintf(fp, "				DAMAGE_POINT = %d\n", m_apMotion[nowmotion].aModelKey[m_nKEYData].Collision[j]->GetDp());		//技の属性
				fprintf(fp, "				POS = %.2f %.2f %.2f \n",																		//座標
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetPos().x,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetPos().y,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetPos().z);
				fprintf(fp, "				SIZ = %.2f %.2f %.2f \n",																	//大きさ
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetSiz().x,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetSiz().y,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetSiz().z);
				fprintf(fp, "			END_COLLISIONSET\n\n");
			}

			//やられ判定の設定
			for (int j = 0; j < m_apMotion[nowmotion].aModelKey[i].nNumHurtCol; j++)
			{//やられ判定の出力
				fprintf(fp, "		#%d番目のやられ判定\n", j + 1);
				fprintf(fp, "			HURTSET\n");
				fprintf(fp, "				STARTFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetStartf());	//発生
				fprintf(fp, "				ENDFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetEndf());		//終了
				fprintf(fp, "				POS = %.2f %.2f %.2f \n",																//座標
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetPos().x,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetPos().y,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetPos().z);
				fprintf(fp, "				SIZ = %.2f %.2f %.2f \n",																//大きさ
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetSiz().x,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetSiz().y,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetSiz().z);
				fprintf(fp, "			END_HURTSET\n\n");
			}

			for (int j = 0; j < NUM_PARTS; j++)
			{//キーの座標と方向
				fprintf(fp, "		KEY	# ----- [ %d ] -----\n", j);
				fprintf(fp, "				POS = %.2f %.2f %.2f \n",
					m_apMotion[nowmotion].aModelKey[i].aKey[j].fPosX,
					m_apMotion[nowmotion].aModelKey[i].aKey[j].fPosY,
					m_apMotion[nowmotion].aModelKey[i].aKey[j].fPosZ);

				fprintf(fp, "				ROT = %.2f %.2f %.2f \n",
					m_apMotion[nowmotion].aModelKey[i].aKey[j].fRotX,
					m_apMotion[nowmotion].aModelKey[i].aKey[j].fRotY,
					m_apMotion[nowmotion].aModelKey[i].aKey[j].fRotZ);
				fprintf(fp, "		END_KEY\n");
			}
			fprintf(fp, "		END_KEYSET\n");
		}
		fprintf(fp, "END_MOTIONSET");
		fprintf(fp, "\n");
		fclose(fp);
	}
}

//===========================
//回転テスト
//===========================
void CPlayer::rolling()
{
	//回転軸の設定
	m_vecAxis = D3DXVECTOR3(m_move.z, 0.0f, -m_move.x);

	//回転量の設定
	float Radius = m_apModel[0]->GetWidth();
	m_fRolling = D3DXVec3Length(&m_move) * 2 * D3DX_PI / (Radius *D3DX_PI);
}

//===========================
//モーション管理
//===========================
void CPlayer::MotionManager()
{
	if (m_pMotionOld != m_pMotion)
	{//状態が違う場合
		PlayFirstMotion();
	}
	SetFrame();		//プレイヤーのモーション

	m_pMotionOld = m_pMotion;
}

//===========================
//最初のモーションを設定
//===========================
void CPlayer::PlayFirstMotion()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//モーション再設定
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//差分の取得
	}
	m_nKEYData = 0;
	m_frame = 0;
}

//===========================
//エディットモード
//===========================
void CPlayer::EditMode()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//モーション再設定
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ) + m_apModel[i]->GetDPos() 
			+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fTempPos);	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ) + m_apModel[i]->GetDRot() 
			+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fTempRot);	//差分の取得
	}

	CDebugProc::Print("現在のモード:EDIT");
	char* cNowmodel;
	for (int i = 0; i < NUM_PARTS; i++)
	{
		if (m_nEditModel==i)
		{
			cNowmodel = "☆";
			m_apModel[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			m_apModel[i]->SetEmi(D3DXCOLOR(0.0f, 0.0f,0.0f, 1.0f));
		}
		else
		{
			cNowmodel = "・";
			m_apModel[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apModel[i]->SetEmi(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		}
		//角度の正規化
		Normalize(i);

		CDebugProc::Print("%s [%d]位置:%f,%f,%f 角度:%f,%f,%f", cNowmodel, i,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ);
	}

	m_nEditModel = m_nEditModel % NUM_PARTS;
	if (m_nEditModel < 0)
	{
		m_nEditModel = NUM_PARTS-1;
	}

	//モーション書き出し
	if (CApplication::GetInputKeyboard()->GetPress(DIK_P))
	{
		WriteMotion(m_pMotion);
	}
	
	//長押し時間のリセット
	if (m_nHold == m_nHoldold)
	{
		m_nHold = 0;
	}
	m_nHoldold = m_nHold;
}

//===========================
//角度の正規化
//===========================
void CPlayer::Normalize(int number)
{
	if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotX >= D3DX_PI)
	{
		m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotX -= D3DX_PI * 2;
	}
	else if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotX <= -D3DX_PI)
	{
		m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotX += D3DX_PI * 2;
	}
	if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotY >= D3DX_PI)
	{
		m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotY -= D3DX_PI * 2;
	}
	else if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotY <= -D3DX_PI)
	{
		m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotY += D3DX_PI * 2;
	}
	if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotZ >= D3DX_PI)
	{
		m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotZ -= D3DX_PI * 2;
	}
	else if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotZ <= -D3DX_PI)
	{
		m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[number].fRotZ += D3DX_PI * 2;
	}
}

//===========================
//プレイヤーの情報の設定(IMGUI)
//===========================
void CPlayer::PlayerStateDraw()
{
	int loop;
	ImGui::NewFrame();

	ImGui::Begin(u8"プレイヤーの情報");	//開始

	if (ImGui::CollapsingHeader(u8"プレイヤーの座標"))//タブ
	{//プレイヤーの座標
		ImGui::InputFloat("X", &m_TempPos.x, 0.1f, 0.1f, "%.2f");
		ImGui::InputFloat("Y", &m_TempPos.y, 0.1f, 0.1f, "%.2f");
		ImGui::InputFloat("Z", &m_TempPos.z, 0.1f, 0.1f, "%.2f");
		if (ImGui::Button("RESET"))
			m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);

		if (ImGui::Button("SET"))
			m_pos += m_TempPos, ZeroMemory(m_TempPos, sizeof(D3DXVECTOR3));
	}
	if (ImGui::CollapsingHeader(u8"プレイヤーの回転"))//タブ
	{//プレイヤーの座標
		ImGui::InputFloat("X", &m_TempRot.x, 0.01f, 0.01f, "%.2f");
		ImGui::InputFloat("Y", &m_TempRot.y, 0.01f, 0.01f, "%.2f");
		ImGui::InputFloat("Z", &m_TempRot.z, 0.01f, 0.01f, "%.2f");

		if (ImGui::Button("RESET"))
			m_rot = D3DXVECTOR3(0.f, 0.f, 0.f);

		if (ImGui::Button("SET"))
			m_rot += m_TempRot, ZeroMemory(m_TempRot, sizeof(D3DXVECTOR3));
	}
	if (ImGui::CollapsingHeader(u8"プレイヤーの押し出し判定の設定"))
	{
		D3DXVECTOR3 axispos = m_AxisBox->GetPos();
		D3DXVECTOR3 axissiz= m_AxisBox->GetSiz();
		ImGui::DragFloat3("Pos", axispos, -2.0f, 2.0f);
		ImGui::DragFloat3("Siz", axissiz, -2.0f, 2.0f);
		m_AxisBox->SetPos(axispos);
		m_AxisBox->SetSiz(axissiz);
		bool bSave = false;
		if (ImGui::Button("Save"))	//現在の軸判定の情報を保存
			bSave = true;	//セーブOnにする
		if (bSave == true)
		{//セーブするようにする
			SaveAxis();
			bSave = false;	//セーブOffに戻す
		}
	}
	if (ImGui::CollapsingHeader(u8"モーションの設定"))
	{
		ImGui::SliderInt(u8"モデル選択", &m_nEditModel, 0, NUM_PARTS - 1);										//編集するモデル
		ImGui::SliderInt(u8"再生するモーション", (int*)&m_pMotion, 0, PM_MAX - 1);								//再生しているモーション番号
		ImGui::SliderInt(u8"再生するキー", &m_nKEYData, 0, m_apMotion[m_pMotion].nNumKey - 1);					//再生するキーの番号
		ImGui::InputInt(u8"キーの数", &m_apMotion[m_pMotion].nNumKey, 1, 1);									//モーションのキーの数
		ImGui::SliderInt(u8"現在のフレーム",&m_frame,0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);	//今再生されているフレーム
		ImGui::InputInt(u8"フレーム数", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame, 1, 1);				//モーションのフレーム数
		ImGui::SliderInt(u8"ループするかどうか", (int*)&m_apMotion[m_pMotion].bLoop, 0,1);						//ループするかどうか設定
		ImGui::SliderInt(u8"ヒットストップ設定",&m_apMotion[m_pMotion].nHitStopTimer, 0, 60);					//ヒットストップの時間設定

		if (ImGui::TreeNode(u8"モデルの座標"))
		{ // モデルの座標編集

			ImGui::InputFloat("PosX", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.x, 0.1f, 0.1f, "%.2f");
			ImGui::InputFloat("PosY", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.y, 0.1f, 0.1f, "%.2f");
			ImGui::InputFloat("PosZ", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.z, 0.1f, 0.1f, "%.2f");
			ImGui::Text("Pos: (%.2f,%.2f,%.2f)",
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.x + m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.y + m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.z + m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ);
			ImGui::Text(u8"現在の座標: (%.2f,%.2f,%.2f)",
			m_apModel[m_nEditModel]->GetPos().x+ m_apModel[m_nEditModel]->GetDPos().x,
				m_apModel[m_nEditModel]->GetPos().y + m_apModel[m_nEditModel]->GetDPos().y,
				m_apModel[m_nEditModel]->GetPos().z+ m_apModel[m_nEditModel]->GetDPos().z);

			if (ImGui::Button("RESET"))
				ZeroMemory(m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos, sizeof(D3DXVECTOR3));

			if (ImGui::Button("SET"))
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX +=
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.x,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY +=
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.y,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ +=
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.z,
				ZeroMemory(m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos, sizeof(D3DXVECTOR3));

			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode(u8"モデルの回転"))
		{ // モデルの座標編集
			ImGui::DragFloat("RotX", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.x, 0.01f, 0.01f);
			ImGui::DragFloat("RotY", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.y, 0.01f, 0.01f);
			ImGui::DragFloat("RotZ", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.z, 0.01f, 0.01f);
			ImGui::Text("Rot: (%.2f,%.2f,%.2f)",
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.x+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotX,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.y+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotY,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.z+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotZ);


			if (ImGui::Button("RESET"))
				ZeroMemory(m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot, sizeof(D3DXVECTOR3));

			if (ImGui::Button("BASE RESET"))
				for (int i = 0; i < NUM_PARTS; i++)
				{
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX = 0.0f,
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY = 0.0f,
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ = 0.0f;
				}

			if (ImGui::Button("SET"))//仮設定の数値を代入
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotX +=
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.x,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotY +=
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.y,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotZ +=
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot.z,
				ZeroMemory(m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempRot, sizeof(D3DXVECTOR3));
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode(u8"当たり判定の設定"))
		{
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision < MAX_COLLISION-1)
			{
				D3DXVECTOR3 pos =
				D3DXVECTOR3(m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX,
							m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY,
							m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ);
				if (ImGui::Button("Create"))//判定生成
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision]
					= CCollision::Create(m_apModel[m_nEditModel]->GetPos()+pos,CCollision::COLLI_DAMAGE),				//当たり判定の生成
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision++;					//最大数をインクリメント
			}
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision > 0)
			{//当たり判定が存在する場合
				if (ImGui::Button("Delete"))//判定の削除
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision - 1]->Uninit(),
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision--;				//最大数をインクリメント

				ImGui::SliderInt(u8"設定する当たり判定の番号", &m_nSelectCollison, 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision - 1);	//モーション番号
				if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision > 0)
				{//当たり判定が存在する場合
					if (m_nSelectCollison >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision)
					{
						m_nSelectCollison = 0;
					}
					if (ImGui::TreeNode(u8"当たり判定"))
					{ // モデルの座標編集
						m_ColiPos = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetPos();
						m_siz = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetSiz();
						int start = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetStartf();
						int end = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetEndf();
						int damage = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetDMG();
						int hitrig = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetHitRig();
						int guardrig = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetGuardRig();
						int dp = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetDp();

						ImGui::DragFloat3("Pos", m_ColiPos,1.0f,-300.0f, 300.0f, "%.2f");								//座標
						ImGui::DragFloat3("Siz", m_siz,  1.0f, -300.0f, 300.0f, "%.2f");								//大きさ
						ImGui::SliderInt("Start", &(start),0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);		//フレーム
						ImGui::SliderInt("End", &(end), 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);			//モーション番号
						ImGui::InputInt("Damage",&(damage) , 0, 1000);													//ダメージ量
						ImGui::SliderInt(u8"ヒット硬直設定", &hitrig, -60, 60);											//ヒット硬直の設定
						ImGui::SliderInt(u8"ガード硬直設定", &guardrig, -60, 60);										//ガード硬直の設定
						ImGui::SliderInt(u8"ダメージ属性", &dp, 0, 3);													//ダメージ属性設定

						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetPos(m_ColiPos);		//位置情報更新
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetSiz(m_siz);			//サイズ
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetStartf(start);			//開始フレーム
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetEndf(end);				//終了フレーム
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetDMG(damage);				//ダメージ量
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetHitRig(hitrig);		//ヒット硬直
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetGuardRig(guardrig);	//ガード硬直
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetDp((CCollision::EDAMAGE_POINT)dp);

						ImGui::TreePop();
						ImGui::Separator();
					}
				}
			}
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode(u8"やられ判定の設定"))
		{
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol < MAX_COLLISION - 1)
			{
				if (ImGui::Button("Create"))//判定生成
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol]
					= CCollision::Create(m_apModel[m_nEditModel]->GetPos(), CCollision::COLLI_HURT),		//やられ判定の生成
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol++;							//最大数をインクリメント
			}

			if (ImGui::Button(u8"やられ判定のコピー"))
				for (int i = 0; i <MAX_COLLISION; i++)
				{
					if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i] != nullptr)
					{
						m_CopyCollision[i] = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i];
					}
				};

			if (ImGui::Button(u8"やられ判定のペースト"))
				for (int i = 0; i < MAX_COLLISION; i++)
				{
					if (m_CopyCollision[i] != nullptr)
					{
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol] = m_CopyCollision[i];
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol++;							//最大数をインクリメント
					}
				};
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol > 0)
			{//やられ判定が存在する場合
				if (ImGui::Button("Delete"))//判定の削除
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol - 1]->Uninit(),
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol--;				//最大数をインクリメント

				ImGui::SliderInt(u8"設定するやられ判定の番号", &m_nSelectHurt, 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol - 1);	//モーション番号
				if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol > 0)
				{//当たり判定が存在する場合
					if (m_nSelectHurt >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol)
					{
						m_nSelectHurt = 0;
					}

					if (ImGui::TreeNode(u8"やられ判定"))
					{ // モデルの座標編集
						m_ColiPos = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetPos();
						m_siz = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetSiz();
						int start = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetStartf();
						int end = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetEndf();
						int damage = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetDMG();
						ImGui::DragFloat3("Pos", m_ColiPos, 1.0f, -300.0f, 300.0f, "%.2f");								//座標
						ImGui::DragFloat3("Siz", m_siz, 1.0f, -300.0f, 300.0f, "%.2f");									//大きさ
						ImGui::SliderInt("Start", &(start), 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);		//フレーム
						ImGui::SliderInt("End", &(end), 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);			//終了フレーム


						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetPos(m_ColiPos);	//位置情報更新
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetSiz(m_siz);		//サイズ
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetStartf(start);		//開始フレーム
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetEndf(end);			//終了フレーム
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetDMG(damage);		//ダメージ設定

						bool bSave = false;
						if (ImGui::Button("Save"))
							bSave = true;
						if (bSave == true)
						{
							FILE* fp = fopen("data/TXT/SaveHurtCollision.txt", "w");
							//ファイル書き出し
							for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol; i++)
							{//当たり判定の出力
								fprintf(fp, "		#%d番目の判定\n\n", i + 1);

								fprintf(fp, "			COLLISIONSET\n");
								fprintf(fp, "				POS = %.2f %.2f %.2f \n",																//座標
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetPos().x,
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetPos().y,
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetPos().z);
								fprintf(fp, "				SIZ = %.2f %.2f %.2f \n",																//大きさ
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetSiz().x,
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetSiz().y,
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetSiz().z);
								fprintf(fp, "			END_COLLISIONSET\n");
							}
							fclose(fp);
							bSave = false;
						}
						ImGui::TreePop();
						ImGui::Separator();
					}
				}
			}
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::Button(u8"キーのコピー"))
			m_CopyKey = m_apMotion[m_pMotion].aModelKey[m_nKEYData];
		if (ImGui::Button(u8"キーのペースト"))
			m_apMotion[m_pMotion].aModelKey[m_nKEYData] = m_CopyKey;
		if (ImGui::Button(u8"現在のモーションのデータ保存"))
				WriteMotion(m_pMotion);

	}
	SetFrame();
	ImGui::End();	//終了
}

//===========================
//当たり判定を表示するかどうか
//===========================
void CPlayer::DrawCollision()
{
	//判定の表示
	for (int i = 0; i < PM_MAX; i++)
	{
		for (int k = 0; k < m_apMotion[i].nNumKey; k++)
		{
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumCollision; j++)
			{//違うモーションの当たり判定をオフにする
				if (m_pMotion == i&&m_nKEYData == k&&m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
				{//キーとモーションが一致している場合のみ表示{//当たり判定
					m_apMotion[i].aModelKey[k].Collision[j]->SetUse(true);
					if (m_bPlay == true)
					{
						m_apMotion[i].aModelKey[k].Collision[j]->SetPos(m_pos + m_apMotion[i].aModelKey[k].Collision[j]->GetDPos());
					}
				}
				else
				{
					if (m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
					{
						m_apMotion[i].aModelKey[k].Collision[j]->SetUse(false);
					}
				}
			}
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumHurtCol; j++)
			{//やられ判定
				if (m_pMotion == i&&m_nKEYData == k&&m_apMotion[i].aModelKey[k].HurtCol[j] != nullptr)
				{
					m_apMotion[i].aModelKey[k].HurtCol[j]->SetUse(true);
					if (m_bPlay == true)
					{
						m_apMotion[i].aModelKey[k].HurtCol[j]->SetPos(m_apMotion[i].aModelKey[k].HurtCol[j]->GetDPos() + m_pos);
					}
				}
				else
				{
					if (m_apMotion[i].aModelKey[k].HurtCol[j] != nullptr)
					{
						m_apMotion[i].aModelKey[k].HurtCol[j]->SetUse(false);
					}
				}
			}
		}

	}
}

//===========================
//フレーム設定
//===========================
void CPlayer::SetFrame()
{
	D3DXVECTOR3 RelaPos, RelaRot;		//1フレームごとの移動量
	D3DXVECTOR3 pos, rot, DiffPos, DiffRot;
		CDebugProc::Print("現在のフレーム:%d", m_frame);

	//カウンター更新
	if (m_nKEYData == m_apMotion[m_pMotion].nNumKey&& m_apMotion[m_pMotion].bLoop == false)
	{
		if (m_bPlay == true)
		{//エディットじゃないときループモーションが終わったらニュートラルにする
			m_pMotion = PM_ST_NEUTRAL;
			PlayFirstMotion();
		}
		return;
	}
	else
	{
		for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision; i++)
		{//ダメージ判定が出現するかどうか(1～4フレームまでダメージ判定を出すみたいな)
			if (m_frame>= m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetStartf())
			{//開始フレーム以上かどうか
				if (m_frame <= m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetEndf())
				{//終了フレーム以下かどうか
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->SetUse(true);
				}
				else
				{
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->SetUse(false);
				}
			}
			else
			{
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->SetUse(false);
			}
		}
		for (int i = 0; i < NUM_PARTS; i++)
		{//パーツ全部のモーション再生
			if (m_apModel[i] != nullptr)
			{
				if (m_nKEYData == m_apMotion[m_pMotion].nNumKey - 1 && m_apMotion[m_pMotion].bLoop == true)
				{//ループする場合最初のモーションに移行する
					DiffPos = D3DXVECTOR3(
						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosZ - m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ);	//差分の取得

					DiffRot = D3DXVECTOR3(
						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ);	//差分の取得
				}
				else if (m_nKEYData == m_apMotion[m_pMotion].nNumKey - 1 && m_apMotion[m_pMotion].bLoop == false)
				{//ループしない場合ニュートラルモーションに戻す
					DiffPos = D3DXVECTOR3(//座標差分の取得
						m_apMotion[0].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,

						m_apMotion[0].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,

						m_apMotion[0].aModelKey[0].aKey[i].fPosZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//向き差分の取得
						m_apMotion[0].aModelKey[0].aKey[i].fRotX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,

						m_apMotion[0].aModelKey[0].aKey[i].fRotY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,

						m_apMotion[0].aModelKey[0].aKey[i].fRotZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ);
				}
				else
				{
					if (m_nKEYData == 0 && m_frame == 0)
					{//位置座標の設定しなおし
						PlayFirstMotion();
					}
					DiffPos = D3DXVECTOR3(//座標差分の取得
						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fPosX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fPosY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fPosZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//向き差分の取得
						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fRotX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fRotY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fRotZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ);
				}
			}

			//位置
			RelaPos = DiffPos / m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame;		//相対値
			RelaRot = DiffRot / m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame;

			//キーの設定
			if (m_bPlay == true)
			{//再生モードの場合
				pos = RelaPos + m_apModel[i]->GetPos();
				rot = RelaRot + m_apModel[i]->GetRot();
			}
			else
			{//編集モードの場合
				pos = RelaPos*m_frame + m_apModel[i]->GetPos();
				rot = RelaRot*m_frame + m_apModel[i]->GetRot();
			}

			//位置の設定
			m_apModel[i]->SetPos(pos);
			m_apModel[i]->SetRot(rot);
		}
	}
	if (m_bPlay == true)
	{//再生モードの場合
	 //カウンター更新

		if (m_frame >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame)
		{//キー番号の更新とカウンターのリセット
			m_nKEYData++;
			m_frame = 0;
			if (m_nKEYData >= m_apMotion[m_pMotion].nNumKey)
			{//キー番号が最大数を超えた場合リセット
				if (m_apMotion[m_pMotion].bLoop == true)
				{
					m_nKEYData = 0;
				}
			}
		}
		m_frame++;
	}
}

//===========================
//軸の判定の保存
//===========================
void CPlayer::SaveAxis()
{
	FILE* fp = fopen("data/TXT/SaveAxis.txt", "w");
	//ファイル読み込み 
	fprintf(fp, "軸の判定\n\n");

	fprintf(fp, "	POS = %.2f %.2f %.2f \n",
		m_AxisBox->GetPos().x,
		m_AxisBox->GetPos().y,
		m_AxisBox->GetPos().z);
	fprintf(fp, "	SIZ = %.2f %.2f %.2f \n",
		m_AxisBox->GetSiz().x,
		m_AxisBox->GetSiz().y,
		m_AxisBox->GetSiz().z);
	fclose(fp);
}

//===========================
//当たり判定の保存
//===========================
void CPlayer::SaveCollision()
{
	FILE* fp = fopen("data/TXT/SaveCollision.txt", "w");
	//ファイル書き出し
	for (int j = 0; j < m_apMotion[m_pMotion].nNumKey; j++)
	{//このモーションの全てのキーを保存
		for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision; i++)
		{//当たり判定の出力
			fprintf(fp, "#%d番目の判定\n\n", i + 1);

			fprintf(fp, "	COLLISIONSET\n");
			fprintf(fp, "		STARTFRAME = %d\n", m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetStartf());	//発生
			fprintf(fp, "		ENDFRAME = %d\n", m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetEndf());		//終了
			fprintf(fp, "		POS = %.2f %.2f %.2f \n",																//座標
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetPos().x,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetPos().y,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetPos().z);
			fprintf(fp, "		SIZ = %.2f %.2f %.2f \n",																//大きさ
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetSiz().x,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetSiz().y,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetSiz().z);
			fprintf(fp, "	END_COLLISIONSET\n");
		}
	}
	fclose(fp);
}

//===========================
//やられ判定の設定
//===========================
void CPlayer::SetHitBox()
{
	for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol; i++)
	{//ダメージ判定が出現するかどうか(1～4フレームまでダメージ判定を出すみたいな)
		if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i] != nullptr)
		{
			if (m_frame >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetStartf())
			{//開始フレーム以上かどうか
				if (m_frame <= m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetEndf())
				{//終了フレーム以下かどうか
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->SetUse(true);
				}
				else
				{
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->SetUse(false);
				}
			}
			else
			{
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->SetUse(false);
			}
		}
	}
}
