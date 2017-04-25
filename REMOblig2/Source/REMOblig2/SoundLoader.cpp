// REM_Prototype Copyright (C) 2017 (Lars Magnus Nyland & Une Johnsen)

#include "REMOblig2.h"
#include "SoundLoader.h"

SoundLoader::SoundLoader()
{
	TArray<FString> SoundRef;

	SoundRef.Add("SoundWave'/Game/Audio/CreepySound.CreepySound'");
	SoundRef.Add("SoundWave'/Game/Audio/Idunno.Idunno'");
	SoundRef.Add("SoundWave'/Game/Audio/Mysterious.Mysterious'");
	SoundRef.Add("SoundWave'/Game/Audio/ObjectHit1.ObjectHit1'");
	SoundRef.Add("SoundWave'/Game/Audio/ObjectHit2.ObjectHit2'");
	SoundRef.Add("SoundWave'/Game/Audio/ObjectHit3.ObjectHit3'");
	SoundRef.Add("SoundWave'/Game/Audio/ObjectHit4.ObjectHit4'");
	SoundRef.Add("SoundWave'/Game/Audio/ObjectHit5.ObjectHit5'");

	for (FString ref : SoundRef)
	{
		ConstructorHelpers::FObjectFinder<USoundWave> SoundLoader(*ref);

		if (SoundLoader.Succeeded())
		{
			Sounds.Add(SoundLoader.Object);
		}
	}

	
}

SoundLoader::~SoundLoader()
{
}
