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
            this->Text = "Железнодорожная касса";
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

            loadBtn = gcnew Button(); loadBtn->Text = "Загрузить"; loadBtn->Click += gcnew EventHandler(this, &MainForm::OnLoad);
            saveBtn = gcnew Button(); saveBtn->Text = "Сохранить"; saveBtn->Click += gcnew EventHandler(this, &MainForm::OnSave);
            addBtn = gcnew Button(); addBtn->Text = "Добавить"; addBtn->Click += gcnew EventHandler(this, &MainForm::OnAdd);
            editBtn = gcnew Button(); editBtn->Text = "Изменить"; editBtn->Click += gcnew EventHandler(this, &MainForm::OnEdit);
            deleteBtn = gcnew Button(); deleteBtn->Text = "Удалить"; deleteBtn->Click += gcnew EventHandler(this, &MainForm::OnDelete);
            sortBtn = gcnew Button(); sortBtn->Text = "Сортировка"; sortBtn->Click += gcnew EventHandler(this, &MainForm::OnSort);
            findBtn = gcnew Button(); findBtn->Text = "Поиск"; findBtn->Click += gcnew EventHandler(this, &MainForm::OnFind);
            priceBtn = gcnew Button(); priceBtn->Text = "По цене"; priceBtn->Click += gcnew EventHandler(this, &MainForm::OnPriceFilter);
            servicesBtn = gcnew Button(); servicesBtn->Text = "Услуги"; servicesBtn->Click += gcnew EventHandler(this, &MainForm::OnExtractServices);
            statsBtn = gcnew Button(); statsBtn->Text = "Статистика"; statsBtn->Click += gcnew EventHandler(this, &MainForm::OnStats);

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
            grid->Columns->Add("passenger", "Фамилия");
            grid->Columns->Add("train", "№ Поезда");
            grid->Columns->Add("from", "Откуда");
            grid->Columns->Add("to", "Куда");
            grid->Columns->Add("type", "Тип");
            grid->Columns->Add("price", "Цена");
            grid->Columns->Add("discount", "Льгота");
            grid->Columns->Add("datetime", "Дата и время");

            for each (Ticket ^ t in list) {
                grid->Rows->Add(t->passengerName, t->trainNumber, t->departureCity, t->arrivalCity,
                    Convert::ToString(t->wagonType), t->price.ToString("F2"), t->hasDiscount ? "Да" : "Нет",
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
            ofd->Filter = "Текстовые файлы (*.txt)|*.txt";
            ofd->Title = "Открыть файл базы данных";
            ofd->Multiselect = false;

            if (ofd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    manager->Load(ofd->FileName);
                    UpdateGrid(manager->GetAll());
                    MessageBox::Show("Файл успешно загружен.", "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Ошибка при загрузке файла:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void OnSave(Object^, EventArgs^) {
            SaveFileDialog^ sfd = gcnew SaveFileDialog();
            sfd->Filter = "Текстовые файлы (*.txt)|*.txt";
            sfd->Title = "Сохранить базу данных";
            sfd->DefaultExt = "txt";
            sfd->AddExtension = true;

            if (sfd->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
                try {
                    manager->Save(sfd->FileName);
                    MessageBox::Show("Файл успешно сохранён.", "Успех", MessageBoxButtons::OK, MessageBoxIcon::Information);
                }
                catch (Exception^ ex) {
                    MessageBox::Show("Ошибка при сохранении файла:\n" + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
            String^ input = PromptInput("Введите номер поезда:", "Поиск");

            // Если пользователь нажал "Отмена" или ничего не ввёл — просто выходим
            if (String::IsNullOrWhiteSpace(input)) {
                UpdateGrid(manager->GetAll());
                return;
            }

            try {
                unsigned short num = Convert::ToUInt16(input);
                auto results = manager->FindByTrainNumber(num);

                if (results->Count == 0) {
                    MessageBox::Show("Поезда с таким номером не найдено.", "Поиск", MessageBoxButtons::OK, MessageBoxIcon::Information);
                    UpdateGrid(manager->GetAll());
                }
                else {
                    UpdateGrid(results);
                }
            }
            catch (...) {
                MessageBox::Show("Некорректный номер поезда.", "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
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
                    MessageBox::Show("Ошибка при фильтрации: " + ex->Message, "Ошибка", MessageBoxButtons::OK, MessageBoxIcon::Error);
                }
            }
        }

        void OnExtractServices(Object^, EventArgs^) {
            ServiceForm^ serviceForm = gcnew ServiceForm(manager);

            // Подписка на событие, чтобы обновлять основную таблицу при удалении билетов
            serviceForm->TicketsExtracted += gcnew EventHandler(this, &MainForm::OnTicketsExtracted);

            serviceForm->ShowDialog(this);
        }

        // Обработчик события обновления таблицы билетов
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
