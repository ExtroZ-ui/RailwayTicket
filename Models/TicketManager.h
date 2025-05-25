#pragma once
#include "Models/Ticket.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

public ref class TicketManager {
private:
    List<Ticket^>^ tickets;

    ref class TrainNumberComparer : public IComparer<Ticket^> {
    public:
        virtual int Compare(Ticket^ a, Ticket^ b) {
            if (a == nullptr || b == nullptr) return 0;
            return a->trainNumber.CompareTo(b->trainNumber);
        }
    };

public:
    TicketManager() {
        tickets = gcnew List<Ticket^>();
    }

    void Load(String^ filename) {
        tickets->Clear();
        if (String::IsNullOrWhiteSpace(filename))
            throw gcnew ArgumentException("��� ����� �� ����� ���� ������.");

        if (!File::Exists(filename)) {
            throw gcnew FileNotFoundException("���� �� ������: " + filename);
        }

        try {
            for each (String ^ line in File::ReadAllLines(filename)) {
                if (!String::IsNullOrWhiteSpace(line)) {
                    try {
                        Ticket^ t = Ticket::FromLine(line);
                        if (t != nullptr) {
                            tickets->Add(t);
                        }
                    }
                    catch (FormatException^ ex) {
                        Console::WriteLine("������ ��������� (������ �������): {0}", ex->Message);
                    }
                    catch (Exception^ ex) {
                        Console::WriteLine("����������� ������ ��� ������� ������: {0}", ex->Message);
                    }
                }
            }
        }
        catch (Exception^ ex) {
            throw gcnew IOException("������ ��� ������ �����: " + ex->Message);
        }
    }

    void Save(String^ filename) {
        if (String::IsNullOrWhiteSpace(filename))
            throw gcnew ArgumentException("��� ����� �� ����� ���� ������.");

        try {
            List<String^>^ lines = gcnew List<String^>();
            for each (Ticket ^ t in tickets) {
                if (t != nullptr)
                    lines->Add(t->ToLine());
            }

            File::WriteAllLines(filename, lines);
        }
        catch (Exception^ ex) {
            throw gcnew IOException("������ ��� ������ �����: " + ex->Message);
        }
    }

    List<Ticket^>^ GetAll() {
        return tickets;
    }

    void Add(Ticket^ t) {
        if (t == nullptr) throw gcnew ArgumentNullException("����� �� ����� ���� null");
        tickets->Add(t);
    }

    void Update(int index, Ticket^ updated) {
        if (updated == nullptr)
            throw gcnew ArgumentNullException("����� ����� �� ����� ���� null");

        if (index < 0 || index >= tickets->Count)
            throw gcnew ArgumentOutOfRangeException("������������ ������: " + index);

        tickets[index] = updated;
    }

    void Delete(int index) {
        if (index < 0 || index >= tickets->Count)
            throw gcnew ArgumentOutOfRangeException("������������ ������: " + index);
        tickets->RemoveAt(index);
    }

    void SortByTrainNumber() {
        try {
            tickets->Sort(gcnew TrainNumberComparer());
        }
        catch (Exception^ ex) {
            throw gcnew InvalidOperationException("������ ��� ����������: " + ex->Message);
        }
    }

    List<Ticket^>^ FindByTrainNumber(unsigned short number) {
        List<Ticket^>^ res = gcnew List<Ticket^>();
        for each (Ticket ^ t in tickets)
            if (t != nullptr && t->trainNumber == number)
                res->Add(t);
        return res;
    }

    List<Ticket^>^ FilterByPrice(float min, float max) {
        if (min > max)
            throw gcnew ArgumentException("����������� ���� �� ����� ���� ������ ������������.");

        List<Ticket^>^ res = gcnew List<Ticket^>();
        for each (Ticket ^ t in tickets)
            if (t != nullptr && t->price >= min && t->price <= max)
                res->Add(t);
        return res;
    }

    List<Ticket^>^ ExtractTicketsWithServices(Char wagonType) {
        List<Ticket^>^ extracted = gcnew List<Ticket^>();

        try {
            for (int i = tickets->Count - 1; i >= 0; i--) {
                Ticket^ t = tickets[i];
                if (t != nullptr && t->wagonType == wagonType && !t->hasDiscount) {
                    extracted->Add(t);
                    tickets->RemoveAt(i);
                }
            }
        }
        catch (Exception^ ex) {
            throw gcnew InvalidOperationException("������ ��� ���������� �������: " + ex->Message);
        }

        return extracted;
    }

    List<Ticket^>^ GetTicketsWithServicesPreview(Char wagonType) {
        List<Ticket^>^ result = gcnew List<Ticket^>();
        try {
            for each (Ticket ^ t in tickets) {
                if (t != nullptr && t->wagonType == wagonType && !t->hasDiscount)
                    result->Add(t);
            }
        }
        catch (Exception^ ex) {
            throw gcnew InvalidOperationException("������ ��� ���������� �������: " + ex->Message);
        }

        return result;
    }

    Dictionary<String^, int>^ DepartureCityStats() {
        auto dict = gcnew Dictionary<String^, int>();
        for each (Ticket ^ t in tickets) {
            if (t == nullptr || String::IsNullOrEmpty(t->departureCity)) continue;

            if (!dict->ContainsKey(t->departureCity))
                dict[t->departureCity] = 0;
            dict[t->departureCity]++;
        }
        return dict;
    }

    Dictionary<String^, int>^ ArrivalCityStats() {
        auto dict = gcnew Dictionary<String^, int>();
        for each (Ticket ^ t in tickets) {
            if (t == nullptr || String::IsNullOrEmpty(t->arrivalCity)) continue;

            if (!dict->ContainsKey(t->arrivalCity))
                dict[t->arrivalCity] = 0;
            dict[t->arrivalCity]++;
        }
        return dict;
    }
};
