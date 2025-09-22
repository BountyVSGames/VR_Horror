// Fill out your copyright notice in the Description page of Project Settings.


#include "SeamlessTeleportation/Teleporter.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "GameFramework/Character.h"
#include "Player/InteractPlayer.h"

// Sets default values
ATeleporter::ATeleporter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create and set all components for this actor

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(FName("Root Scene Component"));

	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));

	ClassGizmo = CreateDefaultSubobject<UBillboardComponent>(FName("Gizmo"));

	SceneRootComponent->SetupAttachment(RootComponent);

	ClassGizmo->SetupAttachment(SceneRootComponent);

	//Set some default collision box settings
	CollisionMesh->SetGenerateOverlapEvents(true);
	CollisionMesh->SetCollisionProfileName(TEXT("Trigger"), false);
	CollisionMesh->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnComponentOverlap);

	CollisionMesh->SetupAttachment(SceneRootComponent);
}

// Called when the game starts or when spawned
void ATeleporter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATeleporter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeleporter::DrawLineToTargetLocation()
{
	if (TeleportToActor)
		DrawDebugLine(GetWorld(), GetActorLocation(), TeleportToActor->GetActorLocation(), FColor::Green, false, 5.f, 10, 2.f);
	else
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Teleporter does not have destination actor defined"));
}

void ATeleporter::FlushDebugLines()
{
	FlushPersistentDebugLines(GetWorld());
}

void ATeleporter::OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if overlapping actor is the player.

	AInteractPlayer* PlayerCharacter = Cast<AInteractPlayer>(OtherActor);
	if (PlayerCharacter)
	{
		//Get distance from player to trigger
		FVector TempDistance;
		TempDistance = PlayerCharacter->GetActorLocation() - GetActorLocation();
		//Calculate destination coordinates
		FVector TempCoords;

		if (TeleportToActor)
		{
			TempCoords = TeleportToActor->GetActorLocation() + TempDistance;
		}
		else
		{
			TempCoords = FVector(0.f, 0.f, 0.f);
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Teleporter does not have destination actor defined. Teleported to (0,0,0) instead"));
		}

		//Teleport player to coordinates + offset
		PlayerCharacter->SetActorLocation(TempCoords, false, nullptr, ETeleportType::TeleportPhysics);
	}
	
}



