#include "Helper\CDirectX.h"
#include "Game\CEntityPool.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	static constexpr XMFLOAT2 KWindowSize{ 960.0f, 540.0f };
	static constexpr float KClearColor[]{ 0.0f, 0.6f, 1.0f, 1.0f };
	static constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static const string KAssetDir{ "Asset/" };

	CDirectX directx{ hInstance, KWindowSize };
	directx.Create(TEXT("Neon Invader"), WndProc, nShowCmd, KAssetDir + "dotumche_10_korean.spritefont");

	CShader* vs{ directx.AddShader(EShaderType::VertexShader, L"HLSL/VertexShader.hlsl", "main", KInputLayout, ARRAYSIZE(KInputLayout)) };
	CShader* ps{ directx.AddShader(EShaderType::PixelShader, L"HLSL/PixelShader.hlsl", "main") };
	
	CEntityPool entity_pool{ directx };
	CTexture* texture_bg{ entity_pool.AddSharedTexture(KAssetDir + "bg_space_seamless.png") };
	CTexture* texture_sprite{ entity_pool.AddSharedTexture(KAssetDir + "neon_space_shooter.png")};
	CEntity* entity_bg{ entity_pool.AddEntity() };
	{
		entity_bg->SetTexture(texture_bg);
		entity_bg->CreateRectangle(texture_bg->GetTextureSize());
		entity_bg->Sampler = ESampler::Point;
	}

	CEntity* entity_main_ship{ entity_pool.AddEntity() };
	{
		entity_main_ship->SetTexture(texture_sprite);
		entity_main_ship->CreateRectangle(XMFLOAT2(110, 80));
		entity_main_ship->SetRectangleUVRange(XMFLOAT2(0, 40), XMFLOAT2(110, 80));
		entity_main_ship->Sampler = ESampler::Linear;
	}

	CEntity* entity_main_ship_shot{ entity_pool.AddEntity() };
	{
		entity_main_ship_shot->SetTexture(texture_sprite);
		entity_main_ship_shot->CreateRectangle(XMFLOAT2(110, 40));
		entity_main_ship_shot->SetRectangleUVRange(XMFLOAT2(0, 0), XMFLOAT2(110, 40));
		entity_main_ship_shot->Sampler = ESampler::Linear;
		entity_main_ship_shot->Visible = true;
	}

	steady_clock clock{};
	long long time_prev{ clock.now().time_since_epoch().count() };
	long long time_now{};
	long long time_now_microsec{};
	long long timer_animation{};
	long long timer_movement{};
	float delta_time{};
	bool keys[MAX_PATH]{};
	while (true)
	{
		static MSG msg{};
		time_now = clock.now().time_since_epoch().count();

		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT) break;

			if (msg.message == WM_KEYDOWN)
			{
				UINT key{ MapVirtualKey(static_cast<UINT>(msg.wParam), MAPVK_VK_TO_CHAR) };
				keys[key] = true;
			}
			if (msg.message == WM_KEYUP)
			{
				UINT key{ MapVirtualKey(static_cast<UINT>(msg.wParam), MAPVK_VK_TO_CHAR) };
				keys[key] = false;
			}

			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
		{
			delta_time = 0.000'000'001f * (time_now - time_prev);
			time_now_microsec = time_now / 1'000;

			bool turn_left{}, turn_right{}, move_forward{}, move_backward{}, move_left{}, move_right{};
			if (GetKeyState(VK_RIGHT) < 0) turn_right = true;
			if (GetKeyState(VK_LEFT) < 0) turn_left = true;
			if (GetKeyState(VK_UP) < 0) move_forward = true;
			if (GetKeyState(VK_DOWN) < 0) move_backward = true;
			if (keys['W']) move_forward = true;
			if (keys['S']) move_backward = true;
			if (keys['A']) move_left = true;
			if (keys['D']) move_right = true;

			if (time_now_microsec >= timer_movement + 3'000)
			{
				if (turn_left) entity_main_ship->RotationAngle += 0.03f;
				if (turn_right) entity_main_ship->RotationAngle -= 0.03f;
				if (move_forward) entity_main_ship->MoveForward(0.8f);
				if (move_backward) entity_main_ship->MoveBackward(0.5f);
				if (move_left) entity_main_ship->MoveLeft(0.5f);
				if (move_right) entity_main_ship->MoveRight(0.5f);

				if (entity_main_ship->RotationAngle >= XM_2PI) entity_main_ship->RotationAngle = 0.0f;
				if (entity_main_ship->RotationAngle <= -XM_2PI) entity_main_ship->RotationAngle = 0.0f;
				entity_main_ship->WorldPosition.x = max(entity_main_ship->WorldPosition.x, -KWindowSize.x / 2);
				entity_main_ship->WorldPosition.x = min(entity_main_ship->WorldPosition.x, KWindowSize.x / 2);
				entity_main_ship->WorldPosition.y = max(entity_main_ship->WorldPosition.y, -KWindowSize.y / 2);
				entity_main_ship->WorldPosition.y = min(entity_main_ship->WorldPosition.y, KWindowSize.y / 2);

				timer_movement = time_now_microsec;
			}

			if (time_now_microsec >= timer_animation + 1'500)
			{
				timer_animation = time_now_microsec;
			}
			
			directx.BeginRendering(KClearColor);

			vs->Use();
			ps->Use();

			entity_pool.DrawEntities();

			directx.RenderText(L"Delta Time: " + to_wstring(delta_time) + L" ÃÊ", XMFLOAT2(0, 0), Colors::LimeGreen);
			directx.RenderText(L"Rotation angle: " + to_wstring(entity_main_ship->RotationAngle), XMFLOAT2(0, 15), Colors::LimeGreen);

			directx.EndRendering();

			time_prev = time_now;
		}
	}

	return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}