#include "SOS.H"


VOID  sosMIDIPause_Start(VOID)
{
}


WORD  sosMIDIPauseSong(WORD a1, WORD a2)
{
    BYTE v8, v4;
    WORD vc, v10;
    _wSOSMIDISongPaused[a1] = 1;
    if (a2)
    {
        _wSOSMIDISongMuted[a1] = 1;
        for (vc = 0; vc < 32; vc++)
        {
            if (_lpSOSMIDITrack[a1][vc])
            {
                v8 = _lpSOSMIDITrackHeaders[a1][vc]->wChannel;
                v10 = _sSOSMIDITrackMap[a1]->wTrackDevice[vc];
                if (_wSOSMIDIChannelStealing)
                    v4 = _bSOSMIDIChannelRemap[v10][a1][v8];
                else
                    v4 = v8;
                _bSOSMIDIData1[0] = 0xb0 | v8;
                _bSOSMIDIData1[1] = 7;
                _bSOSMIDIData1[2] = _bSOSMIDIDeviceChannelVolume[v10][v4];
                sosMIDIHandleMIDIData(a1, _bSOSMIDIData1, v10, 3);
                _wSOSMIDIChannelMuted[v10][v4] = 1;
            }
        }
    }
    return 0;
}

WORD sosMIDIResumeSong(WORD a1)
{
    BYTE v8, v4;
    WORD v10, vc;
    _wSOSMIDISongPaused[a1] = 0;
    if (_wSOSMIDISongMuted[a1])
    {
        for (v10 = 0; v10 < 32; v10++)
        {
            if (_lpSOSMIDITrack[a1][v10])
            {
                vc = _sSOSMIDITrackMap[a1]->wTrackDevice[v10];
                v8 = _lspSOSMIDITrackHeaders[a1][v10]->wChannel;
                if (_wSOSMIDIChannelStealing)
                    v4 = _bSOSMIDIChannelRemap[vc][a1][v8];
                else
                    v4 = v8;
                _bSOSMIDIData1[0] = 0xb0 | v8;
                _bSOSMIDIData1[1] = 7;
                _bSOSMIDIData1[2] = _bSOSMIDIDeviceChannelVolume[vc][v4];
                sosMIDIHandleMIDIData(a1, _bSOSMIDIData1, vc, 3);
                _wSOSMIDIChannelMuted[vc][v4] = 0;
            }
        }
    }
    return 0;
}


WORD  sosMIDIMuteSong(WORD a1)
{
    BYTE v4, v8;
    WORD v8;
    for (v8 = 0; v8 < 32; v8++)
    {
        if (_lpSOSMIDITrack[a1][v8])
        {
            v4 = _lpSOSMIDITrackHeaders[a1][v8]->wChannel;
            _bSOSMIDIData1[0] = 0xb0 | v4;
            _bSOSMIDIData1[1] = 7;
            _bSOSMIDIData1[2] = 0;
            sosMIDIHandleMIDIData(a1, _bSOSMIDIData1, _sSOSMIDITrackMap[a1]->wTrackDevice[v8], 3);
        }
    }
    _wSOSMIDISongMuted[a1] = 1;
    return 0;
}

WORD  sosMIDIUnMuteSong(WORD a1)
{
    BYTE v8, v4;
    WORD v10, vc;
    _wSOSMIDISongMuted[a1] = 0;
    for (v10 = 0; v10 < 32; v10++)
    {
        if (_lpSOSMIDITrack[a1][v10])
        {
            vc = _sSOSMIDITrackMap[a1]->wTrackDevice[v10];
            v8 = _lspSOSMIDITrackHeaders[a1][v10]->wChannel;
            if (_wSOSMIDIChannelStealing)
                v4 = _bSOSMIDIChannelRemap[vc][a1][v8];
            else
                v4 = v8;
            _bSOSMIDIData1[0] = 0xb0 | v8;
            _bSOSMIDIData1[1] = 7;
            _bSOSMIDIData1[2] = _bSOSMIDIDeviceChannelVolume[vc][v4];
            sosMIDIHandleMIDIData(a1, _bSOSMIDIData1, vc, 3);
        }
    }
    return 0;
}

WORD sosMIDISetSongVolume(WORD a1, BYTE a2)
{
    BYTE v8, v4;
    WORD v10, v18;
    _wSOSMIDISongVolume[a1] = a2;
    for (v10 = 0; v10 < 32; v10++)
    {
        if (_lpSOSMIDITrack[a1][v10])
        {
            v8 = _lspSOSMIDITrackHeaders[a1][v10]->wChannel;
            v18 = _sSOSMIDITrackMap[a1]->wTrackDevice[v10];
            if (_wSOSMIDIChannelStealing)
                v4 = _bSOSMIDIChannelRemap[vc][a1][v8];
            else
                v4 = v8;
            _bSOSMIDIData1[0] = 0xb0 | v8;
            _bSOSMIDIData1[1] = 7;
            _bSOSMIDIData1[2] = _bSOSMIDIDeviceChannelVolume[v18][v4];
            sosMIDIHandleMIDIData(a1, _bSOSMIDIData1, _sSOSMIDITrackMap[a1]->wTrackDevice[v10], 3);
        }
    }
    return 0;
}

WORD sosMIDIFadeSong(WORD a1, WORD a2, WORD a3, BYTE a4, BYTE a5, WORD a6)
{
    WORD v8, vc, v10, v1c, v18;
    if (a2 & _SOS_MIDI_FADE_IN)
        v8 = a5 - a4;
    else
        v8 = a4 - a5;
    vc = sosTIMERGetEventRate(_SOSMIDIEventHandle[a1]);
    v1c = (100 << 16) / vc;
    v18 = (a3 << 16) / v1c;
    if (!v18)
    {
        if (a2 & _SOS_MIDI_FADE_OUT_STOP)
        {
            sosMIDIStopSong(a1);
            return 0;
        }
        sosMIDISetSongVolume(a1, a5);
        return 0;
    }
    sosMIDISetSongVolume(a1, a4);
    v10 = (v8 << 16) / v18;

    _wSOSMIDIVolumeFadeDirection[a1] = a2;
    _dwSOSMIDIVolumeFadeFraction[a1] = v10;
    _dwSOSMIDIVolumeFadeVolume[a1] = a4 << 16;
    _wSOSMIDIVolumeFadeTicks[a1] = v18;
    return 0;
}

VOID  sosMIDIPause_End(VOID)
{

}
