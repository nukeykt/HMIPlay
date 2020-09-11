#include "SOS.H"


VOID  sosMIDITmpo_Start(VOID)
{

}

WORD  sosMIDISongAlterTempo(WORD a1, WORD a2)
{
    WORD v4, v8;
    if (sosMIDISongDone(a1))
        return 0;
    v4 = (a2 << 16) / 100;
    v8 = (_sSOSMIDISongHeader[a1]->wTempo * v4) >> 16;
    if (!v8)
        v8 = 1;
    sosTIMERAlterEventRate(_wSOSMIDIEventHandle[a1], v8);
    return sosTIMERGetEventRate(_wSOSMIDIEventHandle[a1]);
}

WORD sosMIDIGetTimeToPlay(WORD a1)
{
    return _sSOSMIDISongHeader[a1]->wTimeToPlay;
}

VOID  sosMIDITmpo_End(VOID)
{

}