#include "Game\CNeonInvader.h"

static constexpr XMFLOAT2 KWindowSize{ 960.0f, 540.0f };

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	static constexpr float KClearColor[]{ 0.023f, 0.07f, 0.13f, 1.0f };
	static constexpr D3D11_INPUT_ELEMENT_DESC KInputLayout[]
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 8, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	static const string KAssetDir{ "Asset/" };

	srand((unsigned int)time(nullptr));

	CDirectX directx{ hInstance, KWindowSize };
	directx.Create(TEXT("Neon Invader"), WndProc, nShowCmd, KAssetDir + "dotumche_10_korean.spritefont");

	CShader* vs{ directx.AddShader(EShaderType::VertexShader, L"HLSL/VertexShader.hlsl", "main", KInputLayout, ARRAYSIZE(KInputLayout)) };
	CShader* ps{ directx.AddShader(EShaderType::PixelShader, L"HLSL/PixelShader.hlsl", "main") };
	
	CTexturePool texture_pool{ directx.GetDevicePtr(), directx.GetDeviceContextPtr() };
	CTexture* texture_bg{ texture_pool.AddSharedTexture(KAssetDir + "bg_space_seamless.png") };
	CTexture* texture_sprite{ texture_pool.AddSharedTexture(KAssetDir + "neon_space_shooter.png") };
	CTexture* texture_title{ texture_pool.AddSharedTexture(KAssetDir + "title.png") };
	CTexture* texture_game_over{ texture_pool.AddSharedTexture(KAssetDir + "game_over.png") };
	CTexture* texture_completed{ texture_pool.AddSharedTexture(KAssetDir + "completed.png") };
	CTexture* texture_explosion{ texture_pool.AddSharedTexture(KAssetDir + "explosion_196x190_by_Bleed.png") };
	CTexture* texture_guideline{ texture_pool.AddSharedTexture(KAssetDir + "guideline.png") };
	CTexture* texture_itemset{ texture_pool.AddSharedTexture(KAssetDir + "itemset.png") };
	CTexture* texture_ui{ texture_pool.AddSharedTexture(KAssetDir + "ui.png") };

	CNeonInvader neon_invader{ KWindowSize, &directx };
	neon_invader.CreateGameObjects(texture_sprite, texture_explosion, texture_itemset, texture_ui);
	neon_invader.CreateAudioObjects(KAssetDir);

	CStageSetLoader stage_set_loader{};
	stage_set_loader.LoadStageSetFromFile(KAssetDir + "stage_set.txt");

	neon_invader.SetGameData(stage_set_loader.GetStageSetData());

	CObject2D obj_bg{ &directx };
	{
		obj_bg.SetTexture(texture_bg);
		obj_bg.CreateRectangle(texture_bg->GetTextureSize());
		obj_bg.Sampler = ESampler::Point;
	}

	CObject2D obj_title{ &directx };
	{
		obj_title.SetTexture(texture_title);
		obj_title.CreateRectangle(texture_title->GetTextureSize());
		obj_title.Sampler = ESampler::Linear;
	}
	
	CObject2D obj_game_over{ &directx };
	{
		obj_game_over.SetTexture(texture_game_over);
		obj_game_over.CreateRectangle(texture_game_over->GetTextureSize());
		obj_game_over.Sampler = ESampler::Linear;
	}

	CObject2D obj_completed{ &directx };
	{
		obj_completed.SetTexture(texture_completed);
		obj_completed.CreateRectangle(texture_completed->GetTextureSize());
		obj_completed.Sampler = ESampler::Linear;
	}

	CObject2D obj_guideline{ &directx };
	{
		XMFLOAT2 size{ texture_guideline->GetTextureSize() };
		obj_guideline.SetTexture(texture_guideline);
		obj_guideline.CreateRectangle(texture_guideline->GetTextureSize(), XMFLOAT2(0, size.y / 2));
		obj_guideline.Sampler = ESampler::Linear;
	}

	CObject2D obj_ammo{ &directx };
	{
		obj_ammo.SetTexture(texture_ui);
		obj_ammo.CreateRectangle(XMFLOAT2(50, 50));
		obj_ammo.SetRectangleUVRange(XMFLOAT2(100, 0), XMFLOAT2(50, 50));
		obj_ammo.Sampler = ESampler::Linear;
		obj_ammo.WorldPosition.x = -20.0f;
		obj_ammo.WorldPosition.y = -KWindowSize.y / 2 + 30.0f;
	}

	static constexpr XMFLOAT2 obj_life_size{ 50.0f, 50.0f };
	static constexpr float obj_life_interval{ obj_life_size.x + 10.0f };
	vector<CObject2D> obj_lives{};
	{
		float offset_x{ - obj_life_interval * (0.5f * (neon_invader.KMaxLifeLimit - 1)) };
		for (int i = 0; i < neon_invader.KMaxLifeLimit; ++i)
		{
			obj_lives.emplace_back(&directx);

			auto& obj{ obj_lives.back() };
			obj.SetTexture(texture_ui);
			obj.CreateRectangle(obj_life_size);
			obj.SetRectangleUVRange(XMFLOAT2(obj_life_size.x, 0), obj_life_size);
			obj.Sampler = ESampler::Linear;
			obj.WorldPosition.x = offset_x + obj_life_interval * i;
			obj.WorldPosition.y = KWindowSize.y / 2 - obj_life_size.y + 10.0f;
		}
	}

	CASCIIRenderer ascii_renderer{ &directx };
	ascii_renderer.Create(100, KAssetDir, "charset_d2coding_20_info.txt", 0.8f);

	steady_clock clock{};
	long long time_prev{ clock.now().time_since_epoch().count() };
	long long time_now{};
	long long time_now_microsec{};
	long long timer_animation{};
	long long timer_movement{};
	long long timer_game{};
	long long timer_reorientation{};
	long long timer_item_spawning{};
	float delta_time{};
	bool keys[MAX_PATH]{};

	bool should_show_title{ true };
	bool should_show_game_over{ false };
	bool should_show_completed{ false };
	bool should_draw_guildeline{ false };

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
			auto main_sprite{ neon_invader.GetMainSpriteEntity() };

			vs->Use();
			ps->Use();

			delta_time = 0.000'000'001f * (time_now - time_prev);
			time_now_microsec = time_now / 1'000;

			bool turn_left{}, turn_right{}, move_forward{}, move_backward{}, move_left{}, move_right{};
			if (GetAsyncKeyState(VK_RIGHT) < 0) turn_right = true;
			if (GetAsyncKeyState(VK_LEFT) < 0) turn_left = true;
			if (GetAsyncKeyState(VK_UP) < 0) move_forward = true;
			if (GetAsyncKeyState(VK_DOWN) < 0) move_backward = true;
			if (keys['W']) move_forward = true;
			if (keys['S']) move_backward = true;
			if (keys['A']) move_left = true;
			if (keys['D']) move_right = true;

			if (time_now_microsec >= timer_movement + 3'000)
			{
				if (turn_left) main_sprite->RotationAngle += 0.02f;
				if (turn_right) main_sprite->RotationAngle -= 0.02f;
				if (move_forward) main_sprite->MoveForward(0.7f);
				if (move_backward) main_sprite->MoveBackward(0.4f);
				if (move_left) main_sprite->MoveLeft(0.4f);
				if (move_right) main_sprite->MoveRight(0.4f);

				if (main_sprite->RotationAngle >= XM_2PI) main_sprite->RotationAngle = 0.0f;
				if (main_sprite->RotationAngle <= -XM_2PI) main_sprite->RotationAngle = 0.0f;
				main_sprite->WorldPosition.x = max(main_sprite->WorldPosition.x, -KWindowSize.x / 2);
				main_sprite->WorldPosition.x = min(main_sprite->WorldPosition.x, KWindowSize.x / 2);
				main_sprite->WorldPosition.y = max(main_sprite->WorldPosition.y, -KWindowSize.y / 2);
				main_sprite->WorldPosition.y = min(main_sprite->WorldPosition.y, KWindowSize.y / 2);

				timer_movement = time_now_microsec;
			}

			if (GetAsyncKeyState(VK_SPACE) < 0)
			{
				if (time_now_microsec >= timer_game + 500)
				{
					neon_invader.SpawnMainSpriteShot();
				}
			}

			if (time_now_microsec >= timer_animation + 1'500)
			{
				neon_invader.AnimateScene();

				timer_animation = time_now_microsec;
			}

			directx.BeginRendering(KClearColor);

			obj_bg.Draw();

			if (neon_invader.IsGameRunning() && should_draw_guildeline)
			{
				obj_guideline.WorldPosition = main_sprite->WorldPosition;
				obj_guideline.RotationAngle = main_sprite->RotationAngle;
				obj_guideline.Draw();
			}

			neon_invader.ApplyPhysics(delta_time);
			neon_invader.DrawEntitiesInAddedOrder();
			
			neon_invader.SetCameraToOrigin();

			if (should_show_title)
			{
				if (GetAsyncKeyState(VK_RETURN) < 0)
				{
					should_show_title = false;
					main_sprite->Visible = true;

					neon_invader.InitGame();
					neon_invader.SetStage(0);
				}

				obj_title.Draw();
			}
			else
			{
				ascii_renderer.RenderText("Score: " + to_string(neon_invader.GetScore()),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 010.0f));

				ascii_renderer.RenderText("Stage: " + to_string(neon_invader.GetStage() + 1) + "/" + to_string(neon_invader.GetMaxStage() + 1),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 050.0f));
				ascii_renderer.RenderText("Enemy: " + to_string(neon_invader.GetEnemyCount()) + "/" + to_string(neon_invader.GetMaxEnemyCount()),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 080.0f));

				ascii_renderer.RenderText(" : " + to_string(neon_invader.GetShotCount()) + "/" + to_string(neon_invader.GetMaxShotCount()),
					XMFLOAT2(0.0f, -KWindowSize.y / 2 + 50.0f));

				obj_ammo.Draw();

				int life{ neon_invader.GetLife() };
				for (int i = 0; i < neon_invader.KMaxLifeLimit; ++i)
				{
					if (i < life)
					{
						obj_lives[i].SetRectangleUVRange(XMFLOAT2(50, 0), obj_life_size);
					}
					else
					{
						obj_lives[i].SetRectangleUVRange(XMFLOAT2(0, 0), obj_life_size);
					}
					obj_lives[i].Draw();
				}
			}
			
			if (should_show_game_over)
			{
				if (GetAsyncKeyState(VK_RETURN) < 0)
				{
					should_show_game_over = false;
					main_sprite->Visible = true;

					neon_invader.InitGame();
					neon_invader.SetStage(0);
				}

				obj_game_over.Draw();
			}
			else if (should_show_completed)
			{
				if (GetAsyncKeyState(VK_RETURN) < 0)
				{
					should_show_completed = false;

					neon_invader.InitGame();
					neon_invader.SetStage(0);
				}

				obj_completed.Draw();
			}

			directx.EndRendering();

			if (time_now_microsec >= timer_item_spawning + 2'000'000)
			{
				neon_invader.SpawnItem();

				timer_item_spawning = time_now_microsec;
			}
			
			if (time_now_microsec >= timer_reorientation + 5'000)
			{
				neon_invader.ReorientEnemies();
				neon_invader.ReorientItems();

				timer_reorientation = time_now_microsec;
			}

			if (time_now_microsec >= timer_game + 500)
			{
				neon_invader.ExecuteGame();

				timer_game = time_now_microsec;
			}

			if (neon_invader.IsGameOver())
			{
				if (neon_invader.IsCompleted())
				{
					should_show_completed = true;
				}
				else
				{
					should_show_game_over = true;
					main_sprite->Visible = false;
				}
			}

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