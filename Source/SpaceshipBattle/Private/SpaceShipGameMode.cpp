// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipGameMode.h"
#include "SpaceShip.h"

ASpaceShipGameMode::ASpaceShipGameMode()
{
	DefaultPawnClass = ASpaceShip::StaticClass();
}

void ASpaceShipGameMode::IncreaseScore()
{
	Score++;
}

int ASpaceShipGameMode::GetScore()
{
	return Score;
}
