// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/InteractPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetStringLibrary.h"
#include "NavigationSystem.h"
#include "Components/CapsuleComponent.h"

void AInteractPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* playerController = Cast<APlayerController>(GetController());

	UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
	if (subSystem)
	{
		//Value to determine how far the player needs to press the grip button
		GripThresshold = 0.5f;

		subSystem->AddMappingContext(InteractionMappingContext, 0);
		subSystem->AddMappingContext(SmoothMappingContext, 0);
	}
}

//UTILITY
void AInteractPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//Interaction
	enhancedInputComponent->BindAction(RightGripAction, ETriggerEvent::Triggered, this, &AInteractPlayer::InteractRightController);
	enhancedInputComponent->BindAction(LeftGripAction, ETriggerEvent::Triggered, this, &AInteractPlayer::InteractLeftController);

	enhancedInputComponent->BindAction(RightGripAction, ETriggerEvent::Completed, this, &AInteractPlayer::StopInteractRightController);
	enhancedInputComponent->BindAction(LeftGripAction, ETriggerEvent::Completed, this, &AInteractPlayer::StopInteractLeftController);
}

//INTERACTION
void AInteractPlayer::InteractLeftController(const FInputActionValue& value)
{
	LeftHandGripValue = value.Get<float>();

	Interact(value, EControllerHand::Left);
}
void AInteractPlayer::InteractRightController(const FInputActionValue& value)
{
	RightHandGripValue = value.Get<float>();

	Interact(value, EControllerHand::Right);
}

void AInteractPlayer::StopInteractLeftController()
{
	LeftHandGripValue = 0.f;

	Interact(0.0f, EControllerHand::Left);
}
void AInteractPlayer::StopInteractRightController()
{
	RightHandGripValue = 0.f;

	Interact(0.0f, EControllerHand::Right);
}

void AInteractPlayer::Interact(const FInputActionValue& value, EControllerHand ControllerHand)
{
	const float currentGrip = value.Get<float>();

	switch (ControllerHand)
	{
	case EControllerHand::Left:
		if ((LeftGrippingState == EGrippingState::None) && currentGrip >= GripThresshold)
		{
			GripObject(ControllerHand);
		}
		else if (LeftGrippingState == EGrippingState::GripAndHold && currentGrip < GripThresshold)
		{
			StopGrippingObject(ControllerHand);
		}
		else if (LeftGrippingState == EGrippingState::GripNotHold && currentGrip < GripThresshold)
		{
			LeftGrippingState = EGrippingState::None;
		}
		break;
	case EControllerHand::Right:
		if ((RightGrippingState == EGrippingState::None || RightGrippingState == EGrippingState::GripNotHold) && currentGrip >= GripThresshold)
		{
			GripObject(ControllerHand);
		}
		else if (RightGrippingState == EGrippingState::GripAndHold && currentGrip < GripThresshold)
		{
			StopGrippingObject(ControllerHand);
		}
		else if (RightGrippingState == EGrippingState::GripNotHold && currentGrip < GripThresshold)
		{
			RightGrippingState = EGrippingState::None;
		}
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Interaction triggered on neither left or right hand."));
		UE_LOG(LogTemp, Error, TEXT("Interaction event triggered by neither hands"));
	}
}

void AInteractPlayer::GripObject(EControllerHand ControllerHand)
{
	UGripMotionControllerComponent* controller;
	UBoxComponent* controllerBox;

	switch (ControllerHand)
	{
	case EControllerHand::Left:
		controller = LeftMotionController.Get();
		controllerBox = controller->InteractionTrigger;

		LeftGrippingState = EGrippingState::GripNotHold;
		break;
	case EControllerHand::Right:
		controller = RightMotionController.Get();
		controllerBox = controller->InteractionTrigger;

		RightGrippingState = EGrippingState::GripNotHold;
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Interaction triggered on neither left or right hand."));
		UE_LOG(LogTemp, Error, TEXT("Interaction event triggered by neither hands"));
		return;
	}

	check(controllerBox);

	TArray< UPrimitiveComponent*> allOverlappingObjects;
	controllerBox->GetOverlappingComponents(allOverlappingObjects);

	for (int i = 0; i < allOverlappingObjects.Num(); i++)
	{
		AActor* rootActor = allOverlappingObjects[i]->GetAttachmentRootActor();

		if (allOverlappingObjects[i]->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass()) 
			|| (rootActor && rootActor->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass())))
		{
			switch (ControllerHand)
			{
			case EControllerHand::Left:
				AttachedObjectLeftController = allOverlappingObjects[i];
				LeftGrippingState = EGrippingState::GripAndHold;
				break;
			case EControllerHand::Right:
				AttachedObjectRightController = allOverlappingObjects[i];
				RightGrippingState = EGrippingState::GripAndHold;
				break;
			default:
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Interaction triggered on neither left or right hand."));
				UE_LOG(LogTemp, Error, TEXT("Interaction event triggered by neither hands"));
				return;
			}

			controller->GripObjectByInterface(allOverlappingObjects[i], allOverlappingObjects[i]->GetComponentTransform());
			return;
		}
	}
}
void AInteractPlayer::StopGrippingObject(EControllerHand ControllerHand)
{
	UGripMotionControllerComponent* controller;

	if (ControllerHand == EControllerHand::Left && AttachedObjectLeftController != nullptr)
	{
		controller = LeftMotionController.Get();

		if (!controller->DropObjectByInterface(AttachedObjectLeftController))
		{
			controller->DropObjectByInterface(AttachedObjectLeftController->GetAttachmentRootActor());
		}

		LeftGrippingState = EGrippingState::None;
	}
	else if (ControllerHand == EControllerHand::Right && AttachedObjectRightController != nullptr)
	{
		controller = RightMotionController.Get();

		if (!controller->DropObjectByInterface(AttachedObjectRightController))
		{
			controller->DropObjectByInterface(AttachedObjectRightController->GetAttachmentRootActor());
		}

		RightGrippingState = EGrippingState::None;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("ERROR: Drop interaction event triggered while not containing a object"));
		UE_LOG(LogTemp, Error, TEXT("Drop interaction event triggered while not containing a object"));
	}
}