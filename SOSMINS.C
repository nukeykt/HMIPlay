#include "SOS.H"

VOID  sosMIDIIns_Start(VOID)
{
}

WORD  sosMIDISetInsData(WORD a1, LPSTR a2, WORD a3)
{
    return _lpSOSMIDIDrvFunction[a1][_MIDI_DRV_SET_INST_DATA](a2, a1, a3);
}

WORD sosMIDISetMT32InsData(WORD a1, LPSTR a2, WORD a3)
{
    WORD v4, v10, v8, vc;
    if (a3)
    {
        for (v4 = 0; v4 < 0xffff; v4++)
        {
        }
        v10 = sosTIMERRegisterEvent(1500, sosMIDIMT32Timer, &v8);
        if (v10)
            return v10;
        for (v4 = 0; v4 < a3; v4++)
        {
            while (!_bSOSMIDIMT32SendData)
                vc = 1;
            _bSOSMIDIMT32SendData = 0;
            vc = 1;
            _lpSOSMIDIDrvFunction[a1][_MIDI_DRV_SET_INST_DATA](a2 + v4, vc, a1);
        }
        sosTIMERRemoveEvent(v8);
    }
    return 0;
}

VOID far sosMIDIMT32Timer(VOID)
{
    _bSOSMIDIMT32SendData = 1;
}

VOID  sosMIDIIns_End(VOID)
{

}