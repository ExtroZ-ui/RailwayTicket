#pragma once
#include "TicketManager.h"
#include "AddTicketForm.h"
#include "PriceRangeForm.h"
#include "StatsForm.h"
#include "ServiceForm.h"
#using <Microsoft.VisualBasic.dll>

namespace TrainTickets {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace Microsoft::VisualBasic;

    public ref class MainForm : public Form {
    private:
        TicketManager^ manager;
        DataGridView^ grid;
        Button^ loadBtn;
        Button^ saveBtn;
        Button^ addBtn;
        Button^ deleteBtn;
        Button^ sortBtn;
        Button^ findBtn;
        Button^ priceBtn;
        Button^ servicesBtn;
        Button^ statsBtn;
        Button^ editBtn;

    public:
        MainForm() {
            InitializeComponent();
            manager = gcnew TicketManager();
        }

        void InitializeComponent() {
            this->Text = "��������������� �����";
            this->Size = Drawing::Size(1000, 600);
            this->StartPosition = FormStartPosition::CenterScreen;

            grid = gcnew DataGridView();
            grid->Location = Drawing::Point(10, 10);
            grid->Size = Drawing::Size(960, 400);
            grid->ReadOnly = true;
            grid->AllowUserToAddRows = false;
            grid->AllowUserToDeleteRows = false;
            grid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            grid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            this->Controls->Add(grid);

            FlowLayoutPanel^ panel = gcnew FlowLayoutPanel();
            panel->Location = Drawing::Point(10, 420);
            panel->Size = Drawing::Size(960, 130);
            panel->FlowDirection = FlowDirection::LeftToRight;

            loadBtn = gcnew Button(); loadBtn->Text = "���������"; loadBtn->Click += gcnew EventHandler(this, &MainForm::OnLoad);
            saveBtn = gcnew Button(); saveBtn->Text = "���������"; saveBtn->Click += gcnew EventHandler(this, &MainForm::OnSave);
            addBtn = gcnew Button(); addBtn->Text = "��������"; addBtn->Click += gcnew EventHandler(this, &MainForm::OnAdd);
            editBtn = gcnew Button(); editBtn->Text = "��������"; editBtn->Click += gcnew EventHandler(this, &MainForm::OnEdit);
            deleteBtn = gcnew Button(); deleteBtn->Text = "�������"; deleteBtn->Click += gcnew EventHandler(this, &MainForm::OnDelete);
            sortBtn = gcnew Button(); sortBtn->Text = "����������"; sortBtn->Click += gcnew EventHandler(this, &MainForm::OnSort);
            findBtn = gcnew Button(); findBtn->Text = "�����"; findBtn->Click += gcnew EventHandler(this, &MainForm::OnFind);
            priceBtn = gcnew Button(); priceBtn->Text = "�� ����"; priceBtn->Click += gcnew EventHandler(this, &MainForm::OnPriceFilter);
            servicesBtn = gcnew Button(); servicesBtn->Text = "������"; servicesBtn->Click += gcnew EventHandler(this, &MainForm::OnExtractServices);
            statsBtn = gcnew Button(); statsBtn->Text = "����������"; statsBtn->Click += gcnew EventHandler(this, &MainForm::OnStats);

            array<Button^>^ buttons = { loadBtn, saveBtn, addBtn, editBtn, deleteBtn, sortBtn, findBtn, priceBtn, servicesBtn, statsBtn };
            for each (Button ^ btn in buttons) {
                btn->Width = 90;
                btn->Height = 30;
                panel->Controls->Add(btn);
            }
            this->Controls->Add(panel);
        }

        void UpdateGrid(List<Ticket^>^ list) {
            grid->Columns->Clear();
            grid->Rows->Clear();
            grid->Columns->Add("passenger", "�������");
            grid->Columns->Add("train", "� ������");
            grid->Columns->Add("from", "������");
            grid->Columns->Add("to", "����");
            grid->Columns->Add("type", "���");
            grid->Columns->Add("price", "����");
            grid->Columns->Add("discount", "������");
            grid->Columns->Add("datetime", "���� � �����");

            for each (Ticket ^ t in list) {
                grid->Rows->Add(t->passengerName, t->trainNumber, t->departureCity, t->arrivalCity,
                    Convert::ToString(t->wagonType), t->price.ToString("F2"), t->hasDiscount ? "��" : "���",
                    String::Format("{0:D2}.{1:D2}.{2:D4} {3:D2}:{4:D2}:{5:D2}",
                        t->dateTime.date.day, t->dateTime.date.month, t->dateTime.date.year,
                        t->dateTime.time.hour, t->dateTime.time.minute, t->dateTime.time.second));
            }
        }

        String^ PromptInput(String^ text, String^ caption) {
            Form^ prompt = gcnew Form();
            prompt->Width = 400;
            prompt->Height = 150;
            prompt->Text = caption;

            Label^ textLabel = gcnew Label();
            textLabel->Left = 10;
            textLabel->Top = 10;
            textLabel->Text = text;
            textLabel->Width = 360;

            TextBox^ inputBox = gcnew TextBox();
            inputBox->Left = 10;
            inputBox->Top = 40;
            inputBox->Width = 360;

            Button^ confirmation = gcnew Button();
            confirmation->Text = "OK";
            confirmation->Left = 290;
            confirmation->Width = 80;
            confirmation->Top = 70;
            confirmation->DialogResult = System::Windows::Forms::DialogResult::OK;

            prompt->Controls->Add(textLabel);
            prompt->Controls->Add(inputBox);
            prompt->Controls->Add(confirmation);
            prompt->AcceptButton = confirmation;
            if (prompt->ShowDialog() == System::Windows::Forms::DialogResult::OK)
                return inputBox->Text;
            else
                return nullptr;
        }

        void OnLoad(Object^, EventArgs^) {
            OpenFileDialog^ ofd = gcnew OpenFileDialog();
            ofd->Filter = "��������� ����� (*.txt)|*.txt";
            ofd->Title = "������� ���� ���� ������";
            ofd->Multiselect = false;

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    manager->Load(ofd->FileName);
                    UpdateGrid(manager->GetAll());
                    MessageBox::Show("���� ������� ��������.", "�����", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("������ ��� �������� �����:\n" + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void OnSave(Object^, EventArgs^) {
            SaveFileDialog^ sfd = gcnew SaveFileDialog();
            sfd->Filter = "��������� ����� (*.txt)|*.txt";
            sfd->Title = "��������� ���� ������";
            sfd->DefaultExt = "txt";
            sfd->AddExtension = true;

            if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    manager->Save(sfd->FileName);
                    MessageBox::Show("���� ������� �������.", "�����", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("������ ��� ���������� �����:\n" + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void OnAdd(Object^, EventArgs^) {
            AddTicketForm^ form = gcnew AddTicketForm();
            if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK && form->result != nullptr) {
                manager->Add(form->result);
                UpdateGrid(manager->GetAll());
            }
        }

        void OnEdit(Object^, EventArgs^) {
            if (grid->SelectedRows->Count == 0) return;

            int index = grid->SelectedRows[0]->Index;
            Ticket^ ticket = manager->GetAll()[index];

            AddTicketForm^ form = gcnew AddTicketForm(ticket);
            if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK && form->result != nullptr) {
                manager->Update(index, form->result);
                UpdateGrid(manager->GetAll());
            }
        }

        void OnDelete(Object^, EventArgs^) {
            if (grid->SelectedRows->Count > 0) {
                int index = grid->SelectedRows[0]->Index;
                manager->Delete(index);
                UpdateGrid(manager->GetAll());
            }
        }

        void OnSort(Object^, EventArgs^) {
            manager->SortByTrainNumber();
            UpdateGrid(manager->GetAll());
        }

        void OnFind(Object^, EventArgs^) {
            String^ input = PromptInput("������� ����� ������:", "�����");

            // ���� ������������ ����� "������" ��� ������ �� ��� � ������ �������
            if (String::IsNullOrWhiteSpace(input)) {
                UpdateGrid(manager->GetAll());
                return;
            }

            try {
                unsigned short num = Convert::ToUInt16(input);
                auto results = manager->FindByTrainNumber(num);

                if (results->Count == 0) {
                    MessageBox::Show("������ � ����� ������� �� �������.", "�����", MessageBoxButtons::OK, MessageBoxIcon::Information);
                    UpdateGrid(manager->GetAll());
                }
                else {
                    UpdateGrid(results);
                }
            }
            catch (...) {
                MessageBox::Show("������������ ����� ������.", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            }
        }

        void OnPriceFilter(Object^, EventArgs^) {
            PriceRangeForm^ form = gcnew PriceRangeForm();
            if (form->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    auto filtered = manager->FilterByPrice(form->MinPrice, form->MaxPrice);
                    UpdateGrid(filtered);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("������ ��� ����������: " + ex->Message, "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void OnExtractServices(Object^, EventArgs^) {
            ServiceForm^ serviceForm = gcnew ServiceForm(manager);

            // �������� �� �������, ����� ��������� �������� ������� ��� �������� �������
            serviceForm->TicketsExtracted += gcnew EventHandler(this, &MainForm::OnTicketsExtracted);

            serviceForm->ShowDialog(this);
        }

        // ���������� ������� ���������� ������� �������
        void OnTicketsExtracted(Object^ sender, EventArgs^ e) {
            UpdateGrid(manager->GetAll());
        }


        void OnStats(Object^, EventArgs^) {
            auto departureStats = manager->DepartureCityStats();
            auto arrivalStats = manager->ArrivalCityStats();

            StatsForm^ statsForm = gcnew StatsForm(departureStats, arrivalStats);
            statsForm->ShowDialog(this);
        }


    };
}
