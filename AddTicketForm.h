#pragma once
#include "Ticket.h"

using namespace System;
using namespace System::Windows::Forms;

public ref class AddTicketForm : public Form {
public:
    Ticket^ result;

    AddTicketForm() {
        this->Text = "Добавить билет";
        this->Width = 400;
        this->Height = 500;

        Label^ nameLabel = gcnew Label();
        nameLabel->Text = "Фамилия:";
        nameLabel->Top = 10;
        nameLabel->Left = 10;

        TextBox^ nameBox = gcnew TextBox();
        nameBox->Top = 30;
        nameBox->Left = 10;
        nameBox->Width = 350;

        Label^ trainLabel = gcnew Label();
        trainLabel->Text = "Номер поезда:";
        trainLabel->Top = 60;
        trainLabel->Left = 10;

        NumericUpDown^ trainBox = gcnew NumericUpDown();
        trainBox->Top = 80;
        trainBox->Left = 10;
        trainBox->Maximum = 9999;

        Label^ depLabel = gcnew Label();
        depLabel->Text = "Город отправления:";
        depLabel->Top = 110;
        depLabel->Left = 10;

        ComboBox^ depBox = gcnew ComboBox();
        depBox->Top = 130;
        depBox->Left = 10;
        depBox->Width = 350;
        depBox->DropDownStyle = ComboBoxStyle::DropDown;
        depBox->Items->AddRange(gcnew array<String^>{"Москва", "Санкт-Петербург", "Новосибирск", "Другой..."});

        Label^ arrLabel = gcnew Label();
        arrLabel->Text = "Город прибытия:";
        arrLabel->Top = 160;
        arrLabel->Left = 10;

        ComboBox^ arrBox = gcnew ComboBox();
        arrBox->Top = 180;
        arrBox->Left = 10;
        arrBox->Width = 350;
        arrBox->DropDownStyle = ComboBoxStyle::DropDown;
        arrBox->Items->AddRange(gcnew array<String^>{"Казань", "Екатеринбург", "Владивосток", "Другой..."});

        Label^ typeLabel = gcnew Label();
        typeLabel->Text = "Тип вагона:";
        typeLabel->Top = 210;
        typeLabel->Left = 10;

        ComboBox^ typeBox = gcnew ComboBox();
        typeBox->Top = 230;
        typeBox->Left = 10;
        typeBox->DropDownStyle = ComboBoxStyle::DropDownList;
        typeBox->Items->AddRange(gcnew array<String^>{ "П", "К", "С" });

        Label^ priceLabel = gcnew Label();
        priceLabel->Text = "Цена:";
        priceLabel->Top = 260;
        priceLabel->Left = 10;

        NumericUpDown^ priceBox = gcnew NumericUpDown();
        priceBox->Top = 280;
        priceBox->Left = 10;
        priceBox->Maximum = 100000;
        priceBox->DecimalPlaces = 2;

        Label^ discountLabel = gcnew Label();
        discountLabel->Text = "Льгота:";
        discountLabel->Top = 310;
        discountLabel->Left = 10;

        ComboBox^ discountBox = gcnew ComboBox();
        discountBox->Top = 330;
        discountBox->Left = 10;
        discountBox->DropDownStyle = ComboBoxStyle::DropDownList;
        discountBox->Items->AddRange(gcnew array<String^>{ "Нет", "Да" });

        Label^ dtLabel = gcnew Label();
        dtLabel->Text = "Дата и время:";
        dtLabel->Top = 360;
        dtLabel->Left = 10;

        DateTimePicker^ dtPicker = gcnew DateTimePicker();
        dtPicker->Top = 380;
        dtPicker->Left = 10;
        dtPicker->Format = DateTimePickerFormat::Custom;
        dtPicker->CustomFormat = "dd.MM.yyyy HH:mm:ss";
        dtPicker->Width = 200;

        Button^ okBtn = gcnew Button();
        okBtn->Text = "OK";
        okBtn->Top = 420;
        okBtn->Left = 10;
        okBtn->Click += gcnew EventHandler(this, &AddTicketForm::OnOK);

        this->Controls->AddRange(gcnew array<Control^>{
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

        this->nameBox = nameBox;
        this->trainBox = trainBox;
        this->depBox = depBox;
        this->arrBox = arrBox;
        this->typeBox = typeBox;
        this->priceBox = priceBox;
        this->discountBox = discountBox;
        this->dtPicker = dtPicker;
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

    void OnOK(Object^ sender, EventArgs^ e) {
        try {
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
            MessageBox::Show("Ошибка ввода: " + ex->Message);
        }
    }
};
