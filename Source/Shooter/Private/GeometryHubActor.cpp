// Fill out your copyright notice in the Description page of Project Settings.


#include "GeometryHubActor.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY_STATIC(LogGeometryHub,All,All)

// Sets default values
AGeometryHubActor::AGeometryHubActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGeometryHubActor::BeginPlay()
{
	Super::BeginPlay();
	
	//DoActorSpawn1();
	//DoActorSpawn2();
	DoActorSpawn3();
	DoActorSpawn4();
}

// Called every frame
void AGeometryHubActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogGeometryHub,Warning,TEXT("property pointer %i, is valid %i"),ProperrtyActor!=nullptr,IsValid(ProperrtyActor));
	UE_LOG(LogGeometryHub,Error,TEXT("none property %i, is valid %i"),NoneProperrtyActor!=nullptr,IsValid(NoneProperrtyActor));
}

void AGeometryHubActor::DoActorSpawn1()
{
	UWorld* World=GetWorld();
	if(World)
	{
		for(int32 i=0;i<10;i++)
		{
			const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator,FVector(100.0f*i,300.0f*i,300.0f));
			ABaseGeometryActor* Geometry=World->SpawnActor<ABaseGeometryActor>(GeometryClass,GeometryTransform);
			if(Geometry)
			{
				FGeometryData Data;
				Data.MoveType=FMath::RandBool()?EMovementType::Static:EMovementType::Sin;
				Geometry->SetGeometryData(Data);
			}
		}
	}
}

void AGeometryHubActor::DoActorSpawn2()
{
		UWorld* World=GetWorld();
		if(World)
		{
			for(int32 i=0 ; i<10 ; i++)
			{
				const FTransform GeometryTransform = FTransform(FRotator::ZeroRotator,FVector(100.0f*i,300.0f*i,700.0f));
				ABaseGeometryActor* Geometry=World->SpawnActorDeferred<ABaseGeometryActor>(GeometryClass,GeometryTransform);
				if(Geometry)
				{
					FGeometryData Data;
					Data.Color=FLinearColor::MakeRandomColor();
					Geometry->SetGeometryData(Data);
					Geometry->FinishSpawning(GeometryTransform);
				}
			}
		}
}

void AGeometryHubActor::DoActorSpawn3()
{
		UWorld* World=GetWorld();
			if(World)
			{
				for (const FGeometryPayLoad PayLoad:GeometryPayLoads)
				{
					ABaseGeometryActor* Geometry = World->SpawnActorDeferred<ABaseGeometryActor>(PayLoad.GeometryClass,PayLoad.InitialTransform);

					if(Geometry)
					{
						Geometry->SetGeometryData(PayLoad.Data);
						Geometry->OnColorChanged.AddDynamic(this, &AGeometryHubActor::OnColorChanged);
						Geometry->OnTimerFinished.AddUObject(this,&AGeometryHubActor::OnTimerFinished);
						Geometry->FinishSpawning(PayLoad.InitialTransform);
					}
				}
			}
}

void AGeometryHubActor::DoActorSpawn4()
{
	if(!GetWorld())return;
	FTransform GeometryTransform = FTransform(FRotator::ZeroRotator,FVector(700.0f,300.0f,300.0f));
	NoneProperrtyActor = GetWorld()->SpawnActor<ABaseGeometryActor>(GeometryClass,GeometryTransform);

	GeometryTransform = FTransform(FRotator::ZeroRotator,FVector(700.0f,700.0f,300.0f));
	ProperrtyActor = GetWorld()->SpawnActor<ABaseGeometryActor>(GeometryClass,GeometryTransform);
}

void AGeometryHubActor::OnColorChanged(const FLinearColor& Color, const FString& Name)
{
	UE_LOG(LogGeometryHub, Warning, TEXT("Actor name: %s Color %s"),*Name,*Color.ToString());
}

void AGeometryHubActor::OnTimerFinished(AActor* Actor)
{
	if(!Actor)return;
	UE_LOG(LogGeometryHub,Error,TEXT("Timer finished: %s"),*Actor->GetName());

	ABaseGeometryActor* Geometry=Cast<ABaseGeometryActor>(Actor);
	if(!Geometry)return;

	UE_LOG(LogGeometryHub,Display,TEXT("Cast is succes< amplitude %f"),Geometry->GetGeometryData().Amplitude);
	UE_LOG(LogGeometryHub,Display,TEXT("Cast is succes< Color %s"),*Geometry->GetGeometryData().Color.ToString());
	UE_LOG(LogGeometryHub,Display,TEXT("Cast is succes< Frequency %f"),Geometry->GetGeometryData().Frequency);
	UE_LOG(LogGeometryHub,Display,TEXT("Cast is succes< TimerRate %f"),Geometry->GetGeometryData().TimerRate);

	Geometry->Destroy();
	//Geometry->SetLifeSpan(2.0f);
}

