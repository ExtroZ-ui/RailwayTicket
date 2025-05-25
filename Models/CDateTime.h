#pragma once

public value struct CTime {
    unsigned short hour, minute, second;
};

public value struct CDate {
    unsigned short year, month, day;
};

public value struct CDateTime {
    CDate date;
    CTime time;
};
