// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPawn.h"
#include "../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h"
#include "BulletActor.h"

// Sets default values
APlayerPawn::APlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector dir = FVector(0, h, v).GetSafeNormal();
	SetActorLocation(GetActorLocation() + dir * speed * DeltaTime);
}

// Called to bind functionality to input
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &APlayerPawn::OnAxisHorizontal);

	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &APlayerPawn::OnAixsVertical);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &APlayerPawn::OnActionFire);
}

void APlayerPawn::OnAxisHorizontal(float value)
{
	h = value;
}

void APlayerPawn::OnAixsVertical(float value)
{
	v = value;
}

void APlayerPawn::OnActionFire()
{
	FTransform t = GetActorTransform();
	FVector newLoc = t.GetLocation() + GetActorUpVector() * 100;
	t.SetLocation(newLoc);

	GetWorld()->SpawnActor<ABulletActor>(ABulletActor::StaticClass(), t);
}

