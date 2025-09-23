// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactibles/VRDialComponent.h"
#include "VRValveComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent), ClassGroup = (VRExpansionPlugin))
class VR_HORROR_Y3_API UVRValveComponent : public UVRDialComponent
{
	GENERATED_BODY()
	
	public:
		UVRValveComponent(const FObjectInitializer& ObjectInitializer);

		// Event triggered on the interfaced object when gripped
		void OnGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation) override;
		void OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed) override;

		void TickGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation, float DeltaTime) override;

		bool AllowsMultipleGrips_Implementation() override;

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

		void SetIsObjectLocked(bool state);
		
		bool GetInteractionCompleted();
	protected:
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRValveComponent")
			bool RequiredDualHolding;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRValveComponent")
			bool IsInteractionOnSelf;
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VRValveComponent")
			float PercentageRequiredToUnlock;
		UPROPERTY(EditAnywhere, Category = "VRValveComponent", meta = (EditCondition = "!IsInteractionOnSelf"))
			class AActor* InteractionObject;

		int HandsInteractCounter;
	private:
		float CompletionPercentage;
		bool InteractionCompleted;
		bool IsObjectLocked;
};
