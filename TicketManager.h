#pragma once
#include "Ticket.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

public ref class TicketManager {
private:
    List<Ticket^>^ tickets;

    ref class TrainNumberComparer : public IComparer<Ticket^> {
    public:
        virtual int Compare(Ticket^ a, Ticket^ b) {
            return a->trainNumber.CompareTo(b->trainNumber);
        }
    };

public:
    TicketManager() {
        tickets = gcnew List<Ticket^>();
    }

    void Load(String^ filename) {
        tickets->Clear();
        if (!File::Exists(filename)) {
            throw gcnew FileNotFoundException("Файл не найден: " + filename);
        }

        try {
            for each (String ^ line in File::ReadAllLines(filename)) {
                if (!String::IsNullOrWhiteSpace(line)) {
                    try {
                        tickets->Add(Ticket::FromLine(line));
                    }
                    catch (FormatException^ ex) {
                        Console::WriteLine("Строка пропущена: {0}", ex->Message);
                    }
                }
            }
        }
        catch (Exception^ ex) {
            throw gcnew IOException("Ошибка при чтении файла: " + ex->Message);
        }
    }

    void Save(String^ filename) {
        try {
            List<String^>^ lines = gcnew List<String^>();
            for each (Ticket ^ t in tickets)
                lines->Add(t->ToLine());

            File::WriteAllLines(filename, lines);
        }
        catch (Exception^ ex) {
            throw gcnew IOException("Ошибка при записи файла: " + ex->Message);
        }
    }

    List<Ticket^>^ GetAll() {
        return tickets;
    }

    void Add(Ticket^ t) {
        if (t == nullptr) throw gcnew ArgumentNullException("Объект билета не может быть null");
        tickets->Add(t);
    }

    void Update(int index, Ticket^ updated) {
        if (index < 0 || index >= tickets->Count)
            throw gcnew ArgumentOutOfRangeException("Недопустимый индекс для изменения: " + index);
        if (updated == nullptr)
            throw gcnew ArgumentNullException("Новый билет не может быть null");

        tickets[index] = updated;
    }

    void Delete(int index) {
        if (index >= 0 && index < tickets->Count)
            tickets->RemoveAt(index);
        else
            throw gcnew ArgumentOutOfRangeException("Недопустимый индекс для удаления: " + index);
    }

    void SortByTrainNumber() {
        tickets->Sort(gcnew TrainNumberComparer());
    }

    List<Ticket^>^ FindByTrainNumber(unsigned short number) {
        List<Ticket^>^ res = gcnew List<Ticket^>();
        for each (Ticket ^ t in tickets)
            if (t->trainNumber == number)
                res->Add(t);
        return res;
    }

    List<Ticket^>^ FilterByPrice(float min, float max) {
        if (min > max) throw gcnew ArgumentException("Минимальная цена не может быть больше максимальной");

        List<Ticket^>^ res = gcnew List<Ticket^>();
        for each (Ticket ^ t in tickets)
            if (t->price >= min && t->price <= max)
                res->Add(t);
        return res;
    }

    List<Ticket^>^ ExtractWithServices(Char wagonType) {
        List<Ticket^>^ result = gcnew List<Ticket^>();
        for (int i = tickets->Count - 1; i >= 0; i--) {
            Ticket^ t = tickets[i];
            if (t->wagonType == wagonType && !t->hasDiscount) {
                result->Add(t);
                tickets->RemoveAt(i);
            }
        }
        return result;
    }

    Dictionary<String^, int>^ CityStats() {
        Dictionary<String^, int>^ stats = gcnew Dictionary<String^, int>();
        for each (Ticket ^ t in tickets) {
            for each (String ^ city in gcnew array<String^> { t->departureCity, t->arrivalCity }) {
                if (!stats->ContainsKey(city))
                    stats[city] = 0;
                stats[city]++;
            }
        }
        return stats;
    }
};
