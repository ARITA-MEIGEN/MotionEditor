//=================================================
// Content     (�Q�[���̐ݒ�)(player.cpp)
// Author     : �L�c����
//=================================================

//�C���N���[�h
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

//�}�N����`
#define PLAYER_SPEED	(2.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x

int CPlayer::m_snPlayernumber = 0;

//===========================
//�R���X�g���N�^
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
	m_nPlayernumber = m_snPlayernumber;
	m_snPlayernumber++;
}

//===========================
//�f�X�g���N�^
//===========================
CPlayer::~CPlayer()
{

}

//===========================
//����������
//===========================
HRESULT CPlayer::Init()
{
	m_pMotion = PM_ST_NEUTRAL;
	m_nKEYData = 0;
	m_bPlay = false;

	for (int i = 0; i < NUM_PARTS; i++)
	{//�v���C���[�̐���
		m_apModel[i] = CModel::Create();
	}

	//�G���[�h�~�p�ɑS�Ă̐��l��0������
	for (int i = 0; i < MAX_MOTION; i++)
	{//���[�V�����̏�����
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
			m_apMotion[i].aModelKey[j].nNumCollision = 0;	//�_���[�W����̐���������
			m_apMotion[i].aModelKey[j].nNumHurtCol = 0;		//���ꔻ��̐���������
			m_apMotion[i].aModelKey[j].nFrame = 1;			//�L�[�̍Đ����Ԃ�1�ɐݒ�
		}
		m_apMotion[i].nHitStopTimer = 0;	//�q�b�g�X�g�b�v������

	}

	m_AxisBox = CCollision::Create(m_pos,CCollision::COLLI_AXIS);						//�����o������(�v���C���[�̎�)
	
	//���f���ƃ��[�V�����̓ǂݍ���
	ReadMotion();

	for (int i = 0; i < NUM_PARTS; i++)
	{
		//�v���C���[�̐���
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fPosX,
			m_apMotion[0].aModelKey[0].aKey[i].fPosY,
			m_apMotion[0].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[0].aModelKey[0].aKey[i].fRotX,
			m_apMotion[0].aModelKey[0].aKey[i].fRotY,
			m_apMotion[0].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}

	//��]�}�g���b�N�X�̏�����
	//D3DXMatrixIdentity(&m_mtxRot);

	//����̍��W�ݒ�
	for (int i = 0; i < PM_MAX; i++)
	{
		for (int k = 0; k < m_apMotion[i].nNumKey; k++)
		{
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumCollision; j++)
			{//�Ⴄ���[�V�����̓����蔻����I�t�ɂ���
				if (m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
				{//�L�[�ƃ��[�V��������v���Ă���ꍇ�̂ݕ\��{//�����蔻��
					m_apMotion[i].aModelKey[k].Collision[j]->SetPos(m_pos + m_apMotion[i].aModelKey[k].Collision[j]->GetDPos());
				}
			}
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumHurtCol; j++)
			{//���ꔻ��
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
//�I������
//===========================
void CPlayer::Uninit(void)
{

	for (int i = 0; i < NUM_PARTS; i++)
	{//�v���C���[�̐���
		m_apModel[i]->Uninit();
		delete m_apModel[i];
		m_apModel[i] = nullptr;
	}

	CObject::Release();
}

//===========================
//�X�V����
//===========================
void CPlayer::Update(void)
{
	DrawCollision();

	if (m_bPlay == true)
	{
		m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
		m_pos += m_move;		//�ʒu�̍X�V
		m_AxisBox->SetPos(m_move + m_AxisBox->GetPos());
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̍X�V

		if (ControlPlayer() == true)				//����
		{
			//m_pMotion = PM_STAND;	//���삳��Ă��Ȃ��ꍇ�j���[�g������
		}

		CDebugProc::Print("���݂̃��[�h:PLAY");

		MotionManager();

		CDebugProc::Print("���݂̃��[�h:%d�@", (int)m_pMotion);
	}
	else 
	{
		EditMode();
		PlayerStateDraw();
	}
	SetHitBox();

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_F2))
	{//���[�h�؂�ւ�
		m_bPlay = !m_bPlay;
		m_pMotion = PM_ST_NEUTRAL;
		m_frame = 0;
		m_nKEYData = 0;
	}

	CDebugProc::Print("���݂̃v���C���[�̍��W:%f %f %f", m_pos.x, m_pos.y, m_pos.z);
}

//===========================
//�`�揈��
//===========================
void CPlayer::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;	//�f�o�C�X�ւ̃|�C���^
	pDevice = CApplication::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;	//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;			//���݂̃}�e���A���ۑ��p

	//���݂̃}�e���A�����ێ�
	pDevice->GetMaterial(&matDef);

	//�p�[�c�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);

	//�p�[�c�̃��f���̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y+ m_TempRot.y, m_rot.x + m_TempRot.x, m_rot.z + m_TempRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�p�[�c�̃��f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_TempPos.x, m_pos.y + m_TempPos.y, m_pos.z + m_TempPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//Shadow();

	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	for (int i = 0; i < NUM_PARTS; i++)
	{
		m_apModel[i]->Draw(m_mtxWorld);
	}
}

