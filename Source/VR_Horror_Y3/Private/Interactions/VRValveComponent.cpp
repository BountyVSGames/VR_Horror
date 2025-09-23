// Fill out your copyright notice in the Description page of Project Settings.
#include "Interactions/VRValveComponent.h"
#include "GripMotionControllerComponent.h"
#include "Interactions/InteractionInterface.h"
#include "interactions/VRDoorComponent.h"

UVRValveComponent::UVRValveComponent(const FObjectInitializer& ObjectInitializer) : UVRDialComponent(ObjectInitializer)
{
	InteractionCompleted = false;

	PercentageRequiredToUnlock = 90.f;
	CompletionPercentage = 0.f;
	HandsInteractCounter = 0;

	ClockwiseMaximumDialAngle = 360.f;
	CClockwiseMaximumDialAngle = 0.f;
	bUseRollover = true;

	RotationScaler = 1.0f;
	DialRotationAxis = EVRInteractibleAxis::Axis_X;
}

void UVRValveComponent::OnGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation)
{
	HandsInteractCounter++;

	if (RequiredDualHolding && HandsInteractCounter == 2)
	{
		UVRDialComponent::OnGrip_Implementation(GrippingController, GripInformation);
	}
	else if (!RequiredDualHolding)
	{
		UVRDialComponent::OnGrip_Implementation(GrippingController, GripInformation);
	}
}
void UVRValveComponent::OnGripRelease_Implementation(UGripMotionControllerComponent* ReleasingController, const FBPActorGripInformation& GripInformation, bool bWasSocketed)
{
	InteractionCompleted = false;

	if (HandsInteractCounter > 0)
	{
		HandsInteractCounter--;
	}

	UVRDialComponent::OnGripRelease_Implementation(ReleasingController, GripInformation, bWasSocketed);
}

void UVRValveComponent::TickGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation, float DeltaTime)
{
	if (!IsObjectLocked)
	{
		if (RequiredDualHolding && HandsInteractCounter == 2)
		{
			//TODO: Add support that it follows the progress on both hands
			if (GrippingController->GetName().Contains("Left"))
			{
				UVRDialComponent::TickGrip_Implementation(GrippingController, GripInformation, DeltaTime);
			}
		}
		else if (!RequiredDualHolding)
		{
			UVRDialComponent::TickGrip_Implementation(GrippingController, GripInformation, DeltaTime);
		}

		CompletionPercentage = (CurrentDialAngle / ClockwiseMaximumDialAngle) * 100.0f;

		if (CompletionPercentage >= PercentageRequiredToUnlock && !InteractionCompleted)
		{
			InteractionCompleted = true;

			if (InteractionObject == nullptr && IsInteractionOnSelf)
			{
				if (GetOwner()->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
				{
					IInteractionInterface::Execute_InteractionCompleted(GetOwner());
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Interaction Failed - Self does not implement InteractionInterface"));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Interaction failed to trigger. Couldn't find InteractionInterface on self."));
				}
			}
			else if (InteractionObject == nullptr && !IsInteractionOnSelf)
			{
				UE_LOG(LogTemp, Error, TEXT("Interaction Failed - InteractionObject is not set"));
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Interaction failed to trigger. InteractionObject is not set"));
			}
			else
			{
				if (InteractionObject->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
				{
					IInteractionInterface::Execute_InteractionCompleted(InteractionObject);
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Interaction Failed - InteractionObject does not implement InteractionInterface"));
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Interaction failed to trigger. Couldn't find InteractionInterface on target object."));
				}
			}
		}
		else if (CompletionPercentage < 90.f && InteractionCompleted)
		{
			InteractionCompleted = false;
		}
	}
}

bool UVRValveComponent::AllowsMultipleGrips_Implementation()
{
	return true;
}

bool UVRValveComponent::GetInteractionCompleted()
{
	return (CompletionPercentage >= PercentageRequiredToUnlock);
}
void UVRValveComponent::SetIsObjectLocked(bool state)
{
	IsObjectLocked = state;
}