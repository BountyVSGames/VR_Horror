// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//Plugin Includes
#include "VRCharacter.h"
#include "GripMotionControllerComponent.h"

//Engine Includes
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InputCoreTypes.h"
#include "Components/BoxComponent.h"

//Other
#include "GrippingState.h"

#include "InteractPlayer.generated.h"

UENUM(BlueprintType)
enum class EMovementModePlayer : uint8 {
	E_SmoothMovement		UMETA(DisplayName = "Smooth Movement"),
	E_Teleportation			UMETA(DisplayName = "Teleportation"),
	E_Disabled				UMETA(DisplayName = "Disabled")
};

UENUM(BlueprintType)
enum class EActiveHand : uint8 {
	E_TeleportLeftHand		UMETA(DisplayName = "Teleport Left Hand"),
	E_TeleportRightHand     UMETA(DisplayName = "Teleport Right Hand")
};

UCLASS()
class VR_HORROR_Y3_API AInteractPlayer : public AVRCharacter
{
public:
	GENERATED_BODY()

	AInteractPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent);

	//Sets the player's movement mode
	UFUNCTION(BlueprintCallable, Category="PlayerCharacter|Movement")
	void SetInputMode(EMovementModePlayer movementMode) {};
	
	//Sets the active hand used for teleportation
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Movement|Teleportation")
	void SetTeleportationHand(EActiveHand activeHand) {};

	//Set whether or not the screen goes black on dash
	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter|Movement|Teleportation")
	void SetBlackout(bool newValue) { };

	//Get Current active Movement Mode
	UFUNCTION(BlueprintPure, Category = "PlayerCharacter|Movement")
	EMovementModePlayer GetMovementMode() { return EMovementModePlayer::E_Disabled; };

	//Get Current active Hand
	UFUNCTION(BlueprintPure, Category = "PlayerCharacter|Movement|Teleportation")
	EActiveHand GetTeleportationHand() { return EActiveHand::E_TeleportRightHand; };

protected:
	virtual void BeginPlay() override;
	//virtual void Tick(float DeltaSeconds) override;

	void SmoothMove(const FInputActionValue& value) {};
	void Rotate(const FInputActionValue& value) {};

	void TeleportLeft(const FInputActionValue& value) {};
	void TeleportRight(const FInputActionValue& value) {};

	void InteractLeftController(const FInputActionValue& value);
	void InteractRightController(const FInputActionValue& value);
	void Interact(const FInputActionValue& value, EControllerHand ControllerHand);

	void StopInteractLeftController();
	void StopInteractRightController();

	//Events

	//Event gets called once the player starts dashing (teleporting)
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerCharacter|Movement|Teleportation")
	void OnDashStarted();

	//Event gets called once the player starts using tracing an arc (teleporting)
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerCharacter|Movement|Teleportation")
	void OnTraceStarted();

	//Event gets called once the player is done tracing (teleporting)
	UFUNCTION(BlueprintImplementableEvent, Category = "PlayerCharacter|Movement|Teleportation")
	void OnTraceEnded();

	//Variables

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Interaction")
	UInputMappingContext* InteractionMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Interaction")
	UInputAction* RightGripAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Interaction")
	UInputAction* LeftGripAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement")
	UInputMappingContext* SmoothMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Actions")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Actions")
	UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Actions")
	UInputAction* LeftTeleportAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Actions")
	UInputAction* RightTeleportAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Options")
	bool bSnapTurningEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Options", meta = (ClampMin = "1", ClampMax = "180", UIMin = "1", UIMax = "180"))
	float SnapTurnDegrees = 45.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Options")
	float PlayerMovementSpeed = 150.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input|Movement|Options")
	float SmoothRotateSpeed = 40.f;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Movement|Options")
	bool EnableBlackoutOnDash = true;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Movement|Trace")
	bool bTraceHasStarted = false;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	float LeftHandGripValue;

	UPROPERTY(BlueprintReadOnly, Category = "Input")
	float RightHandGripValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Input|Interaction")
	EGrippingState LeftGrippingState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"), Category = "Input|Interaction")
	EGrippingState RightGrippingState;
	
	UPROPERTY(BlueprintReadWrite, Category = "Input|Movement|Variables")
	bool bIsValidTeleportLocation = false;

	UPROPERTY(BlueprintReadWrite, Category = "Input|Movement|Variables")
	FVector LerpDashTarget;

	UFUNCTION(BlueprintCallable)
	void UpdateRotation(USkeletalMeshComponent* hand, FRotator rot);

private:
	void GripObject(EControllerHand ControllerHand);
	void StopGrippingObject(EControllerHand ControllerHand);

	void SmoothTurn(float value) {};
	void SnapTurn(bool bIsRightTurn) {};
	void StopRotating() {};

	void StartDash() {};
	void StartTrace() {};

	void EndTrace() {};

	UPrimitiveComponent* AttachedObjectLeftController;
	UPrimitiveComponent* AttachedObjectRightController;

	bool bHasRotated = false;

	float GripValue;

	AActor* TeleportVisual;

	UPROPERTY(EditAnywhere, Category = "Input|Movement")
	EMovementModePlayer MovementMode;

	UPROPERTY(EditAnywhere, Category = "Input|Movement")
	EActiveHand TeleportationHand;
};
