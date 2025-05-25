#pragma once
#include "Models/TicketManager.h"
#include "Forms/MainForm.h"

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Collections::Generic;
using namespace System::IO;

public ref class ServiceForm : public Form {
private:
    ComboBox^ comboBoxWagonType;
    Button^ btnExtract;
    Button^ btnSave;
    DataGridView^ dgvExtractedTickets;

    TicketManager^ ticketManager;
    List<Ticket^>^ extractedTickets;

    EventHandler^ ticketsExtractedHandlers;

public:
    ServiceForm(TicketManager^ manager) {
        ticketManager = manager;
        InitializeComponent();
        UpdateTable();
    }

    event EventHandler^ TicketsExtracted {
        void add(EventHandler^ handler) {
            ticketsExtractedHandlers = static_cast<EventHandler^>(Delegate::Combine(ticketsExtractedHandlers, handler));
        }
        void remove(EventHandler^ handler) {
            ticketsExtractedHandlers = static_cast<EventHandler^>(Delegate::Remove(ticketsExtractedHandlers, handler));
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
        this->btnSave = gcnew Button();
        this->dgvExtractedTickets = gcnew DataGridView();

        this->SuspendLayout();

        this->comboBoxWagonType->DropDownStyle = ComboBoxStyle::DropDownList;
        this->comboBoxWagonType->Items->AddRange(gcnew array<String^> { "�", "�", "�" });
        this->comboBoxWagonType->Location = System::Drawing::Point(15, 15);
        this->comboBoxWagonType->Width = 100;
        this->comboBoxWagonType->SelectedIndexChanged += gcnew EventHandler(this, &ServiceForm::comboBoxWagonType_SelectedIndexChanged);
        this->comboBoxWagonType->SelectedIndex = 0;

        this->btnExtract->Text = "������� ������";
        this->btnExtract->Location = System::Drawing::Point(130, 15);
        this->btnExtract->Click += gcnew EventHandler(this, &ServiceForm::btnExtract_Click);

        this->btnSave->Text = "���������";
        this->btnSave->Location = System::Drawing::Point(270, 15);
        this->btnSave->Click += gcnew EventHandler(this, &ServiceForm::btnSave_Click);

        this->dgvExtractedTickets->Location = System::Drawing::Point(15, 50);
        this->dgvExtractedTickets->Size = System::Drawing::Size(600, 250);
        this->dgvExtractedTickets->ReadOnly = true;
        this->dgvExtractedTickets->AllowUserToAddRows = false;
        this->dgvExtractedTickets->AutoGenerateColumns = false;

        this->Controls->Add(this->comboBoxWagonType);
        this->Controls->Add(this->btnExtract);
        this->Controls->Add(this->btnSave);
        this->Controls->Add(this->dgvExtractedTickets);

        this->ClientSize = System::Drawing::Size(640, 320);
        this->Text = "��������� ������� � ��������";

        this->ResumeLayout(false);
    }

    void comboBoxWagonType_SelectedIndexChanged(Object^ sender, EventArgs^ e) {
        try {
            UpdateTable();
        }
        catch (Exception^ ex) {
            MessageBox::Show("������ ��� ���������� �������: " + ex->Message,
                "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void UpdateTable() {
        dgvExtractedTickets->DataSource = nullptr;
        dgvExtractedTickets->Columns->Clear();
        dgvExtractedTickets->Rows->Clear();

        if (comboBoxWagonType->SelectedItem == nullptr)
            return;

        Char wagonType = comboBoxWagonType->SelectedItem->ToString()[0];

        List<Ticket^>^ previewList = nullptr;

        try {
            previewList = ticketManager->GetTicketsWithServicesPreview(wagonType);
        }
        catch (Exception^ ex) {
            MessageBox::Show("������ ��� ��������� ������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        if (previewList == nullptr || previewList->Count == 0) {
            MessageBox::Show("��� ������� � ��������������� ��������.", "����������", MessageBoxButtons::OK, MessageBoxIcon::Information);
            return;
        }

        dgvExtractedTickets->Columns->Add("passenger", "�������");
        dgvExtractedTickets->Columns->Add("train", "� ������");
        dgvExtractedTickets->Columns->Add("from", "������");
        dgvExtractedTickets->Columns->Add("to", "����");
        dgvExtractedTickets->Columns->Add("type", "���");
        dgvExtractedTickets->Columns->Add("price", "����");
        dgvExtractedTickets->Columns->Add("discount", "������");
        dgvExtractedTickets->Columns->Add("datetime", "���� � �����");

        try {
            for each (Ticket ^ t in previewList) {
                dgvExtractedTickets->Rows->Add(
                    t->passengerName,
                    t->trainNumber,
                    t->departureCity,
                    t->arrivalCity,
                    Convert::ToString(t->wagonType),
                    t->price.ToString("F2"),
                    t->hasDiscount ? "��" : "���",
                    String::Format("{0:D2}.{1:D2}.{2:D4} {3:D2}:{4:D2}:{5:D2}",
                        t->dateTime.date.day, t->dateTime.date.month, t->dateTime.date.year,
                        t->dateTime.time.hour, t->dateTime.time.minute, t->dateTime.time.second)
                );
            }
        }
        catch (Exception^ ex) {
            MessageBox::Show("������ ��� ���������� �������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }
    }

    void btnExtract_Click(Object^ sender, EventArgs^ e) {
        if (comboBoxWagonType->SelectedItem == nullptr) {
            MessageBox::Show("�������� ��� ������.");
            return;
        }

        Char wagonType = comboBoxWagonType->SelectedItem->ToString()[0];

        try {
            extractedTickets = gcnew List<Ticket^>(ticketManager->ExtractTicketsWithServices(wagonType));
        }
        catch (Exception^ ex) {
            MessageBox::Show("������ ��� ���������� �������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            return;
        }

        if (extractedTickets == nullptr || extractedTickets->Count == 0) {
            MessageBox::Show("������ �� �������.");
            return;
        }

        dgvExtractedTickets->DataSource = nullptr;
        dgvExtractedTickets->AutoGenerateColumns = true;

        try {
            dgvExtractedTickets->DataSource = extractedTickets;
        }
        catch (Exception^ ex) {
            MessageBox::Show("������ ��� ����������� �������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
        }

        OnTicketsExtracted(EventArgs::Empty);
    }

    void btnSave_Click(Object^ sender, EventArgs^ e) {
        if (dgvExtractedTickets->Rows->Count == 0) {
            MessageBox::Show("��� ������ ��� ����������.");
            return;
        }

        SaveFileDialog^ saveDialog = gcnew SaveFileDialog();
        saveDialog->Filter = "��������� ����� (*.txt)|*.txt";
        saveDialog->Title = "��������� ������";

        if (saveDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
            try {
                StreamWriter^ writer = gcnew StreamWriter(saveDialog->FileName);

                // ���������
                for (int i = 0; i < dgvExtractedTickets->ColumnCount; ++i) {
                    writer->Write(dgvExtractedTickets->Columns[i]->HeaderText);
                    if (i < dgvExtractedTickets->ColumnCount - 1)
                        writer->Write(",");
                }
                writer->WriteLine();

                // ������
                for (int i = 0; i < dgvExtractedTickets->Rows->Count; ++i) {
                    if (dgvExtractedTickets->Rows[i]->IsNewRow) continue;

                    for (int j = 0; j < dgvExtractedTickets->ColumnCount; ++j) {
                        Object^ val = dgvExtractedTickets->Rows[i]->Cells[j]->Value;
                        writer->Write(val != nullptr ? val->ToString() : "");
                        if (j < dgvExtractedTickets->ColumnCount - 1)
                            writer->Write(",");
                    }
                    writer->WriteLine();
                }

                writer->Close();
                MessageBox::Show("���� ������� �������.");
            }
            catch (IOException^ ioEx) {
                MessageBox::Show("������ ��� ������ �����: " + ioEx->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
            catch (Exception^ ex) {
                MessageBox::Show("�������������� ������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }
    }
};
