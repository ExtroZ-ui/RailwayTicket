#pragma once
#include "Models/Ticket.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class AddTicketForm : public Form {
public:
    Ticket^ result;

    // Конструктор по умолчанию (для добавления нового билета)
    AddTicketForm() {
        InitializeComponents();
        this->Text = "Добавить билет";
    }

    // Конструктор для редактирования существующего билета
    AddTicketForm(Ticket^ ticket) {
        InitializeComponents();
        this->Text = "Редактировать билет";

        // Заполняем поля данными из ticket
        nameBox->Text = ticket->passengerName;
        trainBox->Value = ticket->trainNumber;
        depBox->Text = ticket->departureCity;
        arrBox->Text = ticket->arrivalCity;
        typeBox->Text = ticket->wagonType.ToString();
        priceBox->Value = Decimal(ticket->price);
        discountBox->Text = ticket->hasDiscount ? "Да" : "Нет";

        DateTime dt(
            ticket->dateTime.date.year,
            ticket->dateTime.date.month,
            ticket->dateTime.date.day,
            ticket->dateTime.time.hour,
            ticket->dateTime.time.minute,
            ticket->dateTime.time.second
        );
        dtPicker->Value = dt;
    }

private:
    TextBox^ nameBox;
    NumericUpDown^ trainBox;
    ComboBox^ depBox;
    ComboBox^ arrBox;
    ComboBox^ typeBox;
    NumericUpDown^ priceBox;
    ComboBox^ discountBox;
    DateTimePicker^ dtPicker;

    void InitializeComponents() {
        this->ClientSize = System::Drawing::Size(400, 500);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;

        int top = 20;
        int spacing = 40;

        Label^ nameLabel = gcnew Label();
        nameLabel->Text = "Фамилия:";
        nameLabel->Location = Point(20, top);
        nameLabel->AutoSize = true;
        nameBox = gcnew TextBox();
        nameBox->Location = Point(150, top);
        nameBox->Width = 200;

        top += spacing;
        Label^ trainLabel = gcnew Label();
        trainLabel->Text = "Номер поезда:";
        trainLabel->Location = Point(20, top);
        trainLabel->AutoSize = true;
        trainBox = gcnew NumericUpDown();
        trainBox->Location = Point(150, top);
        trainBox->Maximum = 9999;

        top += spacing;
        Label^ depLabel = gcnew Label();
        depLabel->Text = "Город отправления:";
        depLabel->Location = Point(20, top);
        depLabel->AutoSize = true;
        depBox = gcnew ComboBox();
        depBox->Location = Point(150, top);
        depBox->Width = 200;
        depBox->DropDownStyle = ComboBoxStyle::DropDown;
        depBox->Items->AddRange(gcnew array<String^>{"Москва", "Санкт-Петербург", "Новосибирск", "Другой..."});

        top += spacing;
        Label^ arrLabel = gcnew Label();
        arrLabel->Text = "Город прибытия:";
        arrLabel->Location = Point(20, top);
        arrLabel->AutoSize = true;
        arrBox = gcnew ComboBox();
        arrBox->Location = Point(150, top);
        arrBox->Width = 200;
        arrBox->DropDownStyle = ComboBoxStyle::DropDown;
        arrBox->Items->AddRange(gcnew array<String^>{"Казань", "Екатеринбург", "Владивосток", "Другой..."});

        top += spacing;
        Label^ typeLabel = gcnew Label();
        typeLabel->Text = "Тип вагона:";
        typeLabel->Location = Point(20, top);
        typeLabel->AutoSize = true;
        typeBox = gcnew ComboBox();
        typeBox->Location = Point(150, top);
        typeBox->DropDownStyle = ComboBoxStyle::DropDownList;
        typeBox->Items->AddRange(gcnew array<String^>{ "П", "К", "С" });

        top += spacing;
        Label^ priceLabel = gcnew Label();
        priceLabel->Text = "Цена:";
        priceLabel->Location = Point(20, top);
        priceLabel->AutoSize = true;
        priceBox = gcnew NumericUpDown();
        priceBox->Location = Point(150, top);
        priceBox->Maximum = 100000;
        priceBox->DecimalPlaces = 2;

        top += spacing;
        Label^ discountLabel = gcnew Label();
        discountLabel->Text = "Льгота:";
        discountLabel->Location = Point(20, top);
        discountLabel->AutoSize = true;
        discountBox = gcnew ComboBox();
        discountBox->Location = Point(150, top);
        discountBox->DropDownStyle = ComboBoxStyle::DropDownList;
        discountBox->Items->AddRange(gcnew array<String^>{ "Нет", "Да" });

        top += spacing;
        Label^ dtLabel = gcnew Label();
        dtLabel->Text = "Дата и время:";
        dtLabel->Location = Point(20, top);
        dtLabel->AutoSize = true;
        dtPicker = gcnew DateTimePicker();
        dtPicker->Location = Point(150, top);
        dtPicker->Format = DateTimePickerFormat::Custom;
        dtPicker->CustomFormat = "dd.MM.yyyy HH:mm:ss";
        dtPicker->Width = 200;
        dtPicker->ShowUpDown = true;

        top += spacing + 10;
        Button^ okBtn = gcnew Button();
        okBtn->Text = "OK";
        okBtn->Location = Point(150, top);
        okBtn->Width = 100;
        okBtn->Click += gcnew EventHandler(this, &AddTicketForm::OnOK);

        this->Controls->AddRange(gcnew array<Control^> {
            nameLabel, nameBox,
                trainLabel, trainBox,
                depLabel, depBox,
                arrLabel, arrBox,
                typeLabel, typeBox,
                priceLabel, priceBox,
                discountLabel, discountBox,
                dtLabel, dtPicker,
                okBtn
        });

        this->AcceptButton = okBtn;
    }

    void OnOK(Object^ sender, EventArgs^ e) {
        try {
            if (String::IsNullOrWhiteSpace(nameBox->Text) ||
                depBox->Text == "" || arrBox->Text == "" || typeBox->Text == "" || discountBox->Text == "")
            {
                MessageBox::Show("Пожалуйста, заполните все поля.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Warning);
                return;
            }

            String^ name = nameBox->Text;
            unsigned short train = Convert::ToUInt16(trainBox->Value);
            String^ dep = depBox->Text;
            String^ arr = arrBox->Text;
            wchar_t type = typeBox->Text[0];
            float price = Convert::ToSingle(priceBox->Value);
            bool discount = (discountBox->Text == "Да");

            DateTime dt = dtPicker->Value;
            CDateTime d;
            d.date.year = dt.Year;
            d.date.month = dt.Month;
            d.date.day = dt.Day;
            d.time.hour = dt.Hour;
            d.time.minute = dt.Minute;
            d.time.second = dt.Second;

            result = gcnew Ticket(name, train, dep, arr, type, price, discount, d);
            this->DialogResult = Windows::Forms::DialogResult::OK;
            this->Close();
        }
        catch (Exception^ ex) {
            MessageBox::Show("Ошибка ввода: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }
};
