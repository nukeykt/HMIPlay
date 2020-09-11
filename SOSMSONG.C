#include "SOS.H"

VOID  sosMIDISong_Start(VOID)
{

}


WORD  sosMIDIInitSong(_SOS_MIDI_INIT_SONG far *a1,
	_SOS_MIDI_TRACK_DEVICE far *a2,
	WORD far *a3)
{
	WORD v4, v8, vc, v10, v14, v18, v1c, v24, v28, v2c;
	BYTE far* v40;
	BYTE far* v3c;
	BYTE far* v34;
	BYTE _huge *v48;

	v8 = 0;
	v2c = 0;
	for (v24 = 0; _szMIDIMIDIFileID[v24]; v24++)
	{
		if (_szMIDIMIDIFileID[v24] != a1->lpSongData[v24])
			return 14;
	}
	for (v14 = 0; v14 < 8; v14++)
	{
		if (!_sSOSMIDISongHeader[v14])
		{
			v18 = v14;
			break;
		}
	}
	if (v14 == 8)
		return 11;
	for (v4 = 0; v4 < 32; v4++)
	{
		_lpSOSMIDITrack[v14][v4] = NULL;
	}

	_sSOSMIDISongHeader[v18] = (_NDMF_FILE_HEADER far *)sosMIDINormalizePtr(a1->lpSongData);
	for (v4 = 0; v4 < 127; v4++)
	{
		_lpSOSMIDITriggerCallback[v18][v4] = NULL;
	}
	_lpSOSMIDIBranchCallback[v18] = NULL;
	_lpSOSMIDILoopCallback[v18] = NULL;
	v40 = (BYTE far *)sosMIDINormalizePtr(a1->lpSongData + _sSOSMIDISongHeader[v18]->dwBranchOffset);
	v34 = (BYTE far *)_sSOSMIDISongHeader[v18] + sizeof(_NDMF_FILE_HEADER);
	_sSOSMIDITrackMap[v18] = a2;
	_wSOSMIDIActiveTracks[v18] = _sSOSMIDISongHeader[v18]->wNDMFTracks;
	_wSOSMIDITotalTracks[v18] = _wSOSMIDIActiveTracks[v18];
	_sSOSMIDISongHeader[v18]->lpSongCallback = a1->lpSongCallback;
	for (v4 = 0; v4 < _wSOSMIDIActiveTracks[v18]; v4++)
	{
		_dwSOSMIDITrackDeltaTime[v18][v4] = 0;
		_lpSOSMIDITrackHeaders[v18][v4] = (_NDMF_TRACK_HEADER _huge *)_sosMIDINormalizePtr(v34 + v8);
		_lpSOSMIDITrack[v18][v4] = (_NDMF_MIDI_EVENT _huge *)_sosMIDINormalizePtr(v34 + v8 + sizeof(_NDMF_TRACK_HEADER));
		v10 = sosMIDIGetDeltaTime(_lpSOSMIDITrack[v18][v4], &__dwSOSMIDITrackDeltaCurrent[v18][v4]);
		v48 = (BYTE _huge *)_lpSOSMIDITrack[v18][v4];
		v48 += v10;
		_lpSOSMIDITrack[v18][v4] = (_NDMF_MIDI_EVENT _huge *)v48;
		v8 += ((_NDMF_TRACK_HEADER _huge*)sosMIDINormalizePtr(v34 + v8))->wTrackLength;
	}
	v3c = (BYTE _huge *)v40;
	v40 += _wSOSMIDIActiveTracks[v18];
	for (v4 = 0; v4 < _wSOSMIDIActiveTracks[v18]; v4++)
	{
		if (*v3c)
		{
			_lpMIDIBranchData[v18][v4] = (_SOS_BRANCH_POINT * far)v40;
		}
		for (v28 = 0; *v3c > v28; v28++)
		{
			((_SOS_BRANCH_POINT * far)v40)[v28].lpCtrlChangeData = (WORD)_sosMIDINormalizePtr(a1->lpSongData + ((_SOS_BRANCH_POINT * far)v40)[v28].lpCtrlChangeDataTemp)
		}
		v40 += *v3c * sizeof(_SOS_BRANCH_POINT);
		v2c = *v3c;
		v3c++;
	}
	for (v4 = 0; v4 < _wSOSMIDITotalTracks[v18]; v4++)
	{
		if (_sSOSMIDITrackMap[v18]->wTrackDevice[v4] == 0xff)
		{
			vc = 0;
			v1c = 0;
			while (_sSOSMIDISongHeader[v18]->sTrackMap.wTrackDevice[v4][v1c] && !vc && v1c < 5)
			{
				for (v14 = 0; v14 < 5; v14++)
				{
					if (_sSOSMIDISongHeader[v18]->sTrackMap.wTrackDevice[v4][v1c] == _MIDI_SOUND_MASTER_II)
					{
						if (_wSOSMIDIDriverID[v14] == _MIDI_SOUND_MASTER_II || _wSOSMIDIDriverID[v14] == _MIDI_MPU_401 || _wSOSMIDIDriverID[v14] == _MIDI_AWE32)
						{
							_sSOSMIDITrackMap[v18]->wTrackDevice[v4] = v14;
							vc = 1;
							break;
						}
					}
					else if (_sSOSMIDISongHeader[v18]->sTrackMap.wTrackDevice[v4][v1c] == _MIDI_OPL2)
					{
						if (_wSOSMIDIDriverID[v14] == _MIDI_OPL2 || _wSOSMIDIDriverID[v14] == _MIDI_OPL3)
						{
							_sSOSMIDITrackMap[v18]->wTrackDevice[v4] = v14;
							vc = 1;
							break;
						}
					}
					else if (_wSOSMIDIDriverID[v14] == _sSOSMIDISongHeader[v18]->sTrackMap.wTrackDevice[v4][v1c])
					{
						_sSOSMIDITrackMap[v18]->wTrackDevice[v4] = v14;
						vc = 1;
						break;
					}
				}
				v1c++;
			}
			if (!_sSOSMIDISongHeader[v18]->sTrackMap.wTrackDevice[v4][0])
			{
				_sSOSMIDITrackMap[v18]->wTrackDevice[v4] = 0;
			}
			else if (!vc)
			{
				_lpSOSMIDITrack[v18][v4] = NULL;
				_sSOSMIDITrackMap[v18]->wTrackDevice[v4] = 0xff;
				_wSOSMIDIActiveTracks[v18]--;
			}
		}
	}
	for (v14 = 0; v14 < 128; v14++)
	{
		_sSOSMIDISongHeader[v18]->bCtrlRestore[v14] = 1;
	}
	*a3 = v18;
	return 0;
}

