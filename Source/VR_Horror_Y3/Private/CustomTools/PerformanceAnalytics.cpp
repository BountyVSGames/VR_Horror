// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomTools/PerformanceAnalytics.h"

#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APerformanceAnalytics::APerformanceAnalytics()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CoordsIndex = 0;
	LoopDelay = 0.1f;
}

// Called when the game starts or when spawned
void APerformanceAnalytics::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APerformanceAnalytics::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APerformanceAnalytics::StartPerformanceCapture() {

	//Check if the user used function when in game
	if (GetWorld()->WorldType == EWorldType::PIE || GetWorld()->WorldType == EWorldType::Game)
	{
		GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, *FString::Printf(TEXT("In Game")));

		if(!IsRunning)
		{
			switch(CaptureEnum)
			{
			case ECaptureType::Sequence :
				StartSequence();
				break;

			case ECaptureType::FlyThrough:
				StartFlyThrough();
				break;

			default:
				StartSequence();
				break;
			}
			
		}

		return;
	}

	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, *FString::Printf(TEXT("Not running the game !")));

	return;
}

void APerformanceAnalytics::ToggleProfiler(bool start) {

	//Get the player to call console commands
	APlayerController* control = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	//Execute command to display stats on screen and record them

	FString command = "Stat UNIT";
	control->ConsoleCommand(command);

	command = "Stat FPS";
	control->ConsoleCommand(command);

	command = "Trace.Start";
	if (!start) 
	{
	command = "Trace.Stop";
	}
	
	control->ConsoleCommand(command);
}

void APerformanceAnalytics::StartSequence()
{
	if (!Coordinates.IsEmpty()) {

		IsRunning = true;

		PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

		ToggleProfiler(true);

		TimerFunction(LoopDelay, TEXT("TeleportNextPosition"));
	}
}

void APerformanceAnalytics::StartFlyThrough()
{
	IsRunning = true;

	GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, *FString::Printf(TEXT("Fly Through not implemented")));

	IsRunning = false;
}


void APerformanceAnalytics::TimerFunction(float rate, FName functionToCall) {
	FTimerDelegate Delegate;
	//Set function to call on loop
	Delegate.BindUFunction(this, functionToCall);
	//Loop every (rate) until stopped
	GetWorldTimerManager().SetTimer(timeHandle, Delegate, rate, true);
}

void APerformanceAnalytics::TeleportNextPosition()
{
	if (CoordsIndex < Coordinates.Num()) {

		FVector NextPosition = Coordinates[CoordsIndex];

		PlayerPawn->TeleportTo(NextPosition, PlayerPawn->GetActorRotation(), true, true);

		CoordsIndex++;
	}
	else {
		//Stop Timer
		GetWorldTimerManager().ClearTimer(timeHandle);
		ToggleProfiler(false);
	}
}
