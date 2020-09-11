#include "SOS.H"

VOID  sosMIDICbck_Start(VOID)
{

}

LPSTR callbackGetCallTable(VOID)
{
    return _lpSOSMIDICBCKDrvFunctions;
}

WORD cdecl far callbackSendData(LPSTR a1, WORD a2, WORD a3)
{
    _lpSOSMIDICBCKFunctions(a1, a2, a3);
    return 0;
}

WORD cdecl far callbackInit(LPSTR, WORD, WORD)
{
    return 0;
}

WORD cdecl far callbackUnInit(LPSTR, WORD, WORD)
{
    return 0;
}

WORD cdecl far callbackReset(LPSTR, WORD, WORD)
{
    return 0;
}


WORD cdecl far callbackSetInstrumentData(LPSTR a1, WORD, WORD)
{
    _lpSOSMIDICBCKFunctions = a1;
}

VOID  sosMIDICbck_End(VOID)
{
}
