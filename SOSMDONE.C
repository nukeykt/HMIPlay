#include "SOS.H"


VOID  sosMIDIDone_Start(VOID)
{
}

BOOL  sosMIDISongDone(WORD a1)
{
    WORD vc;
    if (!_wSOSMIDISongActive[a1])
        vc = 1;
    else
        vc = 0;
    return vc;
}

VOID  sosMIDIDone_End(VOID)
{
}