// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScriptedEvent.generated.h"

UENUM(BlueprintType)
enum class EEventState : uint8 {
	//Event is yet to begin
	Idle			UMETA(DisplayName = "Idle"),
	//Event has been started
	Started			UMETA(DisplayName = "Started"),
	//Event is finished
	Finished        UMETA(DisplayName = "Finished"),
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCompleteEventSignature);

UCLASS()
class VR_HORROR_Y3_API AScriptedEvent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScriptedEvent();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Start scripted event
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	virtual void StartEvent(bool bForceChange);

	//Reset the event state
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	virtual void ResetEvent();

	//Complete scripted event
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	virtual void CompleteEvent(bool bForceChange);

	//Returns the name of this event
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	FName GetEventName();

	//Returns the current state of this event
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	EEventState GetEventState();

	//Delegate to trigger when completing this scripted event
	UPROPERTY(BlueprintAssignable)
	FCompleteEventSignature EventDelegate;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Event gets called when scripted event has been started
	UFUNCTION(BlueprintImplementableEvent, Category = "ScriptedEvents")
	void OnEventStarted();

	//Event get called when scripted event has been completed
	UFUNCTION(BlueprintImplementableEvent, Category = "ScriptedEvents")
	void OnEventCompleted();

	UPROPERTY(EditAnywhere, Category="Event Details")
	FName EventName;

private:	

	UPROPERTY(VisibleAnywhere, Category = "Event Details")
	EEventState EventState;

};
