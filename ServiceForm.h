#pragma once
#include "TicketManager.h"

using namespace System;
using namespace System::Windows::Forms;

public ref class ServiceForm : public Form {
private:
    ComboBox^ comboBoxWagonType;
    Button^ btnExtract;
    DataGridView^ dgvExtractedTickets;

    TicketManager^ ticketManager; // ������ �� ������� �������� �������

    // ���� ��� �������� ������ �����������
    EventHandler^ ticketsExtractedHandlers;

public:
    ServiceForm(TicketManager^ manager) {
        ticketManager = manager;
        InitializeComponent();
    }

    // ����������� ������� (���������� � �������� ����������� �������)
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

        // ComboBox - ��� ������
        this->comboBoxWagonType->DropDownStyle = ComboBoxStyle::DropDownList;
        this->comboBoxWagonType->Items->AddRange(gcnew array<String^> { "�", "�", "�" });
        this->comboBoxWagonType->Location = System::Drawing::Point(15, 15);
        this->comboBoxWagonType->Width = 100;
        this->comboBoxWagonType->SelectedIndex = 0;

        // ������ "�������"
        this->btnExtract->Text = "������� ������";
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
        this->Text = "��������� ������� � ��������";

        this->ResumeLayout(false);
    }

    void btnExtract_Click(Object^ sender, EventArgs^ e) {
        if (comboBoxWagonType->SelectedItem == nullptr) {
            MessageBox::Show("�������� ��� ������.");
            return;
        }

        Char wagonType = comboBoxWagonType->SelectedItem->ToString()[0];

        List<Ticket^>^ extracted = ticketManager->ExtractTicketsWithServices(wagonType);

        if (extracted->Count == 0) {
            MessageBox::Show("������ ��� ���������� ���� ������ � �������� �������� �� �������.");
            dgvExtractedTickets->DataSource = nullptr;
            return;
        }

        dgvExtractedTickets->DataSource = extracted;

        OnTicketsExtracted(EventArgs::Empty); // ���������� �����
    }
};
