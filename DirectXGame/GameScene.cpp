#include "GameScene.h"
#include "Math.h"

using namespace KamataEngine;

// エフェクトを生成
void GameScene::CreateEffect(const Vector3& position) {

	HitEffect* newHitEffect = HitEffect::Create(position);

	hitEffects_.push_back(newHitEffect);
}

GameScene::~GameScene() {

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	for (std::vector<WorldTransform*>& worldTransformLadderLine : worldTransformLadders_) {
		for (WorldTransform* worldTransformLadder : worldTransformLadderLine) {
			delete worldTransformLadder;
		}
	}
	worldTransformLadders_.clear();

	delete debugCamera_;
	delete modelSkydome_;
	delete mapChipField_;
	delete sprite_;
	delete model_;

	delete block_model_;
	
	delete ladder_model_;
	delete deathParticles_;
	delete deathParticle_model_;

	// 02_10 6枚目 敵クラス削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (HitEffect* hitEffect : hitEffects_) {
		delete hitEffect;
	}
}

void GameScene::Initialize() {

	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("sample.png");
	// スプライト生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// 3Dモデル生成
	model_ = Model::Create();
	// ワールドトランスフォーム初期化
	worldTransform_.Initialize();

	// カメラ初期化
	camera_.Initialize();

	// ブロックモデル
	block_model_ = Model::CreateFromOBJ("block");

	ladder_model_ = Model::CreateFromOBJ("ladder");
	
	
	//スカイドーム初期化
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("SkyDome", true);
	skydome_->Initialize(modelSkydome_, &camera_);

	// 02_04マップチップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	
	uint32_t height = mapChipField_->GetNumBlockVirtical();
	uint32_t width = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(height);
	worldTransformLadders_.resize(height);
	
	for (uint32_t i = 0; i < height; i++) {
		worldTransformBlocks_[i].resize(width, nullptr);
		worldTransformLadders_[i].resize(width, nullptr);
	}
	GenerateBlocks();

	//プレイヤー初期化
	player_ = new Player();
	player_model_ = Model::CreateFromOBJ("player");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	modelAttack_ = Model::CreateFromOBJ("attack_effect");
	player_->SetMapChipField(mapChipField_);
	player_->Initialize(player_model_, modelAttack_, &camera_, playerPosition);


	// 02_06カメラコントローラ スライド13枚目
	CController_ = new CameraController(); // 生成
	CController_->Initialize(&camera_);    // 初期化
	CController_->SetTarget(player_);      // 追従対象セット
	CController_->Reset();                 // リセット

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CController_->SetMovableArea(cameraArea);

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//エネミーモデル初期化
	enemy_model_ = Model::CreateFromOBJ("enemy");
	for (int32_t i = 0; i < 2; ++i) {
		Enemy* newEnemy = new Enemy();

		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(30 + i * 2, 18);

		newEnemy->Initialize(enemy_model_, &camera_, enemyPosition);

		newEnemy->SetGameScene(this);
		enemies_.push_back(newEnemy);
	}

	//パーティクル初期化
	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");
	particle_model_ = Model::CreateFromOBJ("particle");
	phase_ = Phase::kFadeIn;

	// 02_13 27枚目
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

	HitEffect::SetModel(particle_model_);
	HitEffect::SetCamera(&camera_);

}


void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:
		
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		break;
	}
}

void GameScene::GenerateBlocks() {

	uint32_t height = mapChipField_->GetNumBlockVirtical();
	uint32_t width = mapChipField_->GetNumBlockHorizontal();

	
	for (uint32_t i = 0; i < height; ++i) {
		for (uint32_t j = 0; j < width; j++) {
			MapChipType type = mapChipField_->GetMapChipTypeByIndex(j, i);

			// ブロックの生成
			if (type == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

				worldTransformBlocks_[i][j] = worldTransform;
			}
			
			//はしご生成
			else if (type == MapChipType::kLadder) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransform->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);

				worldTransformLadders_[i][j] = worldTransform;
			}
		}
	}
}

// ゲームシーン更新
void GameScene::Update() {

	// デスフラグの立ったエフェクトを削除
	hitEffects_.remove_if([](HitEffect* hitEffect) {
		if (hitEffect->IsDead()) {
			delete hitEffect;

			return true;
		}
		return false;
	});

	// 02_15 7枚目 デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		skydome_->Update();
		CController_->Update();

		// 自キャラの更新
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// UpdateBlocks();
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				// アフィン変換～DirectXに転送
				WorldTransformUpdate(*worldTransformBlock);
			}
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}
		break;
	case Phase::kPlay:
		skydome_->Update();
		CController_->Update();
		
		// 自キャラの更新
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				// アフィン変換～DirectXに転送
				WorldTransformUpdate(*worldTransformBlock);
			}
		}

		for (std::vector<WorldTransform*>& worldTransformLadderLine : worldTransformLadders_) {
			for (WorldTransform*& worldTransformLadder : worldTransformLadderLine) {
				if (!worldTransformLadder)
					continue;
				WorldTransformUpdate(*worldTransformLadder);
			}
		}

		CheckAllCollisions();

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}
		break;
	case Phase::kDeath:
		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}

		skydome_->Update();
		CController_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (deathParticles_) {
			deathParticles_->Update();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		skydome_->Update();
		CController_->Update();
		
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (HitEffect* hitEffect : hitEffects_) {
			hitEffect->Update();
		}

		break;
	}
}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3Dオブジェクト描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 自キャラの描画
	if (!player_->IsDead())
		player_->Draw();

	// 天球描画
	skydome_->Draw();

	size_t height = worldTransformBlocks_.size();
	size_t width = worldTransformBlocks_[0].size();

	// ブロックの描画
	for (size_t i = 0; i < height;i++) {
		for (size_t j = 0; j < width;j++) {
			if (worldTransformBlocks_[i][j]) {
				block_model_->Draw(*worldTransformBlocks_[i][j],camera_);
			}
		}
	}

	//はしご描画
	for (size_t i = 0; i < height; i++) {
		for (size_t j = 0; j < width; j++) {
			if (worldTransformLadders_[i][j]) {
				ladder_model_->Draw(*worldTransformLadders_[i][j], camera_);
			}
		}
	}

	// 02_09 12枚目 敵更新 → 02_10 7枚目で更新
	//	enemy_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 02_11 18枚目 デスパーティクルあれば描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	for (HitEffect* hitEffect : hitEffects_) {
		hitEffect->Draw();
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();

	// 02_13 28枚目
	fade_->Draw();
}

// 02_10 16枚目
void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {

			// コリジョン無効の敵はスキップ
			if (enemy->IsCollisionDisabled())
				continue;

			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}
bool GameScene::AreAllEnemiesDefeated() const {
	for (const Enemy* enemy : enemies_) {
		if (!enemy->IsDead())
			return false;
	}
	return true;
}
