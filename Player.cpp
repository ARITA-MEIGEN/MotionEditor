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
#include"Line.h"

//マクロ定義
#define PLAYER_SPEED	(1.0f)	//移動速度
#define MIN_SPEED		(0.1f)	//摩擦による最低速度

//===========================
//コンストラクタ
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
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
	m_pState = P_STAND;
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
			m_apMotion[m_pState].aModelKey[m_nKEYData].nNumCollision = 0;
			m_apMotion[m_pState].aModelKey[m_nKEYData].nFrame = 0;
		}
	}
	
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
	if (m_bPlay == true)
	{
		m_posold = m_pos;		//前回の位置の保存
		m_pos += m_move;		//位置の更新
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//位置の更新

		if (ControlPlayer() == true)		//操作
		{
			//m_pState = P_STAND;	//操作されていない場合ニュートラルに
		}

		if (CGame::GetMesh() != nullptr)
		{
			m_pos.y = CGame::GetMesh()->Collision(m_pos);
		}


		CDebugProc::Print("現在のモード:PLAY");

		MotionManager();

		CDebugProc::Print("現在のモード:%d　", (int)m_pState);
	}

	if (m_bPlay == false)
	{
		EditMode();
	}

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_F2))
	{//モード切り替え
		m_bPlay = !m_bPlay;
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//パーツのモデルの位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
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
		m_pState = P_CROUCH;
	}
	if (m_pState != P_CROUCH)
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			m_move.x = sinf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pState = P_MOVE;
			//m_rotDest.y = D3DX_PI*1.0f + pCamera->Getrot().y;

			bNeutral = false;
		}
		if (pKeyboard->GetPress(DIK_DOWN))
		{
			m_move.x = sinf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pState = P_MOVE;
			//m_rotDest.y = D3DX_PI*0.0f + pCamera->Getrot().y;
			bNeutral = false;

		}
		if (pKeyboard->GetPress(DIK_LEFT))
		{
			m_move.x = sinf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pState = P_MOVE;
			//m_rotDest.y = D3DX_PI*0.5f + pCamera->Getrot().y;
			bNeutral = false;

		}
		if (pKeyboard->GetPress(DIK_RIGHT))
		{
			m_move.x = sinf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_move.z = cosf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
			m_pState = P_MOVE;
			//m_rotDest.y = D3DX_PI*-0.5f + pCamera->Getrot().y;
			bNeutral = false;

		}
	}

	 if (pKeyboard->GetTrigger(DIK_RETURN))
	 {
		 m_pState = P_LATTACK_STAND;
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
	const int lenLine = 2048;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	char Read[lenLine];			//読み取る用
	int	modelnumber = 0;		//モデルの番号
	int motionnumber = 0;		//モーションの番号
	int nNumKey = 0;			//1つのモーションのキーの総数
	int key = 0;
	int model = 0;
	int Idx = 0;


	//ファイル読み込み
	FILE*fp = fopen("data/TXT/ReadMotion.txt", "r");		//ファイル読み込み
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
					ZeroMemory(strLine,sizeof(char)* 2048);	//文字列リセット

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

						m_apModel[modelnumber]->SetModel(&m_nModelpass[0]);
						modelnumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//初期位置の設定
						while (fgets(Read, lenLine, fp) != nullptr)//一行読み込み
						{//キャラクターの設定
							//文字列の分析
							ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//読み込みループ //一行読み込み
								{//パーツの設定	
									ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

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
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//読み込み用ループ
						{
							ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

							//文字列の分析
							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
							{
								//モーションの番号繰り上げ
								motionnumber++;
								key = 0;
								break;
							}
							else if (strcmp(&strLine[0], "NUM_KEY") == 0)
							{
								sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//キーの総数
							}
							else if (strcmp(&strLine[0], "LOOP") == 0)
							{//ループするかしないか
								sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//ループするかどうか
							}
							else if (strcmp(&strLine[0], "KEYSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)
								{
									ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

									//文字列の分析
									sscanf(Read, "%s", &strLine);

									//keyはモデルのキーの番号
									if (strcmp(&strLine[0], "END_KEYSET") == 0)
									{
										key++;
										model = 0;	//番号リセット
										break;
									}
									else if (strcmp(&strLine[0], "FRAME") == 0)
									{//キーの再生時間の設定
										sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[key].nFrame);	//再生時間の設定
									}
									else if (strcmp(&strLine[0], "KEY") == 0)
									{//キー設定
										while (fgets(Read, lenLine, fp) != nullptr)
										{
											ZeroMemory(strLine, sizeof(char) * 2048);	//文字列リセット

											//文字列の分析
											sscanf(Read, "%s", &strLine);

											if (strcmp(&strLine[0], "END_KEY") == 0)
											{
												model++;
												break;
											}
											else if (strcmp(&strLine[0], "POS") == 0)
											{
												sscanf(Read, "%s = %f%f%f", &strLine,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosX,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosY,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosZ);	//再生時間の設定
											}
											else if (strcmp(&strLine[0], "ROT") == 0)
											{
												sscanf(Read, "%s = %f%f%f", &strLine,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fRotX,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fRotY,
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fRotZ);
											}
										}
									}
								}
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
	}
	fclose(fp);
}

//===========================
//モーション書き出し
//===========================
void CPlayer::WriteMotion(int nowmotion)
{
	const int lenLine = 2048;	//1単語の最大数
	char strLine[lenLine];		//読み込み用の文字列
	char Read[lenLine];			//読み取る用
	int	modelnumber = 0;		//モデルの番号
	int motionnumber = 0;		//モーションの番号
	int nNumKey = 0;			//1モーションのキーの総数
	int key = 0;
	int model = 0;
	int Idx = 0;


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
		fprintf(fp, "\n");

		for (int i = 0; i < (int)m_apMotion[nowmotion].nNumKey; i++)
		{
			fprintf(fp, "	KEYSET			# --- << KEY : %d / %d >> ---\n", i, m_apMotion[0].nNumKey);
			fprintf(fp, "		FRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].nFrame);
			for (int j = 0; j < NUM_PARTS; j++)
			{
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
//パーツのモーション再生
//===========================
void CPlayer::MotionPlayer(int nNumber)
{
	D3DXVECTOR3 pos, rot, fDiffPos, fDiffRot;
	D3DXVECTOR3 fRelaPos, fRelaRot;		//1フレームごとの移動量
	CDebugProc::Print("現在のキー:%d", m_nCurrentKey);
	CDebugProc::Print("現在のフレーム:%d", m_MotionCnt);

	//カウンター更新
	if (m_nCurrentKey == m_apMotion[nNumber].nNumKey-1&&m_apMotion[nNumber].bLoop == false)
	{
		if (m_bPlay == true)
		{//エディットじゃないときループモーションが終わったらニュートラルにする
			m_pState = P_STAND;
			PlayFirstMotion();
		}
		return;
	}
	else 
	{
		if (m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame == 0)
		{
			m_nCurrentKey = 0;
		}

		for (int i = 0; i < NUM_PLAYERPARTS; i++)
		{//パーツ全部のモーション再生
			if (m_apModel[i] != nullptr)
			{
				if (m_nCurrentKey == m_apMotion[nNumber].nNumKey - 1&& m_apMotion[nNumber].bLoop == true)
				{//ループする場合最初のモーションに移行する
					fDiffPos = D3DXVECTOR3(
						m_apMotion[nNumber].aModelKey[0].aKey[i].fPosX - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fPosY -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fPosZ - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	//差分の取得

					fDiffRot = D3DXVECTOR3(
						m_apMotion[nNumber].aModelKey[0].aKey[i].fRotX - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fRotY -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fRotZ - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	//差分の取得
				}
				
				else
				{
					if (m_nCurrentKey == 0&& m_MotionCnt==0)
					{//位置座標の設定しなおし
						PlayFirstMotion();
					}
					fDiffPos = D3DXVECTOR3(//座標差分の取得
						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosX - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosY - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosZ -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	

					fDiffRot = D3DXVECTOR3(//向き差分の取得
						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotX -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotY - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotZ -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	
				}

				//位置
				fRelaPos = fDiffPos / m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame;		//相対値
				fRelaRot = fDiffRot / m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame;

				//キーの設定
				pos = fRelaPos + m_apModel[i]->GetPos();
				rot = fRelaRot + m_apModel[i]->GetRot();

				//位置の設定
				m_apModel[i]->SetPos(pos);
				m_apModel[i]->SetRot(rot);
			}
		}
		//カウンター更新
		if (m_MotionCnt >= m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame)
		{//キー番号の更新とカウンターのリセット
			m_nCurrentKey++;
			m_MotionCnt = 0;
			if (m_nCurrentKey >= m_apMotion[nNumber].nNumKey)
			{//キー番号が最大数を超えた場合リセット
				if (m_apMotion[nNumber].bLoop == true)
				{
					m_nCurrentKey = 0;
				}
			}
		}
		m_MotionCnt++;

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
	if (m_pStateOld != m_pState)
	{//状態が違う場合
		PlayFirstMotion();
	}
	MotionPlayer(m_pState);		//プレイヤーのモーション

	m_pStateOld = m_pState;
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
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//差分の取得
	}
	m_nCurrentKey = 0;
	m_MotionCnt = 0;
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
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//初期位置の設定

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//差分の取得
	}

	CDebugProc::Print("現在のモード:EDIT");

	CDebugProc::Print("現在再生されてるモーション 3/4:%d　", (int)m_pState);
	CDebugProc::Print("現在のキー 5/6:%d　", m_nKEYData);

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
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosZ,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotZ);
	}

	CDebugProc::Print("位置の設定 X:Y/T Y:H/G Z:N/B");
	CDebugProc::Print("角度の設定 X:NUM8/NUM7 Y:NUM5/NUM4 Z:NUM2/NUM1");

	//X座標
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX, SINGLE_STA, HOLD_STA,DIK_Y, DIK_T);

	//Y座標
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY, SINGLE_STA, HOLD_STA, DIK_H, DIK_G);

	//Y座標
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ, SINGLE_STA, HOLD_STA, DIK_N, DIK_B);

	//X方向
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotX = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotX, SINGLE_STA, HOLD_STA,DIK_NUMPAD8, DIK_NUMPAD7);

	//Y方向
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotY =
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotY, SINGLE_STA, HOLD_STA, DIK_NUMPAD5, DIK_NUMPAD4);

	//Z方向
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotZ = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotZ, SINGLE_STA, HOLD_STA, DIK_NUMPAD2, DIK_NUMPAD1);

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_4))
	{//再生モーション変更
		m_pState = static_cast<PLAYER_STATE>(m_pState + 1);

		if (m_pState == P_MAX)
		{//最大値の場合最低値に戻す
			m_pState = P_STAND;
		}
	}
	else if (CApplication::GetInputKeyboard()->GetTrigger(DIK_3))
	{//再生モーション変更
		if (m_pState != P_STAND)
		{
			m_pState = static_cast<PLAYER_STATE>(m_pState - 1);
		}
		else
		{//最低値の場合最大値に戻す
			m_pState = static_cast<PLAYER_STATE>(P_MAX - 1);
		}
	}

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_9))
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].Collisionbox[m_apMotion[m_pState].aModelKey[m_nKEYData].nNumCollision]->Create(
			m_apModel[m_nEditModel]->GetDPos()+ m_apModel[m_nEditModel]->GetPos(),
			m_apModel[m_nEditModel]->GetDRot()+ m_apModel[m_nEditModel]->GetRot());
		m_apMotion[m_pState].aModelKey[m_nKEYData].nNumCollision++;
	}


	//キーの変更
	m_nKEYData = State<int, int>(m_nKEYData, 1, 1, DIK_6, DIK_5);
		if (m_nKEYData < 0)
		{
			m_nKEYData = m_apMotion[m_pState].nNumKey-1;
		}
		if (m_nKEYData>=m_apMotion[m_pState].nNumKey)
		{
			m_nKEYData = 0;
		}

	//編集パーツの変更
	m_nEditModel = State<int, int>(m_nEditModel, 1, 1, DIK_DOWN, DIK_UP);

	m_nEditModel = m_nEditModel % NUM_PARTS;
	if (m_nEditModel < 0)
	{
		m_nEditModel = NUM_PARTS-1;
	}

	//モーション書き出し
	if (CApplication::GetInputKeyboard()->GetPress(DIK_P))
	{
		WriteMotion(m_pState);
	}
	
	//長押し時間のリセット
	if (m_nHold== m_nHoldold)
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
	if (m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotX >= D3DX_PI)
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotX -= D3DX_PI * 2;
	}
	else if (m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotX <= -D3DX_PI)
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotX += D3DX_PI * 2;
	}
	if (m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotY >= D3DX_PI)
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotY -= D3DX_PI * 2;
	}
	else if (m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotY <= -D3DX_PI)
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotY += D3DX_PI * 2;
	}
	if (m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotZ >= D3DX_PI)
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotZ -= D3DX_PI * 2;
	}
	else if (m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotZ <= -D3DX_PI)
	{
		m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[number].fRotZ += D3DX_PI * 2;
	}
}

//===========================
//当たり判定の設定
//===========================
void CPlayer::SetCollision()
{

}

