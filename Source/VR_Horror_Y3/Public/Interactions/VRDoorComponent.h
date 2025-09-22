// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/VRLeverComponent.h"
#include "VRValveComponent.h"
#include "InteractionInterface.h"
#include "VRDoorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup = (VRExpansionPlugin))
class VR_HORROR_Y3_API UVRDoorComponent : public UVRLeverComponent, public IInteractionInterface
{
	GENERATED_BODY()
public:
	UVRDoorComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "VRDoorComponent")
	void SetupDoor();

	UFUNCTION(BlueprintCallable, Category = "VRDoorComponent")
	void OpenDoor();

	UFUNCTION(BlueprintCallable, Category = "VRDoorComponent")
	void ForceDoorClose();

	void TickGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation, float DeltaTime) override;
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/*
	-- Required by IGameplayTagAssetInterface to have a body --
	-- TODO: Implement Gameplay Tags
	*/
	bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override { return false; };
	bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override { return false; };
	bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override { return false; };
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer = GameplayTags; }
	/*
		----------------------------------------------------------
	*/
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "VRDoorComponent", meta = (AllowPrivateAccess = "true"))
	UVRValveComponent* ValveComponent;
};
