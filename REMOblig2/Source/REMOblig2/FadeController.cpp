// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "FadeController.h"
#include "REM_GameMode.h"


// Sets default values for this component's properties
UFadeController::UFadeController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<ULevelSequence> FadeInLoader(TEXT("LevelSequence'/Game/FadeIn.FadeIn'"));
	ConstructorHelpers::FObjectFinder<ULevelSequence> FadeOutLoader(TEXT("LevelSequence'/Game/FadeOut.FadeOut'"));

	if (FadeInLoader.Succeeded())
	{
		FadeInSequence = FadeInLoader.Object;
	}

	if (FadeOutLoader.Succeeded())
	{
		FadeOutSequence = FadeOutLoader.Object;
	}
}


// Called when the game starts
void UFadeController::BeginPlay()
{
	Super::BeginPlay();
	
	AREM_GameMode* GameMode = Cast<AREM_GameMode>(GetWorld()->GetAuthGameMode());

	GameMode->FadeController = Cast<USceneComponent>(this);

	Parent = Cast<ALevelSequenceActor>(GetOwner());

	HudInstance = Cast<AREM_Hud>(GetWorld()->GetFirstPlayerController()->GetHUD());
}


// Called every frame
void UFadeController::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	float CurrentPosition = 0.f;

	if (!Parent)
		return;

	if (!Parent->SequencePlayer)
		return;

	if (!Parent->GetSequence())
		return;

	if (LoadLevelAfterFadeDone)
	{
		if (!Parent->SequencePlayer->IsPlaying())
		{
			UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
		}
	}

	if (Parent->SequencePlayer->IsPlaying())
	{
		CurrentPosition = Parent->SequencePlayer->GetPlaybackPosition();
		ULevelSequence* Sequence = Parent->GetSequence();
		UMovieScene* Movie = Parent->GetSequence()->GetMovieScene();

		UMovieSceneFadeTrack* track = Movie->FindMasterTrack<UMovieSceneFadeTrack>();

		if (track != nullptr)
		{
			float InOut = 0.f;
			if (track->Eval(CurrentPosition, LastPosition, InOut))
			{
				if (HudInstance)
				{
					HudInstance->CurrentOpacity = InOut;
				}
			}
		}
	}

	LastPosition = CurrentPosition;
}

void UFadeController::FadeIn()
{
	if (Parent->GetSequence())
	{
		if (Parent->SequencePlayer->IsPlaying())
			StopAnimation();
	}

	if (FadeInSequence)
	{
		Parent->SetSequence(FadeInSequence);

		StartAnimation();
	}
}

void UFadeController::FadeOut(bool LoadAfterDone, FName LevelName)
{
	if (Parent->GetSequence())
	{
		if (Parent->SequencePlayer->IsPlaying())
			StopAnimation();
	}

	if (FadeOutSequence)
	{
		Parent->SetSequence(FadeOutSequence);

		StartAnimation();
	}

	LoadLevelAfterFadeDone = LoadAfterDone;
	LevelToLoad = LevelName;
}

void UFadeController::StartAnimation()
{
	Parent->SequencePlayer->Play();
}

void UFadeController::StopAnimation()
{
	Parent->SequencePlayer->Stop();
}

