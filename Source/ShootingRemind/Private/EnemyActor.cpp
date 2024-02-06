// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "PlayerPawn.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 충돌체
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

	// 외관
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	// 메시와 재질을 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT("/Script/Engine.Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
		if (tempMaterial.Succeeded())
		{
			meshComp->SetMaterial(0, tempMaterial.Object);
		}
	}
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	dir = -GetActorUpVector();
	// 만약 랜덤값이 30%라면 dir을 플레이어 방향으로 하고싶다.
	float randValue = FMath::RandRange(0, 9);
	if (randValue < 3)
	{
		auto target = GetWorld()->GetFirstPlayerController()->GetPawn();
		dir = target->GetActorLocation() - this->GetActorLocation();
		dir.Normalize();// 단위벡터, 벡터의 정규화 : 벡터의 크기를 1로 만든다.
	}
}

void AEnemyActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// 만약 부딪힌것이 플레이어라면
	if (OtherActor->IsA<APlayerPawn>())
	{
		// 너죽고 나죽고 하고싶다.
		OtherActor->Destroy();
		this->Destroy();
	}
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
}

