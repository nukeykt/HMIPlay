#include "SOS.H"


VOID  sosMIDISt_Start(VOID)
{
}

BOOL  sosMIDIHandleMIDIData(WORD a1, LPSTR a2, WORD a3, WORD a4)
{
    BYTE v10, v14, vc, v4, v8;

    WORD v24, v28, v18, v1c, v20;

    v10 = 0;
    v14 = 0xff;
    v24 = 0xffffffff;
    vc = a2[0];
    v4 = vc & 0xf;
    if (!_wSOSMIDIChannelStealing)
    {
        switch (a2[0] & 0xf0)
        {
        case 0xb0:
            switch (a2[1])
            {
            case 7:
                _bSOSMIDIData[0] = a2[0];
                _bSOSMIDIData[1] = 7;
                v28 = (a2[2] * _wSOSMIDISongVolume[a1]) >> 7;
                _bSOSMIDIData[2] = (v28 * _bSOSMIDIMasterVolume) >> 7;
                _bSOSMIDIDeviceChannelVolume[a3][v4] = a2[2];
                if (_wSOSMIDISongMuted[a1])
                    _bSOSMIDIData[2] = 0;
                break;
            default:
                _bSOSMIDIData[0] = a2[0];
                _bSOSMIDIData[1] = a2[1];
                _bSOSMIDIData[2] = a2[2];
                _bSOSMIDIData[3] = a2[3];
                break;
            }
            _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, a4, a3);
            break;
        default:
            _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](a2, a4, a3);
            break;
        }
        return 1;
    }
    v8 = _bSOSMIDIChannelRemap[a3][a1][v4];
