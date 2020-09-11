#include "SOS.H"

VOID  sosMIDITmr_Start(VOID)
{

}

VOID far sosMIDISongHandler(VOID)
{
    WORD v18, v4, v10, v20, v24, v2c, v14;
    VOID ( far * v3c )( WORD );
    VOID ( far * v44 )( WORD );
    BYTE _huge * v34;

    _SOS_MIDI_INIT_SONG far v4c;

    v18 = 1;

    if (!_wSOSMIDISongActive[_bSOSMIDISongHandle] || _wSOSMIDISongPaused[_bSOSMIDISongHandle])
        return;

    if (_wSOSMIDIVolumeFadeTicks[_bSOSMIDISongHandle])
    {
        if (--_bSOSMIDIHandleVolumeFade[_bSOSMIDISongHandle] == 0)
        {
            _bSOSMIDIHandleVolumeFade[_bSOSMIDISongHandle] = 3;
            _wSOSMIDIVolumeFadeTicks[_bSOSMIDISongHandle]--;
            switch (_wSOSMIDIVolumeFadeDirection[_bSOSMIDISongHandle])
            {
            case _SOS_MIDI_FADE_OUT:
            case _SOS_MIDI_FADE_OUT_STOP:
                _dwSOSMIDIVolumeFadeVolume[_bSOSMIDISongHandle] -= _dwSOSMIDIVolumeFadeFraction[_bSOSMIDISongHandle];
                sosMIDISetSongVolume(_bSOSMIDISongHandle, _dwSOSMIDIVolumeFadeVolume[_bSOSMIDISongHandle] >> 16);
                if ((_wSOSMIDIVolumeFadeDirection[_bSOSMIDISongHandle] & _SOS_MIDI_FADE_OUT_STOP) && !_wSOSMIDIVolumeFadeTicks[_bSOSMIDISongHandle])
                {
                    _wSOSMIDISongActive[_bSOSMIDISongHandle] = 0;
                    sosMIDIResetChannelStealing(_bSOSMIDISongHandle);
                    if (_wSOSMIDIEventHandle[_bSOSMIDISongHandle] != 0xffffffff)
                        sosTIMERRemoveEvent(_wSOSMIDIEventHandle[_bSOSMIDISongHandle]);
                    _bSOSMIDITimerSongHandler[_wSOSMIDIEventHandle[_bSOSMIDISongHandle]] = 0xff;
                    _wSOSMIDIEventHandle[_bSOSMIDISongHandle] = 0xffffffff;
                    v3c = _sSOSMIDISongHeader[_bSOSMIDISongHandle]->lpSongCallback;
                    v4c.lpSongData = _sSOSMIDISongHeader[_bSOSMIDISongHandle];
                    v4c.lpSongCallback = v3c;
                    _sSOSMIDISongHeader[_bSOSMIDISongHandle] = NULL;
                    sosMIDIResetSong(_bSOSMIDISongHandle, &v4c);
                    if (v3c)
                        v3c(_bSOSMIDISongHandle);
                    return;
                }
                break;
            case _SOS_MIDI_FADE_IN:
                _dwSOSMIDIVolumeFadeVolume[_bSOSMIDISongHandle] += _dwSOSMIDIVolumeFadeFraction[_bSOSMIDISongHandle];
                sosMIDISetSongVolume(_bSOSMIDISongHandle, _dwSOSMIDIVolumeFadeVolume[_bSOSMIDISongHandle] >> 16);
                break;
            }
        }
    }
    for (v4 = 0; v4 < _wSOSMIDITotalTracks[_bSOSMIDISongHandle]; v4++)
    {
        _dwSOSMIDITrackDeltaTime[_bSOSMIDISongHandle][v4]++;
        if (_lpSOSMIDITrack[_bSOSMIDISongHandle][v4])
        {
            if (_dwSOSMIDITrackDeltaCurrent[_bSOSMIDISongHandle][v4] <= _dwSOSMIDITrackDeltaTime[_bSOSMIDISongHandle][v4])
            {
                do
                {
                    _bSOSMIDIBranchOccurred = 0;
                    v18 = 1;
                    if (_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMidiData[0] < 0xf0)
                    {
                        v10 = _bSOSMIDIEventSize[_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMidiData[0] >> 4];
                    }
                    else
                    {
                        v10 = _bSOSMIDIEventSizeControl[_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMidiData[0] & 0x0f];
                    }
                    if (_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMidiData[0] == 0xff)
                    {
                        switch (_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMidiData[1])
                        {
                        case 0x2f:
                            _lpSOSMIDITrack[_bSOSMIDISongHandle][v4] = NULL;
                            if (_wSOSMIDIActiveTracks[_bSOSMIDISongHandle] - 1 == 1 && _lpSOSMIDITrack[_bSOSMIDISongHandle][0] != NULL)
                            {
                                _wSOSMIDIActiveTracks[_bSOSMIDISongHandle]--;
                                _lpSOSMIDITrack[_bSOSMIDISongHandle][0] = NULL;
                            }
                            _wSOSMIDIActiveTracks[_bSOSMIDISongHandle]--;
                            if (_wSOSMIDIActiveTracks[_bSOSMIDISongHandle] == 0)
                            {
                                _wSOSMIDISongActive[_bSOSMIDISongHandle] = 0;
                                sosMIDIResetChannelStealing(_bSOSMIDISongHandle);
                                if (_wSOSMIDIEventHandle[_bSOSMIDISongHandle] != 0xffffffff)
                                    sosTIMERRemoveEvent(_wSOSMIDIEventHandle[_bSOSMIDISongHandle]);
                                _bSOSMIDITimerSongHandler[_wSOSMIDIEventHandle[_bSOSMIDISongHandle]] = 0xff;
                                _wSOSMIDIEventHandle[_bSOSMIDISongHandle] = 0xffffffff;
                                v3c = _sSOSMIDISongHeader[_bSOSMIDISongHandle]->lpSongCallback;
                                v4c.lpSongData = _sSOSMIDISongHeader[_bSOSMIDISongHandle];
                                v4c.lpSongCallback = v3c;
                                _sSOSMIDISongHeader[_bSOSMIDISongHandle] = NULL;
                                sosMIDIResetSong(_bSOSMIDISongHandle, &v4c);
                                if (v3c)
                                    v3c(_bSOSMIDISongHandle);
                                return;

                            }
                            v10 = 3;
                            break;
                        case 0x5f:
                            v10 = 5;
                            break;
                        }
                    }
                    else
                    {
                        if ((_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[0] & 0xf0) == 0xb0)
                        {
                            switch (_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[1])
                            {
                            case 103:
                                _sSOSMIDISongHeader[_bSOSMIDISongHandle]->bCtrlRestore[_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2]] = 0;
                                break;
                            case 104:
                                _sSOSMIDISongHeader[_bSOSMIDISongHandle]->bCtrlRestore[_lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2]] = 1;
                                break;
                            case 108:
                                break;
                            case 110:
                                break;
                            case 109:
                                v24 = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2];
                                v20 = 0;
                                while (_lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bBranchID != v24)
                                    v20++;
                                _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bLoopCount = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[6];
                                break;
                            case 111:
                            case 112:
                                v24 = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2];
                                v20 = 0;
                                while (_lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bBranchID != v24)
                                    v20++;
                                v2c = _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bLoopCount;
                                if (v2c != 0xff && v2c != 0)
                                {
                                    _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bLoopCount--;
                                    v2c--;
                                }
                                if (_lpSOSMIDILoopCallback[_bSOSMIDISongHandle])
                                {
                                    _bSOSMIDIBranchOccurred = 1;
                                    if (!_lpSOSMIDILoopCallBack[_bSOSMIDISongHandle](_bSOSMIDISongHandle, v4, v24, v2c))
                                        v2c = 0;
                                    if (!_bSOSMIDIBranchOccurred)
                                    {
                                        v18 = 0;
                                        v10 = 0;
                                    }
                                    else
                                        _bSOSMIDIBranchOccurred = 0;
                                }
                                if (v2c)
                                {
                                    for (v14 = 1; v14 < _wSOSMIDITotalTracks[_bSOSMIDISongHandle]; v14++)
                                    {
                                        if (_lpSOSMIDITrack[_bSOSMIDISongHandle])
                                        {
                                            v20 = 0;
                                            while (_lpMIDIBranchData[_bSOSMIDISongHandle][v14][v20].bBranchID != v24)
                                                v20++;
                                            v1c = _lpMIDIBranchData[_bSOSMIDISongHandle][v14][v20].dwOffset + sizeof(_NDMF_TRACK_HEADER);
                                            _lpSOSMIDITrack[_bSOSMIDISongHandle][v14] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrackHeaders[_bSOSMIDISongHandle][v14] + v1c);
                                            v10 = sosMIDIGetDeltaTime(_lpSOSMIDITrack[_bSOSMIDISongHandle][v14], &_dwSOSMIDITrackDeltaCurrent[_bSOSMIDISongHandle][v14]);
                                            _lpSOSMIDITrack[_bSOSMIDISongHandle][v14] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrack[_bSOSMIDISongHandle][v14] + v10);
                                            _dwSOSMIDITrackDeltaTime[_bSOSMIDISongHandle][v14] = 0;
                                            v18 = 0;
                                            sosMIDISetTrackState(_bSOSMIDISongHandle, v14, v20);
                                        }
                                    }
                                    v10 = 0;
                                }
                                break;
                            case 113:
                                break;
                            case 114:
                                v24 = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2] | 0x80;
                                v20 = 0;
                                while (_lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bBranchID != v24)
                                    v20++;
                                v2c = 1;
                                if (_lpSOSMIDIBranchCallback[_bSOSMIDISongHandle])
                                {
                                    _bSOSMIDIBranchOccurred = 1;
                                    if (!_lpSOSMIDIBranchCallback[_bSOSMIDISongHandle](_bSOSMIDISongHandle, v4, v24))
                                        v2c = 0;
                                    if (!_bSOSMIDIBranchOccurred)
                                    {
                                        v18 = 0;
                                        v10 = 0;
                                    }
                                    else
                                        _bSOSMIDIBranchOccurred = 0;
                                }
                                if (v2c)
                                {
                                    for (v14 = 1; v14 < _wSOSMIDITotalTracks[_bSOSMIDISongHandle]; v14++)
                                    {
                                        if (_lpSOSMIDITrack[_bSOSMIDISongHandle])
                                        {
                                            v20 = 0;
                                            while (_lpMIDIBranchData[_bSOSMIDISongHandle][v14][v20].bBranchID != v24)
                                                v20++;
                                            v1c = _lpMIDIBranchData[_bSOSMIDISongHandle][v14][v20].dwOffset + sizeof(_NDMF_TRACK_HEADER);
                                            _lpSOSMIDITrack[_bSOSMIDISongHandle][v14] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrackHeaders[_bSOSMIDISongHandle][v14] + v1c);
                                            v10 = sosMIDIGetDeltaTime(_lpSOSMIDITrack[_bSOSMIDISongHandle][v14], &_dwSOSMIDITrackDeltaCurrent[_bSOSMIDISongHandle][v14]);
                                            _lpSOSMIDITrack[_bSOSMIDISongHandle][v14] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrack[_bSOSMIDISongHandle][v14] + v10);
                                            _dwSOSMIDITrackDeltaTime[_bSOSMIDISongHandle][v14] = 0;
                                            v18 = 0;
                                            sosMIDISetTrackState(_bSOSMIDISongHandle, v14, v20);
                                        }
                                    }
                                    v10 = 0;
                                }
                                break;
                            case 116:
                                break;
                            case 115:
                                v24 = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2];
                                v20 = 0;
                                while (_lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bBranchID != v24)
                                    v20++;
                                _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bLoopCount = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[6];
                                break;
                            case 117:
                            case 118:
                                v24 = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2];
                                v20 = 0;
                                while (_lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bBranchID != v24)
                                    v20++;
                                v2c = _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bLoopCount;
                                if (v2c != 0xff && v2c != 0)
                                {
                                    _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bLoopCount--;
                                    v2c--;
                                }
                                if (_lpSOSMIDILoopCallback[_bSOSMIDISongHandle])
                                {
                                    _bSOSMIDIBranchOccurred = 1;
                                    if (!_lpSOSMIDILoopCallBack[_bSOSMIDISongHandle](_bSOSMIDISongHandle, v4, v24, v2c))
                                        v2c = 0;
                                    if (!_bSOSMIDIBranchOccurred)
                                        v10 = 0;
                                    else
                                        _bSOSMIDIBranchOccurred = 0;
                                }
                                if (v2c)
                                {
                                    v1c = _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].dwOffset + sizeof(_NDMF_TRACK_HEADER);
                                    _lpSOSMIDITrack[_bSOSMIDISongHandle][v4] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrackHeaders[_bSOSMIDISongHandle][v4] + v1c);
                                    sosMIDISetTrackState(_bSOSMIDISongHandle, v4, v20);
                                    v10 = 0;
                                }
                                break;
                            case 119:
                                v2c = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2];
                                if (_lpSOSMIDITriggerCallback[_bSOSMIDISongHandle][v2c])
                                {
                                    _bSOSMIDIBranchOccurred = 1;
                                    _lpSOSMIDITriggerCallback[_bSOSMIDISongHandle][v2c](_bSOSMIDISongHandle, v4, v2c);
                                    if (!_bSOSMIDIBranchOccurred)
                                    {
                                        v18 = 0;
                                        v10 = 0;
                                    }
                                    else
                                        _bSOSMIDIBranchOccurred = 0;
                                }
                                break;
                            case 120:
                                break;
                            case 121:
                                v24 = _lpSOSMIDITrack[_bSOSMIDISongHandle][v4]->bMIDIData[2];
                                v20 = 0;
                                while (_lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].bBranchID != v24)
                                    v20++;
                                v2c = 1;
                                if (_lpSOSMIDIBranchCallback[_bSOSMIDISongHandle])
                                {
                                    _bSOSMIDIBranchOccurred = 1;
                                    if (!_lpSOSMIDIBranchCallback[_bSOSMIDISongHandle](_bSOSMIDISongHandle, v4, v24))
                                        v2c = 0;
                                    if (!_bSOSMIDIBranchOccurred)
                                        v10 = 0;
                                    else
                                        _bSOSMIDIBranchOccurred = 0;
                                }
                                if (v2c)
                                {
                                    v1c = _lpMIDIBranchData[_bSOSMIDISongHandle][v4][v20].dwOffset + sizeof(_NDMF_TRACK_HEADER);
                                    _lpSOSMIDITrack[_bSOSMIDISongHandle][v4] = (_NDMF_MIDI_EVENT _huge*)((BYTE _huge *)_lpSOSMIDITrackHeaders[_bSOSMIDISongHandle][v4] + v1c);
                                    sosMIDISetTrackState(_bSOSMIDISongHandle, v4, v20);
                                    v10 = 0;
                                }
                                break;
                            default:
                                goto loc_113D8;
                            }
                        }
                        else
                        {
loc_113D8:
                            if (v4)
                            {
                                sosMIDIHandleMIDIData(_bSOSMIDISongHandle, _lpSOSMIDITrack[_bSOSMIDISongHandle][v4], _sSOSMIDITrackMap[_bSOSMIDISongHandle]->wTrackDevice[v4], v10);
                            }
                        }
                    }
                    if (!_bSOSMIDIBranchOccurred)
                        _dwSOSMIDITrackDeltaTime[_bSOSMIDISongHandle][v4] = 0;
                    if (!_lpSOSMIDITrack[_bSOSMIDISongHandle][v4])
                        break;
                    v34 = (BYTE _huge *)_lpSOSMIDITrack[_bSOSMIDISongHandle];
                    v34 += v10;
                    _lpSOSMIDITrack[_bSOSMIDISongHandle] = (_NDMF_MIDI_EVENT _huge *)v34;
                    if (v18)
                    {
                        v10 = sosMIDIGetDeltaTime(_lpSOSMIDITrack[_bSOSMIDISongHandle][v4], &_dwSOSMIDITrackDeltaCurrent[_bSOSMIDISongHandle][v4]);
                        v34 = (BYTE _huge *)_lpSOSMIDITrack[_bSOSMIDISongHandle];
                        v34 += v10;
                        _lpSOSMIDITrack[_bSOSMIDISongHandle] = (_NDMF_MIDI_EVENT _huge *)v34;
                    }
                } while (!_dwSOSMIDITrackDeltaCurrent[_bSOSMIDISongHandle][v4]);
            }
        }
    }
}