//===========================
//����
//===========================
bool CPlayer::ControlPlayer(void)
{
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();
	bool bNeutral = true;

	//�ړ�
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

	//�p�x�̐��K��
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
//����
//===========================
CPlayer * CPlayer::Create()
{
	CPlayer*pPlayer;
	pPlayer = new CPlayer(CObject::OBJTYPE_MODEL);
	pPlayer->Init();

	return pPlayer;
}

//===========================
//�}�g���b�N�X�̎擾
//===========================
D3DXMATRIX CPlayer::GetMtx()
{
	return m_mtxWorld;
}

//===========================
//���[�V�����ǂݍ���
//===========================
void CPlayer::ReadMotion()
{
	//�t�@�C���ǂݍ���
	const int lenLine = 2048;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	char Read[lenLine];			//�ǂݎ��p
	int	m_pEnemynumber = 0;		//���f���̔ԍ�
	int motionnumber = 0;		//���[�V�����̔ԍ�
	int key = 0;
	int m_pEnemy = 0;
	int Idx = 0;

	//�t�@�C���ǂݍ���
	FILE*fp = fopen("data/TXT/ReadEditMotion.txt","r");		//�t�@�C���ǂݍ���
	if (fp == nullptr)
	{//�J���Ȃ��������p
		assert(false);
	}
	if (fp != NULL)
	{
		while (fgets(Read, lenLine, fp) != nullptr)
		{
			//������̕���
			sscanf(Read, "%s", &strLine);
			if (strcmp(&strLine[0], "SCRIPT") == 0)	//��r���Ĉ�v���邩�ǂ������ׂ�
			{
				while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v(��s�ǂݍ���)
				{//���f���̏����ݒ�
					ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																	//������̕���
					sscanf(Read, "%s", &strLine);

					if (strcmp(&strLine[0], "END_SCRIPT") == 0)
					{
						break;
					}
					else if (strcmp(&strLine[0], "NUM_MODEL") == 0)
					{
						sscanf(Read, "%s = %d", &strLine, &m_nNumModel);	//�ǂݍ��񂾕������Ƃɐݒ肷��
					}
					else if (strcmp(&strLine[0], "MODEL_FILENAME") == 0)
					{
						sscanf(Read, "%s = %s", &strLine, &m_nModelpass[0]);	//���f���̃p�X�̐ݒ�

						m_apModel[m_pEnemynumber]->SetModel(&m_nModelpass[0]);
						m_pEnemynumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//�����ʒu�̐ݒ�
						while (fgets(Read, lenLine, fp) != nullptr)//��s�ǂݍ���
						{//�L�����N�^�[�̐ݒ�
						 //������̕���
							ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��݃��[�v //��s�ǂݍ���
								{//�p�[�c�̐ݒ�	
									ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																					//������̕���
									sscanf(Read, "%s", &strLine);
									if (strcmp(&strLine[0], "END_PARTSSET") == 0)
									{//�p�[�c�̐ݒ�I��
										Idx++;
										break;
									}
									else if (strcmp(&strLine[0], "INDEX") == 0)
									{//�C���f�b�N�X�̐ݒ�
										sscanf(Read, "%s = %d", &strLine, &Idx);	//���f���̃p�X�̐ݒ�
									}
									else if (strcmp(&strLine[0], "PARENT") == 0)
									{//�e���f���̐ݒ�
										int Parent;
										sscanf(Read, "%s = %d", &strLine, &Parent);	//���f���̃p�X�̐ݒ�
										m_apModel[Idx]->SetParent(m_apModel[Parent]);
									}
									else if (strcmp(&strLine[0], "POS") == 0)
									{//�ʒu
										D3DXVECTOR3 pos;
										sscanf(Read, "%s = %f%f%f", &strLine, &pos.x, &pos.y, &pos.z);	//���W�̎擾
										m_apModel[Idx]->SetPos(pos);
										m_apModel[Idx]->SetDPos(pos);
									}
									else if (strcmp(&strLine[0], "ROT") == 0)
									{//����
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
					{//���[�V�����̐ݒ�
						while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																			//������̕���
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
								sscanf(Read, "%s = %s", &strLine, &cFilename[0]);	//�t�@�C���ǂݍ���

																					//�t�@�C���ǂݍ���
								FILE* sta = fopen(cFilename, "r");		//�t�@�C���ǂݍ���
								if (sta == nullptr)
								{//�J���Ȃ��������p
									assert(false);
								}
								if (sta != NULL)
								{
									while (fgets(Read, lenLine, sta) != nullptr)	//�ǂݍ��ݗp���[�v
									{
										ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																						//������̕���
										sscanf(Read, "%s", &strLine);

										if (strcmp(&strLine[0], "MOTIONSET") == 0)
										{
											while (fgets(Read, lenLine, sta) != nullptr)	//�ǂݍ��ݗp���[�v
											{
												ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																								//������̕���
												sscanf(Read, "%s", &strLine);
												if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
												{
													//���[�V�����̔ԍ��J��グ
													motionnumber++;
													key = 0;
													fclose(sta);
													break;
												}
												if (strcmp(&strLine[0], "NUM_KEY") == 0)
												{
													sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//�L�[�̑���
												}
												else if (strcmp(&strLine[0], "LOOP") == 0)
												{//���[�v���邩���Ȃ���
													sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//���[�v���邩�ǂ���
												}
												else if (strcmp(&strLine[0], "KEYSET") == 0)
												{
													while (fgets(Read, lenLine, sta) != nullptr)
													{
														ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g
	
														sscanf(Read, "%s", &strLine);	//������̕���

														//key�̓��f���̃L�[�̔ԍ�
														if (strcmp(&strLine[0], "END_KEYSET") == 0)
														{
															key++;
															m_pEnemy = 0;	//�ԍ����Z�b�g
															break;
														}
														else if (strcmp(&strLine[0], "FRAME") == 0)
														{//�L�[�̍Đ����Ԃ̐ݒ�
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[key].nFrame);	//�Đ����Ԃ̐ݒ�
														}
														else if (strcmp(&strLine[0], "COLLISIONSET") == 0)
														{
															m_apMotion[motionnumber].aModelKey[key].Collision[m_apMotion[motionnumber].aModelKey[key].nNumCollision]
																= CCollision::Create(m_pos, CCollision::COLLI_DAMAGE);
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																												//������̕���
																sscanf(Read, "%s", &strLine);

																//key�̓��f���̃L�[�̔ԍ�
																if (strcmp(&strLine[0], "END_COLLISIONSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumCollision++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//����̊J�n����
																	int start, select;
																	select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &start);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//����̊J�n����
																	int end;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &end);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	D3DXVECTOR3 dpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &dpos.x, &dpos.y, &dpos.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aModelKey[key].Collision[select]->SetDPos(dpos);
																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  dsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumCollision;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %f%f%f", &strLine, &dsiz.x, &dsiz.y, &dsiz.z);	//�L�[�̑���
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
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																												//������̕���
																sscanf(Read, "%s", &strLine);

																//key�̓��f���̃L�[�̔ԍ�
																if (strcmp(&strLine[0], "END_HURTSET") == 0)
																{
																	m_apMotion[motionnumber].aModelKey[key].nNumHurtCol++;
																	break;
																}
																else if (strcmp(&strLine[0], "STARTFRAME") == 0)
																{//����̊J�n����
																	int start, select;
																	select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &start);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetStartf(start);
																}
																else if (strcmp(&strLine[0], "ENDFRAME") == 0)
																{//����̊J�n����
																	int end;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %d", &strLine, &end);	//���[�v���邩�ǂ���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetEndf(end);
																}
																else if ((strcmp(&strLine[0], "POS") == 0))
																{
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//���݂̓����蔻��̔ԍ�
																	D3DXVECTOR3 hpos;
																	sscanf(Read, "%s = %f%f%f", &strLine, &hpos.x, &hpos.y, &hpos.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetDPos(hpos);
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetPos(hpos);


																}
																else if ((strcmp(&strLine[0], "SIZ") == 0))
																{
																	D3DXVECTOR3  hsiz;
																	int select = m_apMotion[motionnumber].aModelKey[key].nNumHurtCol;	//���݂̓����蔻��̔ԍ�
																	sscanf(Read, "%s = %f%f%f", &strLine, &hsiz.x, &hsiz.y, &hsiz.z);	//�L�[�̑���
																	m_apMotion[motionnumber].aModelKey[key].HurtCol[select]->SetSiz(hsiz);
																}
															}
														}
														else if (strcmp(&strLine[0], "KEY") == 0)
														{//�L�[�ݒ�
															while (fgets(Read, lenLine, sta) != nullptr)
															{
																ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																												//������̕���
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
																		&m_apMotion[motionnumber].aModelKey[key].aKey[m_pEnemy].fPosZ);	//�Đ����Ԃ̐ݒ�
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
														{//�q�b�g�X�g�b�v�̎��Ԑݒ�
															sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nHitStopTimer);
														}
													}
												}
											}
											break;	//�ǂݍ��݂��I������烋�[�v�E�o
										}
									}
								}
							}
						}
					}
					else if (strcmp(&strLine[0], "AXISSET") == 0)
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v
						{
							ZeroMemory(strLine, sizeof(char) * lenLine);	//�����񃊃Z�b�g

																			//������̕���
							sscanf(Read, "%s", &strLine);

							if (strcmp(&strLine[0], "END_AXIS") == 0)
							{
								break;
							}
							else if ((strcmp(&strLine[0], "POS") == 0))
							{
								D3DXVECTOR3 axpos;
								sscanf(Read, "%s = %f%f%f", &strLine, &axpos.x, &axpos.y, &axpos.z);	//�L�[�̑���
								m_AxisBox->SetPos(axpos + m_pos);
								m_AxisBox->SetDPos(axpos);

							}
							else if ((strcmp(&strLine[0], "SIZ") == 0))
							{
								D3DXVECTOR3  axsiz;
								sscanf(Read, "%s = %f%f%f", &strLine, &axsiz.x, &axsiz.y, &axsiz.z);	//�L�[�̑���
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
//���[�V���������o��
//===========================
void CPlayer::WriteMotion(int nowmotion)
{
	const int lenLine = 2048;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	int nNumKey = 0;			//1���[�V�����̃L�[�̑���


	//�t�@�C���ǂݍ���
	FILE*fp = fopen("data/TXT/SaveMotion.txt", "w");		//�t�@�C���ǂݍ���
	if (fp == nullptr)
	{//�J���Ȃ��������p
		assert(false);
	}
	if (fp != NULL)
	{
		//������̕���
		fprintf(fp, "#========================================\n");
		fprintf(fp, "#  ���[�V�������\n");
		fprintf(fp, "#  [ ���[�V�����ԍ� : %d ]\n", (int)nowmotion);
		fprintf(fp, "#\n");
		fprintf(fp, "#========================================\n");
		fprintf(fp, "\n");
		fprintf(fp, "\n");
		fprintf(fp, "MOTIONSET\n");
		fprintf(fp, "	LOOP = %d		# ���[�v���邩�ǂ���[0:���[�v���Ȃ� / 1:���[�v����]\n", (int)m_apMotion[nowmotion].bLoop);
		fprintf(fp, "	NUM_KEY = %d		# �L�[��\n", (int)m_apMotion[nowmotion].nNumKey);
		fprintf(fp, "	HITSTOP = %d		# �q�b�g�X�g�b�v�̎���\n", (int)m_apMotion[nowmotion].nHitStopTimer);
		fprintf(fp, "\n");

		for (int i = 0; i < (int)m_apMotion[nowmotion].nNumKey; i++)
		{
			fprintf(fp, "	KEYSET			# --- << KEY : %d / %d >> ---\n", i, m_apMotion[nowmotion].nNumKey);
			fprintf(fp, "		FRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].nFrame);
			//�����蔻��̐ݒ�
			for (int j = 0; j < m_apMotion[nowmotion].aModelKey[i].nNumCollision; j++)
			{//�����蔻��̏o��
				fprintf(fp, "\n		#%d�Ԗڂ̓����蔻��\n", j + 1);
				fprintf(fp, "			COLLISIONSET\n");
				fprintf(fp, "				STARTFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetStartf());				//����
				fprintf(fp, "				ENDFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetEndf());					//�I��
				fprintf(fp, "				DAMAGE = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetDMG());						//�_���[�W��
				fprintf(fp, "				HIT = %d\n", m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetHitRig());						//�q�b�g�d��
				fprintf(fp, "				GUARD = %d\n", m_apMotion[nowmotion].aModelKey[m_nKEYData].Collision[j]->GetGuardRig());		//�K�[�h�d��
				fprintf(fp, "				DAMAGE_POINT = %d\n", m_apMotion[nowmotion].aModelKey[m_nKEYData].Collision[j]->GetDp());		//�Z�̑���
				fprintf(fp, "				POS = %.2f %.2f %.2f \n",																		//���W
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetPos().x,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetPos().y,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetPos().z);
				fprintf(fp, "				SIZ = %.2f %.2f %.2f \n",																	//�傫��
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetSiz().x,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetSiz().y,
					m_apMotion[nowmotion].aModelKey[i].Collision[j]->GetSiz().z);
				fprintf(fp, "			END_COLLISIONSET\n\n");
			}

			//���ꔻ��̐ݒ�
			for (int j = 0; j < m_apMotion[nowmotion].aModelKey[i].nNumHurtCol; j++)
			{//���ꔻ��̏o��
				fprintf(fp, "		#%d�Ԗڂ̂��ꔻ��\n", j + 1);
				fprintf(fp, "			HURTSET\n");
				fprintf(fp, "				STARTFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetStartf());	//����
				fprintf(fp, "				ENDFRAME = %d\n", m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetEndf());		//�I��
				fprintf(fp, "				POS = %.2f %.2f %.2f \n",																//���W
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetPos().x,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetPos().y,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetPos().z);
				fprintf(fp, "				SIZ = %.2f %.2f %.2f \n",																//�傫��
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetSiz().x,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetSiz().y,
					m_apMotion[nowmotion].aModelKey[i].HurtCol[j]->GetSiz().z);
				fprintf(fp, "			END_HURTSET\n\n");
			}

			for (int j = 0; j < NUM_PARTS; j++)
			{//�L�[�̍��W�ƕ���
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
//��]�e�X�g
//===========================
void CPlayer::rolling()
{
	//��]���̐ݒ�
	m_vecAxis = D3DXVECTOR3(m_move.z, 0.0f, -m_move.x);

	//��]�ʂ̐ݒ�
	float Radius = m_apModel[0]->GetWidth();
	m_fRolling = D3DXVec3Length(&m_move) * 2 * D3DX_PI / (Radius *D3DX_PI);
}

//===========================
//���[�V�����Ǘ�
//===========================
void CPlayer::MotionManager()
{
	if (m_pMotionOld != m_pMotion)
	{//��Ԃ��Ⴄ�ꍇ
		PlayFirstMotion();
	}
	SetFrame();		//�v���C���[�̃��[�V����

	m_pMotionOld = m_pMotion;
}

//===========================
//�ŏ��̃��[�V������ݒ�
//===========================
void CPlayer::PlayFirstMotion()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//���[�V�����Đݒ�
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}
	m_nKEYData = 0;
	m_frame = 0;
}

//===========================
//�G�f�B�b�g���[�h
//===========================
void CPlayer::EditMode()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//���[�V�����Đݒ�
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ) + m_apModel[i]->GetDPos() 
			+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fTempPos);	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ) + m_apModel[i]->GetDRot() 
			+ m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fTempRot);	//�����̎擾
	}

	CDebugProc::Print("���݂̃��[�h:EDIT");
	char* cNowmodel;
	for (int i = 0; i < NUM_PARTS; i++)
	{
		if (m_nEditModel==i)
		{
			cNowmodel = "��";
			m_apModel[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));
			m_apModel[i]->SetEmi(D3DXCOLOR(0.0f, 0.0f,0.0f, 1.0f));
		}
		else
		{
			cNowmodel = "�E";
			m_apModel[i]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apModel[i]->SetEmi(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

		}
		//�p�x�̐��K��
		Normalize(i);

		CDebugProc::Print("%s [%d]�ʒu:%f,%f,%f �p�x:%f,%f,%f", cNowmodel, i,
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

	//���[�V���������o��
	if (CApplication::GetInputKeyboard()->GetPress(DIK_P))
	{
		WriteMotion(m_pMotion);
	}
	
	//���������Ԃ̃��Z�b�g
	if (m_nHold == m_nHoldold)
	{
		m_nHold = 0;
	}
	m_nHoldold = m_nHold;
}

//===========================
//�p�x�̐��K��
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
//�v���C���[�̏��̐ݒ�(IMGUI)
//===========================
void CPlayer::PlayerStateDraw()
{
	int loop;
	ImGui::NewFrame();

	ImGui::Begin(u8"�v���C���[�̏��");	//�J�n

	if (ImGui::CollapsingHeader(u8"�v���C���[�̍��W"))//�^�u
	{//�v���C���[�̍��W
		ImGui::InputFloat("X", &m_TempPos.x, 0.1f, 0.1f, "%.2f");
		ImGui::InputFloat("Y", &m_TempPos.y, 0.1f, 0.1f, "%.2f");
		ImGui::InputFloat("Z", &m_TempPos.z, 0.1f, 0.1f, "%.2f");
		if (ImGui::Button("RESET"))
			m_pos = D3DXVECTOR3(0.f, 0.f, 0.f);

		if (ImGui::Button("SET"))
			m_pos += m_TempPos, ZeroMemory(m_TempPos, sizeof(D3DXVECTOR3));
	}
	if (ImGui::CollapsingHeader(u8"�v���C���[�̉�]"))//�^�u
	{//�v���C���[�̍��W
		ImGui::InputFloat("X", &m_TempRot.x, 0.01f, 0.01f, "%.2f");
		ImGui::InputFloat("Y", &m_TempRot.y, 0.01f, 0.01f, "%.2f");
		ImGui::InputFloat("Z", &m_TempRot.z, 0.01f, 0.01f, "%.2f");

		if (ImGui::Button("RESET"))
			m_rot = D3DXVECTOR3(0.f, 0.f, 0.f);

		if (ImGui::Button("SET"))
			m_rot += m_TempRot, ZeroMemory(m_TempRot, sizeof(D3DXVECTOR3));
	}
	if (ImGui::CollapsingHeader(u8"�v���C���[�̉����o������̐ݒ�"))
	{
		D3DXVECTOR3 axispos = m_AxisBox->GetPos();
		D3DXVECTOR3 axissiz= m_AxisBox->GetSiz();
		ImGui::DragFloat3("Pos", axispos, -2.0f, 2.0f);
		ImGui::DragFloat3("Siz", axissiz, -2.0f, 2.0f);
		m_AxisBox->SetPos(axispos);
		m_AxisBox->SetSiz(axissiz);
		bool bSave = false;
		if (ImGui::Button("Save"))	//���݂̎�����̏���ۑ�
			bSave = true;	//�Z�[�uOn�ɂ���
		if (bSave == true)
		{//�Z�[�u����悤�ɂ���
			SaveAxis();
			bSave = false;	//�Z�[�uOff�ɖ߂�
		}
	}
	if (ImGui::CollapsingHeader(u8"���[�V�����̐ݒ�"))
	{
		ImGui::SliderInt(u8"���f���I��", &m_nEditModel, 0, NUM_PARTS - 1);										//�ҏW���郂�f��
		ImGui::SliderInt(u8"�Đ����郂�[�V����", (int*)&m_pMotion, 0, PM_MAX - 1);								//�Đ����Ă��郂�[�V�����ԍ�
		ImGui::SliderInt(u8"�Đ�����L�[", &m_nKEYData, 0, m_apMotion[m_pMotion].nNumKey - 1);					//�Đ�����L�[�̔ԍ�
		ImGui::InputInt(u8"�L�[�̐�", &m_apMotion[m_pMotion].nNumKey, 1, 1);									//���[�V�����̃L�[�̐�
		ImGui::SliderInt(u8"���݂̃t���[��",&m_frame,0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);	//���Đ�����Ă���t���[��
		ImGui::InputInt(u8"�t���[����", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame, 1, 1);				//���[�V�����̃t���[����
		ImGui::SliderInt(u8"���[�v���邩�ǂ���", (int*)&m_apMotion[m_pMotion].bLoop, 0,1);						//���[�v���邩�ǂ����ݒ�
		ImGui::SliderInt(u8"�q�b�g�X�g�b�v�ݒ�",&m_apMotion[m_pMotion].nHitStopTimer, 0, 60);					//�q�b�g�X�g�b�v�̎��Ԑݒ�

		if (ImGui::TreeNode(u8"���f���̍��W"))
		{ // ���f���̍��W�ҏW

			ImGui::InputFloat("PosX", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.x, 0.1f, 0.1f, "%.2f");
			ImGui::InputFloat("PosY", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.y, 0.1f, 0.1f, "%.2f");
			ImGui::InputFloat("PosZ", &m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.z, 0.1f, 0.1f, "%.2f");
			ImGui::Text("Pos: (%.2f,%.2f,%.2f)",
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.x + m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.y + m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fTempPos.z + m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ);
			ImGui::Text(u8"���݂̍��W: (%.2f,%.2f,%.2f)",
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
		if (ImGui::TreeNode(u8"���f���̉�]"))
		{ // ���f���̍��W�ҏW
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

			if (ImGui::Button("SET"))//���ݒ�̐��l����
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
		if (ImGui::TreeNode(u8"�����蔻��̐ݒ�"))
		{
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision < MAX_COLLISION-1)
			{
				D3DXVECTOR3 pos =
				D3DXVECTOR3(m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX,
							m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY,
							m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ);
				if (ImGui::Button("Create"))//���萶��
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision]
					= CCollision::Create(m_apModel[m_nEditModel]->GetPos()+pos,CCollision::COLLI_DAMAGE),				//�����蔻��̐���
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision++;					//�ő吔���C���N�������g
			}
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision > 0)
			{//�����蔻�肪���݂���ꍇ
				if (ImGui::Button("Delete"))//����̍폜
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision - 1]->Uninit(),
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision--;				//�ő吔���C���N�������g

				ImGui::SliderInt(u8"�ݒ肷�铖���蔻��̔ԍ�", &m_nSelectCollison, 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision - 1);	//���[�V�����ԍ�
				if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision > 0)
				{//�����蔻�肪���݂���ꍇ
					if (m_nSelectCollison >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision)
					{
						m_nSelectCollison = 0;
					}
					if (ImGui::TreeNode(u8"�����蔻��"))
					{ // ���f���̍��W�ҏW
						m_ColiPos = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetPos();
						m_siz = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetSiz();
						int start = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetStartf();
						int end = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetEndf();
						int damage = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetDMG();
						int hitrig = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetHitRig();
						int guardrig = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetGuardRig();
						int dp = m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->GetDp();

						ImGui::DragFloat3("Pos", m_ColiPos,1.0f,-300.0f, 300.0f, "%.2f");								//���W
						ImGui::DragFloat3("Siz", m_siz,  1.0f, -300.0f, 300.0f, "%.2f");								//�傫��
						ImGui::SliderInt("Start", &(start),0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);		//�t���[��
						ImGui::SliderInt("End", &(end), 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);			//���[�V�����ԍ�
						ImGui::InputInt("Damage",&(damage) , 0, 1000);													//�_���[�W��
						ImGui::SliderInt(u8"�q�b�g�d���ݒ�", &hitrig, -60, 60);											//�q�b�g�d���̐ݒ�
						ImGui::SliderInt(u8"�K�[�h�d���ݒ�", &guardrig, -60, 60);										//�K�[�h�d���̐ݒ�
						ImGui::SliderInt(u8"�_���[�W����", &dp, 0, 3);													//�_���[�W�����ݒ�

						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetPos(m_ColiPos);		//�ʒu���X�V
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetSiz(m_siz);			//�T�C�Y
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetStartf(start);			//�J�n�t���[��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetEndf(end);				//�I���t���[��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetDMG(damage);				//�_���[�W��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetHitRig(hitrig);		//�q�b�g�d��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetGuardRig(guardrig);	//�K�[�h�d��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[m_nSelectCollison]->SetDp((CCollision::EDAMAGE_POINT)dp);

						ImGui::TreePop();
						ImGui::Separator();
					}
				}
			}
			ImGui::TreePop();
			ImGui::Separator();
		}
		if (ImGui::TreeNode(u8"���ꔻ��̐ݒ�"))
		{
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol < MAX_COLLISION - 1)
			{
				if (ImGui::Button("Create"))//���萶��
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol]
					= CCollision::Create(m_apModel[m_nEditModel]->GetPos(), CCollision::COLLI_HURT),		//���ꔻ��̐���
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol++;							//�ő吔���C���N�������g
			}

			if (ImGui::Button(u8"���ꔻ��̃R�s�["))
				for (int i = 0; i <MAX_COLLISION; i++)
				{
					if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i] != nullptr)
					{
						m_CopyCollision[i] = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i];
					}
				};

			if (ImGui::Button(u8"���ꔻ��̃y�[�X�g"))
				for (int i = 0; i < MAX_COLLISION; i++)
				{
					if (m_CopyCollision[i] != nullptr)
					{
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol] = m_CopyCollision[i];
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol++;							//�ő吔���C���N�������g
					}
				};
			if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol > 0)
			{//���ꔻ�肪���݂���ꍇ
				if (ImGui::Button("Delete"))//����̍폜
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol - 1]->Uninit(),
					m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol--;				//�ő吔���C���N�������g

				ImGui::SliderInt(u8"�ݒ肷����ꔻ��̔ԍ�", &m_nSelectHurt, 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol - 1);	//���[�V�����ԍ�
				if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol > 0)
				{//�����蔻�肪���݂���ꍇ
					if (m_nSelectHurt >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol)
					{
						m_nSelectHurt = 0;
					}

					if (ImGui::TreeNode(u8"���ꔻ��"))
					{ // ���f���̍��W�ҏW
						m_ColiPos = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetPos();
						m_siz = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetSiz();
						int start = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetStartf();
						int end = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetEndf();
						int damage = m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->GetDMG();
						ImGui::DragFloat3("Pos", m_ColiPos, 1.0f, -300.0f, 300.0f, "%.2f");								//���W
						ImGui::DragFloat3("Siz", m_siz, 1.0f, -300.0f, 300.0f, "%.2f");									//�傫��
						ImGui::SliderInt("Start", &(start), 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);		//�t���[��
						ImGui::SliderInt("End", &(end), 0, m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame);			//�I���t���[��


						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetPos(m_ColiPos);	//�ʒu���X�V
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetSiz(m_siz);		//�T�C�Y
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetStartf(start);		//�J�n�t���[��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetEndf(end);			//�I���t���[��
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[m_nSelectHurt]->SetDMG(damage);		//�_���[�W�ݒ�

						bool bSave = false;
						if (ImGui::Button("Save"))
							bSave = true;
						if (bSave == true)
						{
							FILE* fp = fopen("data/TXT/SaveHurtCollision.txt", "w");
							//�t�@�C�������o��
							for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol; i++)
							{//�����蔻��̏o��
								fprintf(fp, "		#%d�Ԗڂ̔���\n\n", i + 1);

								fprintf(fp, "			COLLISIONSET\n");
								fprintf(fp, "				POS = %.2f %.2f %.2f \n",																//���W
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetPos().x,
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetPos().y,
									m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetPos().z);
								fprintf(fp, "				SIZ = %.2f %.2f %.2f \n",																//�傫��
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
		if (ImGui::Button(u8"�L�[�̃R�s�["))
			m_CopyKey = m_apMotion[m_pMotion].aModelKey[m_nKEYData];
		if (ImGui::Button(u8"�L�[�̃y�[�X�g"))
			m_apMotion[m_pMotion].aModelKey[m_nKEYData] = m_CopyKey;
		if (ImGui::Button(u8"���݂̃��[�V�����̃f�[�^�ۑ�"))
				WriteMotion(m_pMotion);

	}
	SetFrame();
	ImGui::End();	//�I��
}

//===========================
//�����蔻���\�����邩�ǂ���
//===========================
void CPlayer::DrawCollision()
{
	//����̕\��
	for (int i = 0; i < PM_MAX; i++)
	{
		for (int k = 0; k < m_apMotion[i].nNumKey; k++)
		{
			for (int j = 0; j < m_apMotion[i].aModelKey[k].nNumCollision; j++)
			{//�Ⴄ���[�V�����̓����蔻����I�t�ɂ���
				if (m_pMotion == i&&m_nKEYData == k&&m_apMotion[i].aModelKey[k].Collision[j] != nullptr)
				{//�L�[�ƃ��[�V��������v���Ă���ꍇ�̂ݕ\��{//�����蔻��
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
			{//���ꔻ��
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
//�t���[���ݒ�
//===========================
void CPlayer::SetFrame()
{
	D3DXVECTOR3 RelaPos, RelaRot;		//1�t���[�����Ƃ̈ړ���
	D3DXVECTOR3 pos, rot, DiffPos, DiffRot;
		CDebugProc::Print("���݂̃t���[��:%d", m_frame);

	//�J�E���^�[�X�V
	if (m_nKEYData == m_apMotion[m_pMotion].nNumKey&& m_apMotion[m_pMotion].bLoop == false)
	{
		if (m_bPlay == true)
		{//�G�f�B�b�g����Ȃ��Ƃ����[�v���[�V�������I�������j���[�g�����ɂ���
			m_pMotion = PM_ST_NEUTRAL;
			PlayFirstMotion();
		}
		return;
	}
	else
	{
		for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision; i++)
		{//�_���[�W���肪�o�����邩�ǂ���(1�`4�t���[���܂Ń_���[�W������o���݂�����)
			if (m_frame>= m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetStartf())
			{//�J�n�t���[���ȏォ�ǂ���
				if (m_frame <= m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetEndf())
				{//�I���t���[���ȉ����ǂ���
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
		{//�p�[�c�S���̃��[�V�����Đ�
			if (m_apModel[i] != nullptr)
			{
				if (m_nKEYData == m_apMotion[m_pMotion].nNumKey - 1 && m_apMotion[m_pMotion].bLoop == true)
				{//���[�v����ꍇ�ŏ��̃��[�V�����Ɉڍs����
					DiffPos = D3DXVECTOR3(
						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fPosZ - m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ);	//�����̎擾

					DiffRot = D3DXVECTOR3(
						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,

						m_apMotion[m_pMotion].aModelKey[0].aKey[i].fRotZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ);	//�����̎擾
				}
				else if (m_nKEYData == m_apMotion[m_pMotion].nNumKey - 1 && m_apMotion[m_pMotion].bLoop == false)
				{//���[�v���Ȃ��ꍇ�j���[�g�������[�V�����ɖ߂�
					DiffPos = D3DXVECTOR3(//���W�����̎擾
						m_apMotion[0].aModelKey[0].aKey[i].fPosX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,

						m_apMotion[0].aModelKey[0].aKey[i].fPosY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,

						m_apMotion[0].aModelKey[0].aKey[i].fPosZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//���������̎擾
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
					{//�ʒu���W�̐ݒ肵�Ȃ���
						PlayFirstMotion();
					}
					DiffPos = D3DXVECTOR3(//���W�����̎擾
						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fPosX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosX,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fPosY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosY,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fPosZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fPosZ);

					DiffRot = D3DXVECTOR3(//���������̎擾
						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fRotX -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotX,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fRotY -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotY,

						m_apMotion[m_pMotion].aModelKey[m_nKEYData + 1].aKey[i].fRotZ -
						m_apMotion[m_pMotion].aModelKey[m_nKEYData].aKey[i].fRotZ);
				}
			}

			//�ʒu
			RelaPos = DiffPos / m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame;		//���Βl
			RelaRot = DiffRot / m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame;

			//�L�[�̐ݒ�
			if (m_bPlay == true)
			{//�Đ����[�h�̏ꍇ
				pos = RelaPos + m_apModel[i]->GetPos();
				rot = RelaRot + m_apModel[i]->GetRot();
			}
			else
			{//�ҏW���[�h�̏ꍇ
				pos = RelaPos*m_frame + m_apModel[i]->GetPos();
				rot = RelaRot*m_frame + m_apModel[i]->GetRot();
			}

			//�ʒu�̐ݒ�
			m_apModel[i]->SetPos(pos);
			m_apModel[i]->SetRot(rot);
		}
	}
	if (m_bPlay == true)
	{//�Đ����[�h�̏ꍇ
	 //�J�E���^�[�X�V

		if (m_frame >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].nFrame)
		{//�L�[�ԍ��̍X�V�ƃJ�E���^�[�̃��Z�b�g
			m_nKEYData++;
			m_frame = 0;
			if (m_nKEYData >= m_apMotion[m_pMotion].nNumKey)
			{//�L�[�ԍ����ő吔�𒴂����ꍇ���Z�b�g
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
//���̔���̕ۑ�
//===========================
void CPlayer::SaveAxis()
{
	FILE* fp = fopen("data/TXT/SaveAxis.txt", "w");
	//�t�@�C���ǂݍ��� 
	fprintf(fp, "���̔���\n\n");

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
//�����蔻��̕ۑ�
//===========================
void CPlayer::SaveCollision()
{
	FILE* fp = fopen("data/TXT/SaveCollision.txt", "w");
	//�t�@�C�������o��
	for (int j = 0; j < m_apMotion[m_pMotion].nNumKey; j++)
	{//���̃��[�V�����̑S�ẴL�[��ۑ�
		for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumCollision; i++)
		{//�����蔻��̏o��
			fprintf(fp, "#%d�Ԗڂ̔���\n\n", i + 1);

			fprintf(fp, "	COLLISIONSET\n");
			fprintf(fp, "		STARTFRAME = %d\n", m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetStartf());	//����
			fprintf(fp, "		ENDFRAME = %d\n", m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetEndf());		//�I��
			fprintf(fp, "		POS = %.2f %.2f %.2f \n",																//���W
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetPos().x,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetPos().y,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetPos().z);
			fprintf(fp, "		SIZ = %.2f %.2f %.2f \n",																//�傫��
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetSiz().x,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetSiz().y,
				m_apMotion[m_pMotion].aModelKey[m_nKEYData].Collision[i]->GetSiz().z);
			fprintf(fp, "	END_COLLISIONSET\n");
		}
	}
	fclose(fp);
}

//===========================
//���ꔻ��̐ݒ�
//===========================
void CPlayer::SetHitBox()
{
	for (int i = 0; i < m_apMotion[m_pMotion].aModelKey[m_nKEYData].nNumHurtCol; i++)
	{//�_���[�W���肪�o�����邩�ǂ���(1�`4�t���[���܂Ń_���[�W������o���݂�����)
		if (m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i] != nullptr)
		{
			if (m_frame >= m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetStartf())
			{//�J�n�t���[���ȏォ�ǂ���
				if (m_frame <= m_apMotion[m_pMotion].aModelKey[m_nKEYData].HurtCol[i]->GetEndf())
				{//�I���t���[���ȉ����ǂ���
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