WORD  sosMIDIUnInitSong(WORD a1)
{
	if (a1 < 8)
		_sSOSMIDISongHeader[a1] = NULL;
	else
		return 10;
	return 0;
}

WORD  sosMIDIStartSong(WORD a1)
{
	WORD v4;

	v4 = sosTIMERRegisterEvent(_sSOSMIDISongHeader[a1]->wTempo, sosMIDISongHandler, &_wSOSMIDIEventHandle[a1]);
	if (v4)
		return v4;
	_bSOSMIDITimerSongHandler[_wSOSMIDIEventHandle[a1]] = a1;
	_wSOSMIDISongActive[a1] = 1;
	return 0;
}

WORD  sosMIDIStopSong(WORD a1)
{
	_SOS_MIDI_INIT_SONG far v14;
	if (a1 < 8)
	{
		if (_wSOSMIDIEventHandle[a1] != 0xffffffff)
			sosTIMERRemoveEvent(_wSOSMIDIEventHandle[a1]);
		_bSOSMIDITimerSongHandler[_wSOSMIDIEventHandle[a1]] = 0xff;
		_wSOSMIDIEventHandle[a1] = 0xffffffff;
		if (_wSOSMIDISongActive[a1])
		{
			v14.lpSongData = (BYTE far *)_sSOSMIDISongHeader[a1];
			v14.lpSongCallback = _sSOSMIDISongHeader[a1]->lpSongCallback;
			sosMIDIResetChannelStealing(a1);
			_wSOSMIDISongActive[a1] = 0;
			_sSOSMIDISongHeader[a1] = NULL;
			sosMIDIResetSong(a1, &v14);
		}
	}
	else
		return 10;
	return 0;
}

WORD  sosMIDIResetSong(WORD a1, _SOS_MIDI_INIT_SONG far *a2)
{
	WORD v8, v4, v10;
	BYTE far *v1c, *v2c;
	v8 = 0;
	_sSOSMIDISongHeader[a1] = sosMIDINormalizePtr(a2->lpSongData);
	v1c = (BYTE far *)_sSOSMIDISongHeader[v18] + sizeof(_NDMF_FILE_HEADER);
	_wSOSMIDIActiveTracks[a1] = _sSOSMIDISongHeader[v18]->wNDMFTracks;
	_wSOSMIDITotalTracks[a1] = _wSOSMIDIActiveTracks[a1];
	_sSOSMIDISongHeader[a1]->lpSongCallback = a2->lpSongCallback;
	for (v4 = 0; v4 < _wSOSMIDIActiveTracks[a1]; v4++)
	{
		_dwSOSMIDITrackDeltaTime[a1][v4] = 0;
		_lpSOSMIDITrackHeaders[a1][v4] = (_NDMF_TRACK_HEADER _huge *)_sosMIDINormalizePtr(v1c + v8);
		_lpSOSMIDITrack[a1][v4] = (_NDMF_MIDI_EVENT _huge *)_sosMIDINormalizePtr(v1c + v8 + sizeof(_NDMF_TRACK_HEADER));
		v10 = sosMIDIGetDeltaTime(_lpSOSMIDITrack[a1][v4], &__dwSOSMIDITrackDeltaCurrent[a1][v4]);
		v2c = (BYTE _huge *)_lpSOSMIDITrack[a1][v4];
		v2c += v10;
		_lpSOSMIDITrack[a1][v4] = (_NDMF_MIDI_EVENT _huge *)v2c;
		v8 += ((_NDMF_TRACK_HEADER _huge*)sosMIDINormalizePtr(v1c + v8))->wTrackLength;
	}
	for (v4 = 0; v4 < _wSOSMIDITotalTracks[a1]; v4++)
	{
		if (_sSOSMIDITrackMap[a1]->wTrackDevice[v4] == 0xff)
		{
			_lpSOSMIDITrack[a1][v4] = NULL;
			_wSOSMIDIActiveTracks[a1]--;
		}
	}
	return 0;
}

