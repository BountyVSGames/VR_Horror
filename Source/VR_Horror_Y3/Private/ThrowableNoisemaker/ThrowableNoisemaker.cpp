// Fill out your copyright notice in the Description page of Project Settings.

#include "ThrowableNoisemaker/ThrowableNoisemaker.h"
#include "Components/AudioComponent.h"

FName AThrowableNoisemaker::DistractionAudioName(TEXT("DistractionAudio"));

AThrowableNoisemaker::AThrowableNoisemaker(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UOptionalRepStaticMeshComponent>(TEXT("StaticMeshComponent0")))
{
	DistractionAudio = CreateDefaultSubobject<UAudioComponent>(AThrowableNoisemaker::DistractionAudioName);
	ConstructorHelpers::FObjectFinder<UStaticMesh>MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	UStaticMesh* Asset = MeshAsset.Object;
	GetStaticMeshComponent()->SetStaticMesh(Asset);
	GetStaticMeshComponent()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);
};

//void AThrowableNoisemaker::Tick(float DeltaTime)
//{
//	//Allow GrippableStaticMeshActor to do whatever it does
//	AGrippableStaticMeshActor::Tick(DeltaTime);
//	if (true == bDropped)
//	{
//		TimeSinceDropped += DeltaTime;
//		if (TimeSinceDropped > ActivationTime)
//		{
//			bActivated = true;
//		}
//	}
//}

void AThrowableNoisemaker::OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed)
{
	bDropped = true;
	//Allow GrippableStaticMeshActor to do whatever it does
	Super::OnGripRelease_Implementation(ReleasingController, GripInformation, bWasSocketed);
};


void AThrowableNoisemaker::ReceiveHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	//stationary when hit
	if (!bSelfMoved) { return; }
	if (!bActivated) { return; }
	bool bAbort = false;

	// not sure if I should try with rootcomponent or hitcomponent
	if (EComponentMobility::Static != Other->GetRootComponent()->Mobility) 
	{ 
		//if the object that we hit didn't stop me, act like nothing happened
		if (GetVelocity().SquaredLength() > 0.1f)
		{
			return;
		}
		bAbort = true;
	}
	//resetting values -> hit something that stopped me
	bActivated = false;
	bDropped = false;
	TimeSinceDropped = 0.f;

	//if hit something that can move, don't make noise
	if (bAbort)
	{ 
		return;
	}

	//make noise
	FVector Location = this->GetActorLocation();
	MakeNoise(Loudness, Cast<APawn>(this), Location, MaxRange);
	DistractionAudio.Get()->Play();
}