loc_101BE:
    if (v8 != 0xff)
    {
        a2[0] = (vc & 0xf0) | v8;
    }
    else
    {
        if (v4 == 9)
        {
            _bSOSMIDIChannelRemap[a3][a1][v4] = 9;
            v8 = 9;
            goto loc_101BE;
        }
        for (v18 = 0; v18 < 16; v18++)
        {
            while (!_bSOSMIDIDeviceChannelAvailable[a3][v18] && v18 < 16)
                v18++;
            if (v18 < 16 && _bSOSMIDIOwnerChannel[a3][v18] == 0xff)
            {
                _bSOSMIDIChannelRemap[a3][a1][v4] = v18;
                v8 = v18;
                _bSOSMIDIOwnerChannel[a3][v18] = v4;
                _bSOSMIDIOwnerSong[a3][v18] = a1;
                _bSOSMIDIChannelPriority[a3][v18] = _sSOSMIDISongHeader[a1]->wChannelPriority[v4];
                v1c = _bSOSMIDIChannelDataIndex[a3][a1][v4];
                if (v1c != 0xff)
                {
                    _bSOSMIDIDeviceChannelVolume[a3][v8] = 0x7f;
                    _bSOSMIDIData[0] = 0xb0 | v8;
                    _bSOSMIDIData[1] = 0x79;
                    _bSOSMIDIData[2] = 0;
                    v20 = 3;
                    _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                    if (_sSOSMIDIChannelData[a3][v4][v1c].bInstrument != 0xff)
                    {
                        _bSOSMIDIData[0] = 0xc0 | v8;
                        _bSOSMIDIData[1] = _sSOSMIDIChannelData[a3][v4][v1c].bInstrument;
                        v20 = 2;
                        _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                    }
                    if (_sSOSMIDIChannelData[a3][v4][v1c].bPitchBend != 0xff)
                    {
                        _bSOSMIDIData[0] = 0xe0 | v8;
                        _bSOSMIDIData[1] = 0;
                        _bSOSMIDIData[2] = _sSOSMIDIChannelData[a3][v4][v1c].bPitchBend;
                        v20 = 2; // Should be 3 ???
                        _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                    }
                    if (_sSOSMIDIChannelData[a3][v4][v1c].bChannelVolume != 0xff)
                    {
                        _bSOSMIDIData[0] = 0xb0 | v8;
                        _bSOSMIDIData[1] = 7;
                        _bSOSMIDIData[2] = _sSOSMIDIChannelData[a3][v4][v1c].bChannelVolume;
                        v20 = 3;
                        _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                    }
                    if (_sSOSMIDIChannelData[a3][v4][v1c].bSustain != 0xff)
                    {
                        _bSOSMIDIData[0] = 0xb0 | v8;
                        _bSOSMIDIData[1] = 64;
                        _bSOSMIDIData[2] = _sSOSMIDIChannelData[a3][v4][v1c].bSustain;
                        v20 = 3;
                        _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                    }
                }
                else
                {
                    for (v1c = 0; v1c < 4; v1c++)
                    {
                        if (_sSOSMIDIChannelData[a3][v4][v1c].bUsed == 0xff)
                        {
                            _sSOSMIDIChannelData[a3][v4][v1c].bUsed = 1;
                            _bSOSMIDIChannelDataIndex[a3][a1][v4] = v1c;
                            break;
                        }
                    }
                }
                goto loc_101BE;
            }
        }
        for (v18 = 0; v18 < 16; v18++)
        {
            while (!_bSOSMIDIDeviceChannelAvailable[a3][v18] && v18 < 16)
                v18++;
            if (v18 < 16)
            {
                if (_bSOSMIDIChannelPriority[a3][v18] > v10 && _bSOSMIDIChannelPriority[a3][v18] != 0xff)
                {
                    v10 = _bSOSMIDIChannelPriority[a3][v18];
                    v14 = v18;
                }
            }
        }
        if (v14 != 0xff)
        {
            if (v10 > _sSOSMIDISongHeader[a1]->wChannelPriority[v4])
            {
                _bSOSMIDIChannelRemap[a3][a1][v4] = v14;
                _bSOSMIDIChannelRemap[a3][_bSOSMIDIOwnerSong[a3][v14]][v14] = 0xff;
                _bSOSMIDIOwnerChannel[a3][v14] = v4;
                _bSOSMIDIOwnerSong[a3][v14] = a1;
                v8 = v14;
                _bSOSMIDIChannelPriority[a3][v14] = _sSOSMIDISongHeader[a1]->wChannelPriority[v4];
                _bSOSMIDIData[0] = 0xb0 | v8;
                _bSOSMIDIData[1] = 0x7b;
                _bSOSMIDIData[2] = 0;
                v20 = 3;
                _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                _bSOSMIDIData[0] = 0xb0 | v8;
                _bSOSMIDIData[1] = 0x79;
                _bSOSMIDIData[2] = 0;
                v20 = 3;
                _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v20, a3);
                if (_bSOSMIDIChannelDataIndex[a3][a1][v4] == 0xff)
                {
                    for (v1c = 0; v1c < 4; v1c++)
                    {
                        if (_sSOSMIDIChannelData[a3][v4][v1c].bUsed == 0xff)
                        {
                            _sSOSMIDIChannelData[a3][v4][v1c].bUsed = 1;
                            _bSOSMIDIChannelDataIndex[a3][a1][v4] = v1c;
                            break;
                        }
                    }
                }
                goto loc_101BE;
            }
            if (_bSOSMIDIChannelDataIndex[a3][a1][v4] == 0xff)
            {
                for (v1c = 0; v1c < 4; v1c++)
                {
                    if (_sSOSMIDIChannelData[a3][v4][v1c].bUsed == 0xff)
                    {
                        _sSOSMIDIChannelData[a3][v4][v1c].bUsed = 1;
                        _bSOSMIDIChannelDataIndex[a3][a1][v4] = v1c;
                        break;
                    }
                }
            }
        }
    }
    if (v4 != 9)
    {
        switch (vc & 0xf0)
        {
        case 0xb0:
            switch (a2[1])
            {
            case 7:
                _sSOSMIDIChannelData[a3][v4][_bSOSMIDIChannelDataIndex[a3][a1][v4]].bChannelVolume = a2[2];
                v24 = a2[2];
                _bSOSMIDIDeviceChannelVolume[a3][v8] = v24;
                break;
            case 64:
                _sSOSMIDIChannelData[a3][v4][_bSOSMIDIChannelDataIndex[a3][a1][v4]].bSustain = a2[2];
                break;
            }
            break;
        case 0xc0:
            _sSOSMIDIChannelData[a3][v4][_bSOSMIDIChannelDataIndex[a3][a1][v4]].bInstrument = a2[1];
            break;
        case 0xe0:
            _sSOSMIDIChannelData[a3][v4][_bSOSMIDIChannelDataIndex[a3][a1][v4]].bPitchBend = a2[2];
            break;
        }
    }
    else
    {
        if (vc == 0xb9 && a2[1] == 7)
        {
            v24 = a2[2];
            _bSOSMIDIDeviceChannelVolume[a3][9] = v24;
        }
    }
    if (v8 != 0xff)
    {
        if (v24 != 0xffffffff)
        {
            if (_wSOSMIDISongMuted[a1])
                a2[2] = 0;
            else
            {
                v28 = (_wSOSMIDISongVolume[a1] * v24) >> 7;
                v28 = (_bSOSMIDIMasterVolume * v28) >> 7;
                a2[2] = v28;
            }
        }
        _lpSOSMIDIDrvFunction[a3][_MIDI_DRV_SEND_DATA](a2, a4, a3);
        a2[0] = (vc & 0xf0) | v4;
        if (v24 != 0xffffffff)
            a2[2] = v24;
        return 0;
    }
    return -1;
}

