// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpaceShip.generated.h"


class UStaticMeshComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class APlayerController;
class ABullet;
class USoundCue;
class UParticleSystemComponent;
class UParticleSystem;


UCLASS()
class SPACESHIPBATTLE_API ASpaceShip : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASpaceShip();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		bool bIsForward;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Default")
		bool bIsRight;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialization
	void Init();

	// Ship rotation changes with mouse cursor
	void LookAtCursor();

	void Move(float DeltaTime);

	// Up and down
	void MoveForward(float Value);

	// Left and right
	void MoveRight(float Value);

	void Fire();

	void StartFire();

	void EndFire();

	void RestartLevel();

	void OnDeath();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USceneComponent *RootSceneComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USphereComponent *CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		UStaticMeshComponent *ShipSM;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		USpringArmComponent *SpringComp;

	UPROPERTY(VisibleAnywhere, Category = "Component")
		UCameraComponent *CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		USceneComponent *SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		UParticleSystemComponent*ThrusterParticleComp;
	
	UPROPERTY(EditAnywhere, Category = "Particle")
		UParticleSystem* ExplosionParticle;

	APlayerController *PC;

	FTimerHandle TimerHandle_BetweenShot;

	FTimerHandle TimerHandle_Restart;

	bool bDead;

	// whether the ship is moving

	bool bForwardMove;
	
	bool bRightMove;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	bool GetBDead() { return bDead; };

public:

	UPROPERTY(EditAnywhere, Category = "Default", BlueprintReadWrite)
		float Speed;

	UPROPERTY(EditAnywhere, Category = "Default")
		TSubclassOf<ABullet> Bullet;

	UPROPERTY(EditAnywhere, Category = "Default")
		float BetweenShot;

	// Explosion effect
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue *GameOverCue;

	// Firing sound effect
	UPROPERTY(EditAnywhere, Category = "Sound")
		USoundCue *ShootCue;

};