WORD  sosMIDIGetDeltaTime(LPSTR a1, DWORD far *a2)
{
	BYTE* v4;
	WORD v8, vc, v10, v14;

	v8 = 0;
	vc = 0;
	v10 = 0;
	v14 = 0;
	do
	{
		v14++;
		v4 = *a1++;
		if (v4 & 0x80)
			v8 = 1;
		v4 &= 0x7f;
		v10 |= v4 << vc;
		vc += 7;
	} while (!v8);
	*a2 = v10;
	return v14;
}

WORD sosMIDISetMasterVolume(BYTE a1)
{
	WORD v8;
	v8 = 0;
	_bSOSMIDIMasterVolume = a1;
	for (v8 = 0; v8 < 8; v8++)
	{
		if (_wSOSMIDISongActive[v8])
			sosMIDISetSongVolume(v8, _wSOSMIDISongVolume[v8]);
	}
	return 0;
}

WORD sosMIDIRegisterLoopFunction(WORD a1, WORD(far * a2)(WORD, BYTE, BYTE, BYTE))
{
	_lpSOSMIDILoopCallback[a1] = a2;
	return 0;
}

WORD sosMIDIRegisterBranchFunction(WORD a1, WORD(far * a2)(WORD, BYTE, BYTE))
{
	_lpSOSMIDIBranchCallback[a1] = a2;
	return 0;
}

WORD sosMIDIRegisterTriggerFunction(WORD a1, BYTE a2, WORD(far* a3)(WORD, BYTE, BYTE))
{
	_lpSOSMIDITriggerCallback[a1][a2] = a3;
	return 0;
}

WORD sosMIDIBranchToTrackLocation(WORD a1, BYTE a2, BYTE a3)
{
	WORD v10, v14;
	BYTE v4;
	v10 = 0;
	while (_lpMIDIBranchData[a1][a2][v10].bBranchID != a3)
		v10++;
	v14 = _lpMIDIBranchData[a1][a2][v10].dwOffset + sizeof(_NDMF_TRACK_HEADER);
	_lpSOSMIDITrack[a1][a2] = (_NDMF_MIDI_EVENT far *)((BYTE far *)_lpSOSMIDITrackHeaders[a1][a2] + v14);
	_bSOSMIDIData[0] = 0xb0 | _lpSOSMIDITrackHeaders[a1][a2]->wChannel;
	for (v4 = 0; v4 < _lpMIDIBranchData[a1][a2][v10].bCtrlChangeCount; v4 += 2)
	{
		_bSOSMIDIData[1] = ((BYTE*)_lpMIDIBranchData[a1][a2][v10].lpCtrlChangeData + v4)[0];
		_bSOSMIDIData[2] = ((BYTE*)_lpMIDIBranchData[a1][a2][v10].lpCtrlChangeData + v4)[1];
        sosMIDIHandleMIDIData(a1, _bSOSMIDIData, _sSOSMIDITrackMap[a1]->wTrackDevice[a2], 3);
	}
	_bSOSMIDIBranchOccurred = 0;
	return 0;
}

WORD sosMIDIBranchToSongLocation(WORD a1, BYTE a2)
{
	BYTE v4;
	WORD v10, v14, v1c;

	for (v4 = 1; v4 < _wSOSMIDITotalTracks[a1]; v4++)
	{
		if (_lpSOSMIDITrack[a1][v4])
		{
			v10 = 0;
			while (_lpMIDIBranchData[a1][v4][v10].bBranchID != a2)
				v10++;
			v14 = _lpMIDIBranchData[a1][v4][v10].dwOffset + sizeof(_NDMF_TRACK_HEADER);
			_lpSOSMIDITrack[a1][v4] = (_NDMF_MIDI_EVENT far *)((BYTE far *)_lpSOSMIDITrackHeaders[a1][v4] + v14);
			v1c = sosMIDIGetDeltaTime(_lpSOSMIDITrack[a1][v4], &_dwSOSMIDITrackDeltaCurrent[a1][v4]);
            _lpSOSMIDITrack[a1][v4] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrack[a1][v4] + v1c);
			_dwSOSMIDITrackDeltaTime[a1][v4] = 0;
			sosMIDISetTrackState(a1, v4, v10);
		}
	}
	_bSOSMIDIBranchOccurred = 0;
	return 0;
}

VOID  sosMIDISong_End(VOID)
{

}

