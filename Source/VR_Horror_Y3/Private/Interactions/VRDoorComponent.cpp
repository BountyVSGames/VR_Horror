// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactions/VRDoorComponent.h"

UVRDoorComponent::UVRDoorComponent(const FObjectInitializer& ObjectInitializer) : UVRLeverComponent::UVRLeverComponent(ObjectInitializer)
{
	LeverRotationAxis = EVRInteractibleLeverAxis::Axis_Z;
	LeverLimitPositive = 90.f;
	LeverLimitNegative = 0.f;

	bIsLocked = true;
	bAutoDropWhenLocked = true;

	LeverReturnTypeWhenReleased = EVRInteractibleLeverReturnType::RetainMomentum;
	MaxLeverMomentum = 500.f;
}

void UVRDoorComponent::TickGrip_Implementation(UGripMotionControllerComponent* GrippingController, const FBPActorGripInformation& GripInformation, float DeltaTime)
{
	UVRLeverComponent::TickGrip_Implementation(GrippingController, GripInformation, DeltaTime);

	if (!bIsLocked)
	{
		if (!ValveComponent->GetInteractionCompleted())
		{
			CheckAutoDrop(GrippingController, GripInformation);
			bIsLocked = true;
		}
	}
}
void UVRDoorComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	UVRLeverComponent::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!bIsLocked)
	{ 
		if (CurrentLeverAngle > 1.f)
		{
			ValveComponent->SetIsObjectLocked(true);
		}
		else
		{
			ValveComponent->SetIsObjectLocked(false);
		}
	}
}

void UVRDoorComponent::SetupDoor()
{
	ValveComponent = GetOwner()->FindComponentByClass<UVRValveComponent>();
}
void UVRDoorComponent::OpenDoor()
{
	bIsLocked = false;
}
void UVRDoorComponent::ForceDoorClose()
{
	ValveComponent->SetIsObjectLocked(true);
	bDenyGripping = true;

	LeverReturnTypeWhenReleased = EVRInteractibleLeverReturnType::ReturnToZero;
	ValveComponent->bLerpBackOnRelease = true;

	this->bIsLerping = true;
	ValveComponent->bIsLerping = true;

	this->SetComponentTickEnabled(true);
	ValveComponent->SetComponentTickEnabled(true);
}
