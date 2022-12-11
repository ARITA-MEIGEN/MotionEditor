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

//�}�N����`
#define PLAYER_SPEED	(1.0f)	//�ړ����x
#define MIN_SPEED		(0.1f)	//���C�ɂ��Œᑬ�x
#define NUM_MODELPARTS	(1)		//���f���̃p�[�c��

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
	for (int i = 0; i < NUM_PARTS; i++)
	{//�v���C���[�̐���
		m_apModel[i] = CModel::Create();
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

	m_pState=P_NEUTRAL;

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
	m_pState = P_NEUTRAL;
	m_posold = m_pos;		//�O��̈ʒu�̕ۑ�
	m_pos += m_move;		//�ʒu�̍X�V
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		//�ʒu�̍X�V

	ControlPlayer();		//����
	MotionManager();

	/*m_pos.y = */CGame::GetMesh()->Collision(m_pos);


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

	//�N�H�[�^�j�I���̐ݒ�
	//D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fRolling);

	//��]�p�}�g���b�N�X�̐���
	//D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);

	//�p�[�c�̃��f���̌����𔽉f
	//D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);	//�N�H�[�^�j�I���̔��f
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
void CPlayer::ControlPlayer(void)
{
	CCamera*pCamera = CGame::GetCamera();
	CInputKeyboard* pKeyboard = CApplication::GetInputKeyboard();

	//�ړ�
	if (pKeyboard->GetPress(DIK_UP))
	{
		m_move.x = sinf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_pState = P_MOVE;
		//m_rotDest.y = D3DX_PI*1.0f + pCamera->Getrot().y;
	}
	 if (pKeyboard->GetPress(DIK_DOWN))
	{
		m_move.x = sinf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*1.0f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_pState = P_MOVE;
		//m_rotDest.y = D3DX_PI*0.0f + pCamera->Getrot().y;
	}
	if (pKeyboard->GetPress(DIK_LEFT))
	{
		m_move.x = sinf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*-0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_pState = P_MOVE;
		//m_rotDest.y = D3DX_PI*0.5f + pCamera->Getrot().y;
	}
	 if (pKeyboard->GetPress(DIK_RIGHT))
	{
		m_move.x = sinf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_move.z = cosf(D3DX_PI*0.5f + pCamera->Getrot().y)*PLAYER_SPEED;
		m_pState = P_MOVE;
		//m_rotDest.y = D3DX_PI*-0.5f + pCamera->Getrot().y;
	}

	////���ʗ���Ă�����t��]
	//if (m_rotDest.y - m_rot.y >= D3DX_PI)
	//{
	//	m_rotDest.y -= D3DX_PI * 2;
	//}
	//else if (m_rotDest.y - m_rot.y <= -D3DX_PI)
	//{
	//	m_rotDest.y += D3DX_PI * 2;
	//}

	//m_rot.y += (m_rotDest.y - m_rot.y)*0.1f;

	//�p�x�̐��K��
	if (m_rot.y >= D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2;
	}
	else if (m_rot.y <= -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2;
	}

	//rolling();
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
	int nNumKey = 0;			//1���[�V�����̃L�[�̑���
	int key = 0;
	int model = 0;
	int Idx = 0;


	//�t�@�C���ǂݍ���
	FILE*fp = fopen("data/TXT/Motion.txt", "r");		//�t�@�C���ǂݍ���
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
//�p�[�c�̃��[�V����
//===========================
void CPlayer::MotionPlayer(int nNumber)
{
	D3DXVECTOR3 pos, rot, fDiffPos, fDiffRot;
	D3DXVECTOR3 fRelaPos, fRelaRot;		//1�t���[�����Ƃ̈ړ���

	//�J�E���^�[�X�V
	if (m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame==0)
	{
		m_nCurrentKey = 0;
	}
	
	for (int i = 0; i < NUM_PLAYERPARTS; i++)
	{//�p�[�c�S���̃��[�V�����Đ�
		if (m_apModel[i] != nullptr)
		{
			if (m_nCurrentKey == m_apMotion[nNumber].nNumKey - 1)
			{
				fDiffPos = D3DXVECTOR3(
					m_apMotion[nNumber].aModelKey[0].aKey[i].fPosX - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosX,
					m_apMotion[nNumber].aModelKey[0].aKey[i].fPosY - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosY,
					m_apMotion[nNumber].aModelKey[0].aKey[i].fPosZ - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	//�����̎擾

				fDiffRot = D3DXVECTOR3(
					m_apMotion[nNumber].aModelKey[0].aKey[i].fRotX - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotX,
					m_apMotion[nNumber].aModelKey[0].aKey[i].fRotY - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotY,
					m_apMotion[nNumber].aModelKey[0].aKey[i].fRotZ - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	//�����̎擾
			}
			else
			{
				fDiffPos = D3DXVECTOR3(
					m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosX - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosX,
					m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosY - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosY,
					m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fPosZ - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fPosZ);	//�����̎擾

				fDiffRot = D3DXVECTOR3(
					m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotX - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotX,
					m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotY - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotY,
					m_apMotion[nNumber].aModelKey[m_nCurrentKey + 1].aKey[i].fRotZ - m_apMotion[nNumber].aModelKey[m_nCurrentKey].aKey[i].fRotZ);	//�����̎擾
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
	if (m_MotionCnt++ >= m_apMotion[nNumber].aModelKey[m_nCurrentKey].nFrame)
	{//�L�[�ԍ��̍X�V�ƃJ�E���^�[�̃��Z�b�g
		m_nCurrentKey++;
		m_MotionCnt = 0;
		if (m_nCurrentKey >= m_apMotion[nNumber].nNumKey)
		{//�L�[�ԍ����ő吔�𒴂����ꍇ���Z�b�g
			m_nCurrentKey = 0;
		}
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

	if (m_pState == P_NEUTRAL)
	{//�j���[�g����
		MotionPlayer(P_NEUTRAL);		//�v���C���[�̃��[�V����
	}

	else if (m_pState == P_MOVE)
	{//�ړ�
		MotionPlayer(P_MOVE);		//�v���C���[�̃��[�V����
	}
	m_pStateOld = m_pState;
}

//===========================
//�ŏ��̃��[�V������ݒ�
//===========================
void CPlayer::PlayFirstMotion()
{
	for (int i = 0; i < NUM_PARTS; i++)
	{
		//�v���C���[�̐���
		m_apModel[i]->SetPos(D3DXVECTOR3(
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosX,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosY,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fPosZ) + m_apModel[i]->GetDPos());	//�����ʒu�̐ݒ�

		m_apModel[i]->SetRot(D3DXVECTOR3(
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotX,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotY,
			m_apMotion[m_pState].aModelKey[0].aKey[i].fRotZ) + m_apModel[i]->GetDRot());	//�����̎擾
	}
}

