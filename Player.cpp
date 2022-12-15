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
#include"Line.h"

//�}�N����`
#define PLAYER_SPEED	(1.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x

//===========================
//�R���X�g���N�^
//===========================
CPlayer::CPlayer(int nPriority) :CObject(nPriority)
{
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
	m_pState = P_STAND;
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
			m_apMotion[m_pState].aModelKey[m_nKEYData].nNumCollision = 0;
			m_apMotion[m_pState].aModelKey[m_nKEYData].nFrame = 0;
		}
	}
	
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
	if (m_bPlay == true)
	{
		m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
		m_pos += m_move;		//�ʒu�̍X�V
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̍X�V

		if (ControlPlayer() == true)		//����
		{
			//m_pState = P_STAND;	//���삳��Ă��Ȃ��ꍇ�j���[�g������
		}

		if (CGame::GetMesh() != nullptr)
		{
			m_pos.y = CGame::GetMesh()->Collision(m_pos);
		}


		CDebugProc::Print("���݂̃��[�h:PLAY");

		MotionManager();

		CDebugProc::Print("���݂̃��[�h:%d�@", (int)m_pState);
	}

	if (m_bPlay == false)
	{
		EditMode();
	}

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_F2))
	{//���[�h�؂�ւ�
		m_bPlay = !m_bPlay;
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
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//�p�[�c�̃��f���̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
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
	const int lenLine = 2048;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	char Read[lenLine];			//�ǂݎ��p
	int	modelnumber = 0;		//���f���̔ԍ�
	int motionnumber = 0;		//���[�V�����̔ԍ�
	int nNumKey = 0;			//1�̃��[�V�����̃L�[�̑���
	int key = 0;
	int model = 0;
	int Idx = 0;


	//�t�@�C���ǂݍ���
	FILE*fp = fopen("data/TXT/ReadMotion.txt", "r");		//�t�@�C���ǂݍ���
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
					ZeroMemory(strLine,sizeof(char)* 2048);	//�����񃊃Z�b�g

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

						m_apModel[modelnumber]->SetModel(&m_nModelpass[0]);
						modelnumber++;
					}
					else if (strcmp(&strLine[0], "CHARACTERSET") == 0)
					{//�����ʒu�̐ݒ�
						while (fgets(Read, lenLine, fp) != nullptr)//��s�ǂݍ���
						{//�L�����N�^�[�̐ݒ�
							//������̕���
							ZeroMemory(strLine, sizeof(char) * 2048);	//�����񃊃Z�b�g

							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_CHARACTERSET") == 0)
							{
								break;
							}
							if (strcmp(&strLine[0], "PARTSSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��݃��[�v //��s�ǂݍ���
								{//�p�[�c�̐ݒ�	
									ZeroMemory(strLine, sizeof(char) * 2048);	//�����񃊃Z�b�g

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
					{
						while (fgets(Read, lenLine, fp) != nullptr)	//�ǂݍ��ݗp���[�v
						{
							ZeroMemory(strLine, sizeof(char) * 2048);	//�����񃊃Z�b�g

							//������̕���
							sscanf(Read, "%s", &strLine);
							if (strcmp(&strLine[0], "END_MOTIONSET") == 0)
							{
								//���[�V�����̔ԍ��J��グ
								motionnumber++;
								key = 0;
								break;
							}
							else if (strcmp(&strLine[0], "NUM_KEY") == 0)
							{
								sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].nNumKey);	//�L�[�̑���
							}
							else if (strcmp(&strLine[0], "LOOP") == 0)
							{//���[�v���邩���Ȃ���
								sscanf(Read, "%s = %d", &strLine, (int*)&m_apMotion[motionnumber].bLoop);	//���[�v���邩�ǂ���
							}
							else if (strcmp(&strLine[0], "KEYSET") == 0)
							{
								while (fgets(Read, lenLine, fp) != nullptr)
								{
									ZeroMemory(strLine, sizeof(char) * 2048);	//�����񃊃Z�b�g

									//������̕���
									sscanf(Read, "%s", &strLine);

									//key�̓��f���̃L�[�̔ԍ�
									if (strcmp(&strLine[0], "END_KEYSET") == 0)
									{
										key++;
										model = 0;	//�ԍ����Z�b�g
										break;
									}
									else if (strcmp(&strLine[0], "FRAME") == 0)
									{//�L�[�̍Đ����Ԃ̐ݒ�
										sscanf(Read, "%s = %d", &strLine, &m_apMotion[motionnumber].aModelKey[key].nFrame);	//�Đ����Ԃ̐ݒ�
									}
									else if (strcmp(&strLine[0], "KEY") == 0)
									{//�L�[�ݒ�
										while (fgets(Read, lenLine, fp) != nullptr)
										{
											ZeroMemory(strLine, sizeof(char) * 2048);	//�����񃊃Z�b�g

											//������̕���
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
													&m_apMotion[motionnumber].aModelKey[key].aKey[model].fPosZ);	//�Đ����Ԃ̐ݒ�
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
//���[�V���������o��
//===========================
void CPlayer::WriteMotion(int nowmotion)
{
	const int lenLine = 2048;	//1�P��̍ő吔
	char strLine[lenLine];		//�ǂݍ��ݗp�̕�����
	char Read[lenLine];			//�ǂݎ��p
	int	modelnumber = 0;		//���f���̔ԍ�
	int motionnumber = 0;		//���[�V�����̔ԍ�
	int nNumKey = 0;			//1���[�V�����̃L�[�̑���
	int key = 0;
	int model = 0;
	int Idx = 0;


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
//�p�[�c�̃��[�V�����Đ�
//===========================
void CPlayer::MotionPlayer(int nNumber)
{
	D3DXVECTOR3 pos, rot, fDiffPos, fDiffRot;
	D3DXVECTOR3 fRelaPos, fRelaRot;		//1�t���[�����Ƃ̈ړ���
	CDebugProc::Print("���݂̃L�[:%d", m_nCurrentKey);
	CDebugProc::Print("���݂̃t���[��:%d", m_MotionCnt);

	//�J�E���^�[�X�V
	if (m_nCurrentKey == m_apMotion[nNumber].nNumKey-1&&m_apMotion[nNumber].bLoop == false)
	{
		if (m_bPlay == true)
		{//�G�f�B�b�g����Ȃ��Ƃ����[�v���[�V�������I�������j���[�g�����ɂ���
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
		{//�p�[�c�S���̃��[�V�����Đ�
			if (m_apModel[i] != nullptr)
			{
				if (m_nCurrentKey == m_apMotion[nNumber].nNumKey - 1&& m_apMotion[nNumber].bLoop == true)
				{//���[�v����ꍇ�ŏ��̃��[�V�����Ɉڍs����
					fDiffPos = D3DXVECTOR3(
						m_apMotion[nNumber].aModelKey[0].aKey[i].fPosX - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fPosY -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fPosZ - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	//�����̎擾

					fDiffRot = D3DXVECTOR3(
						m_apMotion[nNumber].aModelKey[0].aKey[i].fRotX - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fRotY -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[nNumber].aModelKey[0].aKey[i].fRotZ - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	//�����̎擾
				}
				
				else
				{
					if (m_nCurrentKey == 0&& m_MotionCnt==0)
					{//�ʒu���W�̐ݒ肵�Ȃ���
						PlayFirstMotion();
					}
					fDiffPos = D3DXVECTOR3(//���W�����̎擾
						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosX - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosX,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosY - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosY,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosZ -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	

					fDiffRot = D3DXVECTOR3(//���������̎擾
						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotX -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotX,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotY - 
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotY,

						m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotZ -
						m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	
				}

				//�ʒu
				fRelaPos = fDiffPos / m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame;		//���Βl
				fRelaRot = fDiffRot / m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame;

				//�L�[�̐ݒ�
				pos = fRelaPos + m_apModel[i]->GetPos();
				rot = fRelaRot + m_apModel[i]->GetRot();

				//�ʒu�̐ݒ�
				m_apModel[i]->SetPos(pos);
				m_apModel[i]->SetRot(rot);
			}
		}
		//�J�E���^�[�X�V
		if (m_MotionCnt >= m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame)
		{//�L�[�ԍ��̍X�V�ƃJ�E���^�[�̃��Z�b�g
			m_nCurrentKey++;
			m_MotionCnt = 0;
			if (m_nCurrentKey >= m_apMotion[nNumber].nNumKey)
			{//�L�[�ԍ����ő吔�𒴂����ꍇ���Z�b�g
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
	if (m_pStateOld != m_pState)
	{//��Ԃ��Ⴄ�ꍇ
		PlayFirstMotion();
	}
	MotionPlayer(m_pState);		//�v���C���[�̃��[�V����

	m_pStateOld = m_pState;
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
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}
	m_nCurrentKey = 0;
	m_MotionCnt = 0;
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
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}

	CDebugProc::Print("���݂̃��[�h:EDIT");

	CDebugProc::Print("���ݍĐ�����Ă郂�[�V���� 3/4:%d�@", (int)m_pState);
	CDebugProc::Print("���݂̃L�[ 5/6:%d�@", m_nKEYData);

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
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fPosZ,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[i].fRotZ);
	}

	CDebugProc::Print("�ʒu�̐ݒ� X:Y/T Y:H/G Z:N/B");
	CDebugProc::Print("�p�x�̐ݒ� X:NUM8/NUM7 Y:NUM5/NUM4 Z:NUM2/NUM1");

	//X���W
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosX, SINGLE_STA, HOLD_STA,DIK_Y, DIK_T);

	//Y���W
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosY, SINGLE_STA, HOLD_STA, DIK_H, DIK_G);

	//Y���W
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fPosZ, SINGLE_STA, HOLD_STA, DIK_N, DIK_B);

	//X����
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotX = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotX, SINGLE_STA, HOLD_STA,DIK_NUMPAD8, DIK_NUMPAD7);

	//Y����
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotY =
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotY, SINGLE_STA, HOLD_STA, DIK_NUMPAD5, DIK_NUMPAD4);

	//Z����
	m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotZ = 
		State<float, int>(m_apMotion[m_pState].aModelKey[m_nKEYData].aKey[m_nEditModel].fRotZ, SINGLE_STA, HOLD_STA, DIK_NUMPAD2, DIK_NUMPAD1);

	if (CApplication::GetInputKeyboard()->GetTrigger(DIK_4))
	{//�Đ����[�V�����ύX
		m_pState = static_cast<PLAYER_STATE>(m_pState + 1);

		if (m_pState == P_MAX)
		{//�ő�l�̏ꍇ�Œ�l�ɖ߂�
			m_pState = P_STAND;
		}
	}
	else if (CApplication::GetInputKeyboard()->GetTrigger(DIK_3))
	{//�Đ����[�V�����ύX
		if (m_pState != P_STAND)
		{
			m_pState = static_cast<PLAYER_STATE>(m_pState - 1);
		}
		else
		{//�Œ�l�̏ꍇ�ő�l�ɖ߂�
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


	//�L�[�̕ύX
	m_nKEYData = State<int, int>(m_nKEYData, 1, 1, DIK_6, DIK_5);
		if (m_nKEYData < 0)
		{
			m_nKEYData = m_apMotion[m_pState].nNumKey-1;
		}
		if (m_nKEYData>=m_apMotion[m_pState].nNumKey)
		{
			m_nKEYData = 0;
		}

	//�ҏW�p�[�c�̕ύX
	m_nEditModel = State<int, int>(m_nEditModel, 1, 1, DIK_DOWN, DIK_UP);

	m_nEditModel = m_nEditModel % NUM_PARTS;
	if (m_nEditModel < 0)
	{
		m_nEditModel = NUM_PARTS-1;
	}

	//���[�V���������o��
	if (CApplication::GetInputKeyboard()->GetPress(DIK_P))
	{
		WriteMotion(m_pState);
	}
	
	//���������Ԃ̃��Z�b�g
	if (m_nHold== m_nHoldold)
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
//�����蔻��̐ݒ�
//===========================
void CPlayer::SetCollision()
{

}

