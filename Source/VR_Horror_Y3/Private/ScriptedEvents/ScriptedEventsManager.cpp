// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptedEvents/ScriptedEventsManager.h"
#include <Kismet/GameplayStatics.h>

// Sets default values
AScriptedEventsManager::AScriptedEventsManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CurrentEventIndex = 0;
}

// Called when the game starts or when spawned
void AScriptedEventsManager::BeginPlay()
{
	Super::BeginPlay();
	CurrentEventIndex = 0;

	//Reverse trough the loop and check for empty / invalid elements
	for ( size_t i = ScriptedEvents.Num(); i --> 0;)
	{
		if (!IsValid(ScriptedEvents[i])) {
			ScriptedEvents.RemoveAt(i);
		}
	}
}

void AScriptedEventsManager::EventCallBack()
{
	NextEvent(false);
}

void AScriptedEventsManager::BindDelegatesToCallback()
{
	//Bind Callback function to scripted events delegates
	for (size_t i = 0; i < ScriptedEvents.Num(); i++)
	{
		if (IsValid(ScriptedEvents[i]))
		{
			if (!ScriptedEvents[i]->EventDelegate.IsBound()) {
				ScriptedEvents[i]->EventDelegate.AddDynamic(this, &AScriptedEventsManager::EventCallBack);
			}
		}
	}
}

// Called every frame
void AScriptedEventsManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AScriptedEventsManager::StartEvents()
{
	if (ScriptedEvents.Num() > 0) {
		BindDelegatesToCallback();

		ScriptedEvents[CurrentEventIndex]->StartEvent(false);

		OnEventsStarted();
	}
}

void AScriptedEventsManager::NextEvent(bool bForceChange)
{
	//Check for events
	if (ScriptedEvents.Num() > 0) 
	{
		if (bForceChange) 
		{
			ScriptedEvents[CurrentEventIndex]->CompleteEvent(true);
			return;
		}
		//Check if current event is finished
		if (ScriptedEvents[CurrentEventIndex]->GetEventState() == EEventState::Finished)
		{
			//Check if it is within range
			if (CurrentEventIndex < ScriptedEvents.Num() - 1) {
				CurrentEventIndex++;
				ScriptedEvents[CurrentEventIndex]->StartEvent(false);
				OnEventsNext();
				return;
			}
			{
				//Call Scripted Events completed
				OnEventsCompleted();
			}
		}
	}
}

void AScriptedEventsManager::AddEvent(AScriptedEvent* ScriptedEvent)
{
	//Add to the list
	ScriptedEvents.Add(ScriptedEvent);
	//Subscribe to callback function
	ScriptedEvent->EventDelegate.AddDynamic(this, &AScriptedEventsManager::EventCallBack);
}

void AScriptedEventsManager::ResetAllEvents()
{
	if (ScriptedEvents.Num() > 0)
	{
		for (size_t i = 0; i < ScriptedEvents.Num(); i++)
		{
			ScriptedEvents[i]->ResetEvent();
		}
	}
}

void AScriptedEventsManager::ClearEvents()
{
	ScriptedEvents.Empty();
}

FName AScriptedEventsManager::GetCurrentEvent()
{
	if (ScriptedEvents.Num() > 0) {
		return ScriptedEvents[CurrentEventIndex]->GetEventName();
	}
	return FName("No Events available!");
}