BOOL sosMIDIResetChannelStealing(WORD a1)
{
    BYTE vc, v4, v8;
    WORD v14, v10, v1c;
    for (v14 = 1; v14 < _wSOSMIDITotalTracks[a1]; v14++)
    {
        v10 = _sSOSMIDITrackMap[a1]->wTrackDevice[v14];
        if (v10 != 0xffffffff && v10 != 0xff)
        {
            vc = _lpSOSMIDITrackHeaders[a1][v14]->wChannel;
            if (!_wSOSMIDIChannelStealing)
            {
                _bSOSMIDIData[0] = 0xb0 | vc;
                _bSOSMIDIData[1] = 0x7b;
                _bSOSMIDIData[2] = 0;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _bSOSMIDIData[0] = 0xb0 | vc;
                _bSOSMIDIData[1] = 0x79;
                _bSOSMIDIData[2] = 0;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _bSOSMIDIData[0] = 0xe0 | vc;
                _bSOSMIDIData[1] = 0x40;
                _bSOSMIDIData[2] = 0x40;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _bSOSMIDIData[0] = 0xb0 | vc;
                _bSOSMIDIData[1] = 0x7;
                _bSOSMIDIData[2] = 0x0;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
            }
            else
            {
                v4 = _bSOSMIDIChannelRemap[v10][a1][vc];
                _bSOSMIDIChannelRemap[v10][a1][vc] = 0xff;
                v8 = _bSOSMIDIChannelDataIndex[v10][a1][vc];
                _bSOSMIDIOwnerChannel[v10][v4] = 0xff;
                _bSOSMIDIOwnerSong[v10][v4] = 0xff;
                _bSOSMIDIData[0] = 0xb0 | v4;
                _bSOSMIDIData[1] = 0x7b;
                _bSOSMIDIData[2] = 0;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _bSOSMIDIData[0] = 0xb0 | v4;
                _bSOSMIDIData[1] = 0x79;
                _bSOSMIDIData[2] = 0;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _bSOSMIDIData[0] = 0xe0 | v4;
                _bSOSMIDIData[1] = 0x40;
                _bSOSMIDIData[2] = 0x40;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                _bSOSMIDIData[0] = 0xb0 | v4;
                _bSOSMIDIData[1] = 0x7;
                _bSOSMIDIData[2] = 0x0;
                v1c = 3;
                _lpSOSMIDIDrvFunction[v10][_MIDI_DRV_SEND_DATA](_bSOSMIDIData, v1c, v10);
                if (v8 != 0xff)
                {
                    _sSOSMIDIChannelData[v10][vc][v8].bInstrument = 0xff;
                    _sSOSMIDIChannelData[v10][vc][v8].bPitchBend = 0xff;
                    _sSOSMIDIChannelData[v10][vc][v8].bChannelVolume = 0xff;
                    _sSOSMIDIChannelData[v10][vc][v8].bSustain = 0xff;
                    _sSOSMIDIChannelData[v10][vc][v8].bUsed = 0xff;
                    _bSOSMIDIChannelDataIndex[v10][a1][vc] = 0xff;
                }
            }
        }
    }
    return 1;
}

WORD sosMIDISendMIDIData(WORD a1, LPSTR a2, WORD a3)
{
    _lpSOSMIDIDrvFunction[a1][_MIDI_DRV_SEND_DATA](a2, a3, a1);
    return 0;
}

WORD sosMIDIEnableChannelStealing(WORD a1)
{
    WORD v4;

    v4 = _wSOSMIDIChannelStealing;
    _wSOSMIDIChannelStealing = a1;
    return v4;
}

VOID  sosMIDISt_End(VOID)
{
}