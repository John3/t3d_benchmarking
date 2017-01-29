//-----------------------------------------------------------------------------
// Copyright (C) LogicKing.com, Inc.
//-----------------------------------------------------------------------------

moveMap.bind(keyboard, "e", useObject);

function reloadMission(%missionName)
{
	if(%missionName $= "") 
	{
		if($Server::MissionFile !$= "")
			schedule( 0, 0, loadMission, $Server::MissionFile);
		else
			error("Wrong mission name");

		return;
	}
	else
	{
		%missionName = expandFileName("~/data/missions/" @ %missionName);
		%missionName = makeRelativePath(%missionName, getWorkingDirectory());
		schedule( 0, 0, loadMission, %missionName);
	}
}


// Used to call "use" method
function useObject(%flg)
{    
    commandToServer('useObj', %flg);
}

addMessageCallback( 'MsgDoorLocked', handleDoorLocked );

function handleDoorLocked(%msgType, %msgString)
{
   echo(detag(%msgString));
}