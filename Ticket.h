#pragma once
#include "CDateTime.h"

using namespace System;

public ref class Ticket {
public:
    String^ passengerName;
    unsigned short trainNumber;
    String^ departureCity;
    String^ arrivalCity;
    wchar_t wagonType;
    float price;
    bool hasDiscount;
    CDateTime dateTime;

    Ticket(String^ name, unsigned short num, String^ dep, String^ arr, wchar_t type, float cost, bool discount, CDateTime dt) {
        passengerName = name;
        trainNumber = num;
        departureCity = dep;
        arrivalCity = arr;
        wagonType = type;
        price = cost;
        hasDiscount = discount;
        dateTime = dt;
    }

    String^ ToLine() {
        return String::Format("{0};{1};{2};{3};{4};{5};{6};{7}.{8}.{9};{10}:{11}:{12}",
            passengerName, trainNumber, departureCity, arrivalCity, wagonType, price, hasDiscount,
            dateTime.date.year, dateTime.date.month, dateTime.date.day,
            dateTime.time.hour, dateTime.time.minute, dateTime.time.second);
    }

    static Ticket^ FromLine(String^ line) {
        array<String^>^ parts = line->Split(';');
        wchar_t wagonType = parts[4][0];

        array<String^>^ dateTimeParts = parts[7]->Split(' ');
        array<String^>^ dateParts = dateTimeParts[0]->Split('.');
        array<String^>^ timeParts = dateTimeParts[1]->Split(':');

        CDateTime dt;
        dt.date.year = Convert::ToUInt16(dateParts[0]);
        dt.date.month = Convert::ToUInt16(dateParts[1]);
        dt.date.day = Convert::ToUInt16(dateParts[2]);
        dt.time.hour = Convert::ToUInt16(timeParts[0]);
        dt.time.minute = Convert::ToUInt16(timeParts[1]);
        dt.time.second = Convert::ToUInt16(timeParts[2]);

        return gcnew Ticket(
            parts[0],
            Convert::ToUInt16(parts[1]),
            parts[2],
            parts[3],
            wagonType,
            Convert::ToSingle(parts[5]),
            Convert::ToBoolean(parts[6]),
            dt
        );
    }
};
