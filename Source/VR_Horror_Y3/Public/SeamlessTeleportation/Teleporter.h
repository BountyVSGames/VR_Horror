// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleporter.generated.h"

class UBoxComponent;
class UBillboardComponent;

UCLASS()
class VR_HORROR_Y3_API ATeleporter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATeleporter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Gizmo drawer in engine
	UFUNCTION(CallInEditor)
	void DrawLineToTargetLocation();

	UFUNCTION(CallInEditor)
	void FlushDebugLines();

	//Coordinates to teleport towards
	UPROPERTY(EditAnywhere, Category = "SeamlessTP")
	AActor* TeleportToActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	//Function to call when overlapp happens
	UFUNCTION()
	void OnComponentOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//Gizmo to show in the editor (just visuals)
	UPROPERTY()
	TObjectPtr<UBillboardComponent> ClassGizmo;

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<USceneComponent> SceneRootComponent;

	//Box collider
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> CollisionMesh = nullptr;

	//Occlusion Handler
	UPROPERTY(VisibleDefaultsOnly) 
	TObjectPtr<UBoxComponent> OcclusionDisabler = nullptr;
};
