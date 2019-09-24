#include "Game\CNeonInvader.h"

static constexpr XMFLOAT2 KWindowSize{ 960.0f, 540.0f };

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPTSTR lpCmdLine, _In_ int nShowCmd)
{
	static constexpr float KClearColor[]{ 0.0f, 0.6f, 1.0f, 1.0f };
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

	CNeonInvader neon_invader{ KWindowSize };

	CEntityPool entity_pool{ &directx };
	vector<SShot> v_main_ship_shots{};
	for (size_t i = 0; i < neon_invader.KMaxMainSpriteShotLimit; ++i)
	{
		v_main_ship_shots.emplace_back();
		v_main_ship_shots.back().PtrEntity = entity_pool.CreateEntity();
		v_main_ship_shots.back().PtrEntity->SetTexture(texture_sprite);
		v_main_ship_shots.back().PtrEntity->CreateRectangle(XMFLOAT2(110, 40));
		v_main_ship_shots.back().PtrEntity->SetRectangleUVRange(XMFLOAT2(0, 0), XMFLOAT2(110, 40));
		v_main_ship_shots.back().PtrEntity->Sampler = ESampler::Linear;
		v_main_ship_shots.back().PtrEntity->Visible = false;
		v_main_ship_shots.back().PtrEntity->SetCollisionBox(XMFLOAT2(1, 10));

		entity_pool.AddMainSpriteShotEntity(v_main_ship_shots.back().PtrEntity);
	}

	vector<SShot> v_enemy_shots{};
	for (size_t i = 0; i < neon_invader.KMaxEnemyShotLimit; ++i)
	{
		v_enemy_shots.emplace_back();
		v_enemy_shots.back().PtrEntity = entity_pool.CreateEntity();
		v_enemy_shots.back().PtrEntity->SetTexture(texture_sprite);
		v_enemy_shots.back().PtrEntity->CreateRectangle(XMFLOAT2(110, 40));
		v_enemy_shots.back().PtrEntity->SetRectangleUVRange(XMFLOAT2(110, 0), XMFLOAT2(110, 40));
		v_enemy_shots.back().PtrEntity->Sampler = ESampler::Linear;
		v_enemy_shots.back().PtrEntity->Visible = false;
		v_enemy_shots.back().PtrEntity->SetCollisionBox(XMFLOAT2(1, 10));

		entity_pool.AddEnemyShotEntity(v_enemy_shots.back().PtrEntity);
	}

	vector<SEnemy> v_enemy_ships{};
	for (size_t i = 0; i < neon_invader.KMaxEnemyLimit; ++i)
	{
		v_enemy_ships.emplace_back();
		v_enemy_ships.back().PtrEntity = entity_pool.CreateEntity();
		v_enemy_ships.back().PtrEntity->SetTexture(texture_sprite);
		v_enemy_ships.back().PtrEntity->CreateRectangle(XMFLOAT2(110, 80));
		v_enemy_ships.back().PtrEntity->Sampler = ESampler::Linear;
		v_enemy_ships.back().PtrEntity->Visible = false;

		entity_pool.AddEnemyEntity(v_enemy_ships.back().PtrEntity);
	}

	vector<SItem> v_items{};
	for (size_t i = 0; i < neon_invader.KMaxItemLimit; ++i)
	{
		v_items.emplace_back();

		v_items.back().Dead = true;
		CEntity*& entity{ v_items.back().PtrEntity };
		{
			entity = entity_pool.CreateEntity();

			entity->SetTexture(texture_itemset);
			entity->CreateRectangle(XMFLOAT2(60, 60));
			entity->SetRectangleUVRange(XMFLOAT2(0, 0), XMFLOAT2(60, 60));
			entity->SetCollisionBox(XMFLOAT2(26, 26));
			entity->Sampler = ESampler::Linear;
			entity->Visible = false;
		}

		entity_pool.AddItemEntity(entity);
	}

	CEntity* entity_main_ship{ entity_pool.CreateEntity() };
	{
		entity_main_ship->SetTexture(texture_sprite);
		entity_main_ship->CreateRectangle(XMFLOAT2(110, 80));
		entity_main_ship->SetRectangleUVRange(XMFLOAT2(0, 40), XMFLOAT2(110, 80));
		entity_main_ship->Sampler = ESampler::Linear;
		entity_main_ship->Visible = false;
		entity_main_ship->SetCollisionBox(XMFLOAT2(27, 20));
	}

	vector<SEffect> v_effects{};
	for (size_t i = 0; i < neon_invader.KMaxEffectLimit; ++i)
	{
		v_effects.emplace_back();

		v_effects.back().Dead = true;

		CEntity*& entity{ v_effects.back().PtrEntity };
		{
			entity = entity_pool.CreateEntity();

			XMFLOAT2 rect{ XMFLOAT2(196, 190) };
			entity->SetTexture(texture_explosion);
			entity->CreateRectangle(rect);
			entity->ShouldCollide = false;
			entity->Visible = false;

			SAnimation* animation{ entity->AddAnimation("explode") };
			animation->ShouldRepeat = false;
			animation->vFrames.emplace_back(XMFLOAT2(0, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(196, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(392, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(588, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(784, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(980, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1176, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1372, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1568, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1764, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(1960, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(2156, 0), rect);
			animation->vFrames.emplace_back(XMFLOAT2(2352, 0), rect);

			entity->SetAnimation("explode");
		}
	}

	entity_pool.SetMainSpriteEntity(entity_main_ship);

	CStageSetLoader stage_set_loader{};
	stage_set_loader.LoadStageSetFromFile(KAssetDir + "stage_set.txt");

	neon_invader.SetGameData(
		stage_set_loader.GetStageSetData(), entity_main_ship, v_enemy_ships, v_main_ship_shots, v_enemy_shots, v_effects, v_items);

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
	int main_ship_initial_life{ 3 };

	neon_invader.InitAudio(KAssetDir);

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
				if (turn_left) entity_main_ship->RotationAngle += 0.02f;
				if (turn_right) entity_main_ship->RotationAngle -= 0.02f;
				if (move_forward) entity_main_ship->MoveForward(0.7f);
				if (move_backward) entity_main_ship->MoveBackward(0.4f);
				if (move_left) entity_main_ship->MoveLeft(0.4f);
				if (move_right) entity_main_ship->MoveRight(0.4f);

				if (entity_main_ship->RotationAngle >= XM_2PI) entity_main_ship->RotationAngle = 0.0f;
				if (entity_main_ship->RotationAngle <= -XM_2PI) entity_main_ship->RotationAngle = 0.0f;
				entity_main_ship->WorldPosition.x = max(entity_main_ship->WorldPosition.x, -KWindowSize.x / 2);
				entity_main_ship->WorldPosition.x = min(entity_main_ship->WorldPosition.x, KWindowSize.x / 2);
				entity_main_ship->WorldPosition.y = max(entity_main_ship->WorldPosition.y, -KWindowSize.y / 2);
				entity_main_ship->WorldPosition.y = min(entity_main_ship->WorldPosition.y, KWindowSize.y / 2);

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
				neon_invader.AnimateEffects();

				timer_animation = time_now_microsec;
			}

			directx.BeginRendering(KClearColor);

			obj_bg.Draw();

			if (neon_invader.IsGameRunning() && should_draw_guildeline)
			{
				obj_guideline.WorldPosition = entity_main_ship->WorldPosition;
				obj_guideline.RotationAngle = entity_main_ship->RotationAngle;
				obj_guideline.Draw();
			}

			entity_pool.ApplyPhysics(delta_time);
			entity_pool.DrawEntitiesInAddedOrder();

			if (should_show_title)
			{
				if (GetAsyncKeyState(VK_RETURN) < 0)
				{
					should_show_title = false;
					entity_main_ship->Visible = true;

					neon_invader.InitGame(main_ship_initial_life);
					neon_invader.SetStage(0);
				}

				obj_title.Draw();
			}
			else
			{
				ascii_renderer.RenderText("Stage: " + to_string(neon_invader.GetStage() + 1) + "/" + to_string(neon_invader.GetMaxStage() + 1),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 010.0f));
				ascii_renderer.RenderText("Life : " + to_string(neon_invader.GetLife()),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 040.0f));
				ascii_renderer.RenderText("Enemy: " + to_string(neon_invader.GetEnemyCount()) + "/" + to_string(neon_invader.GetMaxEnemyCount()),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 070.0f));
				ascii_renderer.RenderText("Shots: " + to_string(neon_invader.GetShotCount()) + "/" + to_string(neon_invader.GetMaxShotCount()),
					XMFLOAT2(-KWindowSize.x / 2 + 10.0f, KWindowSize.y / 2 - 100.0f));
			}
			
			if (should_show_game_over)
			{
				if (GetAsyncKeyState(VK_RETURN) < 0)
				{
					should_show_game_over = false;
					entity_main_ship->Visible = true;

					neon_invader.InitGame(main_ship_initial_life);
					neon_invader.SetStage(0);
				}

				obj_game_over.Draw();
			}
			else if (should_show_completed)
			{
				if (GetAsyncKeyState(VK_RETURN) < 0)
				{
					should_show_completed = false;

					neon_invader.InitGame(main_ship_initial_life);
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
					entity_main_ship->Visible = false;
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