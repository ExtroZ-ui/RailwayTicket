#pragma once
#include "TicketManager.h"

using namespace System;
using namespace System::Windows::Forms;

public ref class ServiceForm : public Form {
private:
    ComboBox^ comboBoxWagonType;
    Button^ btnExtract;
    DataGridView^ dgvExtractedTickets;

    TicketManager^ ticketManager; // ссылка на главный менеджер билетов

    // Поле для хранения списка подписчиков
    EventHandler^ ticketsExtractedHandlers;

public:
    ServiceForm(TicketManager^ manager) {
        ticketManager = manager;
        InitializeComponent();
    }

    // Собственное событие (добавление и удаление подписчиков вручную)
    event EventHandler^ TicketsExtracted {
        void add(EventHandler^ handler) {
            ticketsExtractedHandlers = static_cast<EventHandler^>(
                Delegate::Combine(ticketsExtractedHandlers, handler));
        }
        void remove(EventHandler^ handler) {
            ticketsExtractedHandlers = static_cast<EventHandler^>(
                Delegate::Remove(ticketsExtractedHandlers, handler));
        }
    }

protected:
    void OnTicketsExtracted(EventArgs^ e) {
        if (ticketsExtractedHandlers != nullptr) {
            ticketsExtractedHandlers(this, e);
        }
    }

private:
    void InitializeComponent() {
        this->comboBoxWagonType = gcnew ComboBox();
        this->btnExtract = gcnew Button();
        this->dgvExtractedTickets = gcnew DataGridView();

        this->SuspendLayout();

        // ComboBox - тип вагона
        this->comboBoxWagonType->DropDownStyle = ComboBoxStyle::DropDownList;
        this->comboBoxWagonType->Items->AddRange(gcnew array<String^> { "К", "П", "Л" });
        this->comboBoxWagonType->Location = System::Drawing::Point(15, 15);
        this->comboBoxWagonType->Width = 100;
        this->comboBoxWagonType->SelectedIndex = 0;

        // Кнопка "Вынести"
        this->btnExtract->Text = "Вынести билеты";
        this->btnExtract->Location = System::Drawing::Point(130, 15);
        this->btnExtract->Click += gcnew EventHandler(this, &ServiceForm::btnExtract_Click);

        // DataGridView
        this->dgvExtractedTickets->Location = System::Drawing::Point(15, 50);
        this->dgvExtractedTickets->Size = System::Drawing::Size(450, 200);
        this->dgvExtractedTickets->ReadOnly = true;
        this->dgvExtractedTickets->AllowUserToAddRows = false;
        this->dgvExtractedTickets->AutoGenerateColumns = true;

        // Controls
        this->Controls->Add(this->comboBoxWagonType);
        this->Controls->Add(this->btnExtract);
        this->Controls->Add(this->dgvExtractedTickets);

        // Form settings
        this->ClientSize = System::Drawing::Size(480, 270);
        this->Text = "Вынесение билетов с услугами";

        this->ResumeLayout(false);
    }

    void btnExtract_Click(Object^ sender, EventArgs^ e) {
        if (comboBoxWagonType->SelectedItem == nullptr) {
            MessageBox::Show("Выберите тип вагона.");
            return;
        }

        Char wagonType = comboBoxWagonType->SelectedItem->ToString()[0];

        List<Ticket^>^ extracted = ticketManager->ExtractTicketsWithServices(wagonType);

        if (extracted->Count == 0) {
            MessageBox::Show("Билеты для выбранного типа вагона с платными услугами не найдены.");
            dgvExtractedTickets->DataSource = nullptr;
            return;
        }

        dgvExtractedTickets->DataSource = extracted;

        OnTicketsExtracted(EventArgs::Empty); // корректный вызов
    }
};
