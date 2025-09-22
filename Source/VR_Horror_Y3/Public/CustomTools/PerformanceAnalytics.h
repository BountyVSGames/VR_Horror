// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "PerformanceAnalytics.generated.h"

UENUM(BlueprintType)
enum class ECaptureType : uint8 {
	Sequence       UMETA(DisplayName = "Sequence"),
	FlyThrough        UMETA(DisplayName = "Fly Through"),
};

class UCurveFloat;

UCLASS()
class VR_HORROR_Y3_API APerformanceAnalytics : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerformanceAnalytics();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="Tools")
	void StartPerformanceCapture();

	//Time between player position change.
	UPROPERTY(EditAnywhere,Category="Tools|Sequence", meta = (ClampMin = "0.1", UIMin = "0.1"))
	float LoopDelay;

	//Coordinates the player will teleport to.
	UPROPERTY(EditAnywhere, Category = "Tools|Sequence", Meta = (MakeEditWidget = true))
	TArray<FVector> Coordinates;

	UPROPERTY(EditAnywhere, Category="Tools")
	ECaptureType CaptureEnum;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Start or stop the unreal profiler and show performance stats
	void ToggleProfiler(bool start);

	void StartSequence();

	void StartFlyThrough();
private:

	/**
	 * Call function on loop every x seconds
	 * @param rate timer duration (seconds)
	 * @param functionToCall Name of the function the timer will call
	 */
	void TimerFunction(float rate, FName functionToCall);

	//Teleport the play to the next available coordinate.
	UFUNCTION(Category="Tools")
	void TeleportNextPosition();

	FTimerHandle timeHandle;

	//Reference to the player
	APawn* PlayerPawn;

	//Index for going trough array of coordinates
	int CoordsIndex;

	//Is the profiler already running ?
	bool IsRunning;
};
