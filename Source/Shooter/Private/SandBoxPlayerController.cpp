// Fill out your copyright notice in the Description page of Project Settings.


#include "SandBoxPlayerController.h"
#include "SandBoxPawn.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogSandBoxPlayerControllerGeometry,All,All)

void ASandBoxPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if(InputComponent)
	{
		InputComponent->BindAction("ChangePawn",IE_Pressed,this,&ASandBoxPlayerController::ChangePawn);
	}
}

void ASandBoxPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ASandBoxPawn::StaticClass(),Pawns);
}

void ASandBoxPlayerController::ChangePawn()
{
	if(Pawns.Num() <= 1)return;
	
	ASandBoxPawn* CurrentPawn=Cast<ASandBoxPawn>(Pawns[CurrentPawnIndex]);
	CurrentPawnIndex=(CurrentPawnIndex+1)%Pawns.Num();
	if(!CurrentPawn) return;
	
	UE_LOG(LogSandBoxPlayerControllerGeometry,Error,TEXT("Change player pawn"));
	Possess(CurrentPawn);
	
	
}