VOID  sosMIDISetTrackState(WORD a1, WORD a2, WORD a3)
{
    BYTE v1c[2];
    WORD v4;
    if (_sSOSMIDISongHeader[a1]->bCtrlRestore[108])
    {
        v1c[0] = 0xc0 | (_lpSOSMIDITrack[a1][a2]->bMIDIData[0] & 0xf);
        v1c[1] = _lpMIDIBranchData[a1][a2][a3].bInstrument;
        sosMIDIHandleMIDIData(a1, v1c, _sSOSMIDITrackMap[a1]->wTrackDevice[a2], 2);
    }
    v1c[0] = 0xb0 | (_lpSOSMIDITrack[a1][a2]->bMIDIData[0] & 0xf);
    for (v4 = 0; _lpMIDIBranchData[a1][a2][a3].bCtrlChangeCount > v4; v4 += 2)
    {
        v1c[1] = ((BYTE*)_lpMIDIBranchData[a1][a2][a3].lpCtrlChangeData + v4)[0];
        v1c[2] = ((BYTE*)_lpMIDIBranchData[a1][a2][a3].lpCtrlChangeData + v4)[1];
        if (_sSOSMIDISongHeader[a1]->bCtrlRestore[v1c[1]])
        {
            sosMIDIHandleMIDIData(a1, v1c, _sSOSMIDITrackMap[a1]->wTrackDevice[a2], 3);
        }
    }
}

VOID  sosMIDITmr_End(VOID)
{
}
