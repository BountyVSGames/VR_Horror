// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScriptedEvent.h"

#include "ScriptedEventsManager.generated.h"

UCLASS()
class VR_HORROR_Y3_API AScriptedEventsManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScriptedEventsManager();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Start the current list of scripted events
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	void StartEvents();

	//Trigger the next scripted event in the list
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	void NextEvent(bool bForceChange);

	//Add scripted Event to list of events
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	void AddEvent(AScriptedEvent* ScriptedEvent);

	//Resets all bound events
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	void ResetAllEvents();

	//Clears list of scripted events
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	void ClearEvents();

	//Returns the name of the current active event
	UFUNCTION(BlueprintCallable, Category = "ScriptedEvents")
	FName GetCurrentEvent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Called when scripted event has been completed
	UFUNCTION()
	void EventCallBack();

	//Called when the first event is started
	UFUNCTION(BlueprintImplementableEvent, Category = "ScriptedEvents")
	void OnEventsStarted();

	//Called when the next event is started
	UFUNCTION(BlueprintImplementableEvent, Category = "ScriptedEvents")
	void OnEventsNext();

	//Called when the last event has been completed
	UFUNCTION(BlueprintImplementableEvent, Category = "ScriptedEvents")
	void OnEventsCompleted();

	//List of all assigned scripted events
	UPROPERTY(EditAnywhere, Category = "ScriptedEvents")
	TArray<AScriptedEvent*> ScriptedEvents;

private:	

	//Bind all delegates to the callback function
	void BindDelegatesToCallback();

	//Index of all events in the array
	int CurrentEventIndex;
};
