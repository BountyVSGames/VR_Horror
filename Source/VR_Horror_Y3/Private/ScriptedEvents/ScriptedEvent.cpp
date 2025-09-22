// Fill out your copyright notice in the Description page of Project Settings.


#include "ScriptedEvents/ScriptedEvent.h"
#include <ScriptedEvents/ScriptedEventsManager.h>
#include <Kismet/GameplayStatics.h>

// Sets default values
AScriptedEvent::AScriptedEvent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EventState = EEventState::Idle;

}

// Called when the game starts or when spawned
void AScriptedEvent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AScriptedEvent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AScriptedEvent::StartEvent(bool bForceChange)
{
	if (bForceChange) 
	{
		EventState = EEventState::Started;
		//Trigger started event
		OnEventStarted();
		return;
	}

	if (EventState != EEventState::Finished && EventState != EEventState::Started)
	{
		EventState = EEventState::Started;
		//Trigger started event
		OnEventStarted();
		return;
	}
}

void AScriptedEvent::ResetEvent()
{
	EventState = EEventState::Idle;
}

void AScriptedEvent::CompleteEvent(bool bForceChange)
{
	if (bForceChange) {
		EventState = EEventState::Finished;

		//Broadcast delegate
		EventDelegate.Broadcast();

		//Trigger completed event
		OnEventCompleted();

		return;
	}

	if (EventState == EEventState::Started) {
		EventState = EEventState::Finished;

		//Broadcast delegate
		EventDelegate.Broadcast();

		//Trigger completed event
		OnEventCompleted();
		return;
	}
}


FName AScriptedEvent::GetEventName()
{
	return EventName;
}

EEventState AScriptedEvent::GetEventState()
{
	return EventState;
}

