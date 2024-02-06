// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "PlayerPawn.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// �浹ü
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("boxComp"));
	SetRootComponent(boxComp);

	// �ܰ�
	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));
	meshComp->SetupAttachment(RootComponent);

	// �޽ÿ� ������ �ε��ؼ� �����ϰ�ʹ�.
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
	// ���� �������� 30%��� dir�� �÷��̾� �������� �ϰ�ʹ�.
	float randValue = FMath::RandRange(0, 9);
	if (randValue < 3)
	{
		auto target = GetWorld()->GetFirstPlayerController()->GetPawn();
		dir = target->GetActorLocation() - this->GetActorLocation();
		dir.Normalize();// ��������, ������ ����ȭ : ������ ũ�⸦ 1�� �����.
	}
}

void AEnemyActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	// ���� �ε������� �÷��̾���
	if (OtherActor->IsA<APlayerPawn>())
	{
		// ���װ� ���װ� �ϰ�ʹ�.
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

