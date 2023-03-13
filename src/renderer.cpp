//=============================================================================
//
// �����_�[
// Author : �L�c����
//
//=============================================================================

//�C���N���[�h
#include"renderer.h"
#include"Object2D.h"
#include"Application.h"

//=============================================================================
// �R���X�g���N�^
//=============================================================================
CRenderer::CRenderer()
{
	//�����o�ϐ��̏�����
	g_pD3D = nullptr;							// Direct3D�I�u�W�F�N�g
	g_pD3DDevice = nullptr;						// Device�I�u�W�F�N�g
	g_pFont = nullptr;							// �t�H���g
}

//=============================================================================
//�f�X�g���N�^
//=============================================================================
CRenderer::~CRenderer()
{
	assert(g_pD3D == nullptr);					// Direct3D�I�u�W�F�N�g
	assert(g_pD3DDevice == nullptr);			// Device�I�u�W�F�N�g
	assert(g_pFont == nullptr);					//Crenderer��null���m�F
}

//=============================================================================
// ������
//=============================================================================
HRESULT CRenderer::Init(HWND hWnd, bool bWindow)
{

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;
	g_pD3D = nullptr;
	g_pD3DDevice = nullptr;

	// Direct3D�I�u�W�F�N�g�̍쐬
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));							// ���[�N���[���N���A
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �J���[���[�h�̎w��
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �f���M���ɓ������ăt���b�v����
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�i�y�o�b�t�@�j�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

	// �f�B�X�v���C�A�_�v�^��\�����߂̃f�o�C�X���쐬
	// �`��ƒ��_�������n�[�h�E�F�A�ōs�Ȃ�
	if ((FAILED(CRenderer::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))) &&
		// ��L�̐ݒ肪���s������
		// �`����n�[�h�E�F�A�ōs���A���_������CPU�ōs�Ȃ�
		(FAILED(CRenderer::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))) &&
		// ��L�̐ݒ肪���s������
		// �`��ƒ��_������CPU�ōs�Ȃ�
		(FAILED(CRenderer::g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &g_pD3DDevice))))
	{
		// �������s
		return E_FAIL;
	}
	////CRenderer�̏���������
	//d3dpp.EnableAutoDepthStencil = TRUE;
	//d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	
	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(g_pD3DDevice);	//�����_���[�̐�����ɍ��

										// �t���[������
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();

	// �����_�[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	//�|���S���̑傫���ɕ����Ă��ꂢ�Ƀe�N�X�`�����͂�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	//�e�N�X�`�����WU��1.0�𒴂����ꍇ�J��Ԃ����ǂ��������߂�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	//�e�N�X�`�����WV��1.0�𒴂����ꍇ�J��Ԃ����ǂ��������߂�

	// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	//OP�̓I�y���[�V�����̈� �|���S���ƃe�N�X�`���̐F��������
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	//�e�N�X�`���̃A���t�@�l���Q�Ƃ���
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	//�|���S���̃A���t�@�l���Q�Ƃ���

#ifdef _DEBUG
	//// �f�o�b�O���\���p�t�H���g�̐���
	//D3DXCreateFont(g_pD3DDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET,
	//	OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("Terminal"), &g_pFont);
#endif

	return S_OK;
}

//=============================================================================
// �I��
//=============================================================================
void  CRenderer::Uninit()
{
	//�I�u�W�F�N�g�̔j��
	CObject::ReleaseAll();

#ifdef _DEBUG
	// �f�o�b�O���\���p�t�H���g�̔j��
	if (g_pFont != nullptr)
	{
		g_pFont->Release();
		g_pFont = nullptr;
	}
#endif // _DEBUG

	// �f�o�C�X�̔j��
	if (g_pD3DDevice != nullptr)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != nullptr)
	{
		g_pD3D->Release();
		g_pD3D = nullptr;
	}
}

//=============================================================================
// �X�V
//=============================================================================
void  CRenderer::Update()
{
	// �|���S���̍X�V����
	CObject::UpdateAll();
}

//=============================================================================
// �`��
//=============================================================================
void  CRenderer::Draw()
{
	//Z�o�b�t�@&�X�e���V���o�b�t�@�̃N���A
	//g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_STENCIL | D3DCLEAR_TARGET));

	// �o�b�N�o�b�t�@���y�o�b�t�@�̃N���A
	g_pD3DDevice->Clear(0, nullptr,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		// �|���S���̕`�揈��
		CObject::DrawAll();

		//�f�o�b�O���̕`��
		CApplication::GetDebugProc()->Draw();

		// Rendering
		ImGui::EndFrame();

		//ImGui
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

		// Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);

}

//======================================================
//�f�o�C�X�̎擾
//======================================================
LPDIRECT3DDEVICE9  CRenderer::GetDevice(void)
{
	return g_pD3DDevice;
}


//=============================================================================
// FPS�\��
//=============================================================================
#ifdef _DEBUG

void  CRenderer::DrawFPS()
{
	int nCntFPS = GetFPS();
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	TCHAR str[256];

	wsprintf(str, _T("FPS : %d\n"), nCntFPS);

	// �e�L�X�g�`��
	g_pFont->DrawText(NULL, str, -1, &rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
}
#endif // _DEBUG
