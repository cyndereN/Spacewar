// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceShipGameMode.generated.h"

/**
 * 
 */
UCLASS()
class SPACESHIPBATTLE_API ASpaceShipGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASpaceShipGameMode();

	void IncreaseScore();

	UFUNCTION(BlueprintCallable, Category = ShipGameMode)
		int GetScore();

protected:

	int Score;

};
