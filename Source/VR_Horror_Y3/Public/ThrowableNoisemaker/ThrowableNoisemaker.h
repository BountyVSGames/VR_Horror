// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Grippables/GrippableStaticMeshActor.h"

#include "ThrowableNoisemaker.generated.h"

/**
 * 
 */
UCLASS()
class VR_HORROR_Y3_API AThrowableNoisemaker : public AGrippableStaticMeshActor
{
	GENERATED_BODY()
public:
	AThrowableNoisemaker(const FObjectInitializer& ObjectInitializer);
	AThrowableNoisemaker() = delete;
	
//	virtual void BeginPlay() override;
//	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//void Tick(float DeltaTime) override;



	//Code inherited and duplicated from IGameplayTagAssetInterface through AGrippableStaticMeshActor
	
	bool HasAnyMatchingGameplayTags(struct FGameplayTagContainer const& TagContainer)const
	{
		FGameplayTagContainer OwnedTags;
		GetOwnedGameplayTags(OwnedTags);

		return OwnedTags.HasAny(TagContainer);
	};
	bool HasMatchingGameplayTag(struct FGameplayTag TagToCheck)const
	{
		FGameplayTagContainer OwnedTags;
		GetOwnedGameplayTags(OwnedTags);

		return OwnedTags.HasTag(TagToCheck);
	};
	bool HasAllMatchingGameplayTags(struct FGameplayTagContainer const& TagContainer)const
	{
		FGameplayTagContainer OwnedTags;
		GetOwnedGameplayTags(OwnedTags);

		return OwnedTags.HasAll(TagContainer);
	};
		
	//Time between release and being able to start making noise.
	UPROPERTY(EditAnywhere, Category = "NoiseSettings")
	float ActivationTime = 0.5f;
	float TimeSinceDropped = 0.f;

	//Default is infinite (0); Max range at which the sound can be heard, multiplied by Loudness.Values <= 0 mean no limit(still limited by listener's range however
	UPROPERTY(EditAnywhere, Category = "NoiseSettings")
	float MaxRange = 0.f;

	//Loudness of the noise.If MaxRange is non - zero, modifies MaxRange, otherwise modifies the squared distance of the sensor's range
	UPROPERTY(EditAnywhere, Category = "NoiseSettings")
	float Loudness = 1.f;

	//The audio that gets played to the player goes here
	UPROPERTY(EditAnywhere, Category = "NoiseSettings")
	TObjectPtr<UAudioComponent> DistractionAudio;
	static FName DistractionAudioName;

	bool bDropped = false;
	bool bActivated = false;

	//when releasing an object, act like the pin has been pulled on a grenade
	virtual void OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed = false) override;
//	virtual void OnUsed_Implementation() override;

	//check what we hit and if something should happen
	void ReceiveHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit);

};
