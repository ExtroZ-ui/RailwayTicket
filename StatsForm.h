#pragma once

namespace TrainTickets {

    using namespace System;
    using namespace System::Windows::Forms;
    using namespace System::Collections::Generic;

    public ref class StatsForm : public Form
    {
    public:
        StatsForm(Dictionary<String^, int>^ departureStats, Dictionary<String^, int>^ arrivalStats)
        {
            this->Text = "Статистика по городам: Из / В / Итого";
            this->Size = Drawing::Size(600, 500);
            this->StartPosition = FormStartPosition::CenterParent;

            Label^ titleLabel = gcnew Label();
            titleLabel->Text = "Города, отсортированные по общему числу билетов (из + в)";
            titleLabel->Location = Drawing::Point(10, 10);
            titleLabel->AutoSize = true;

            DataGridView^ grid = gcnew DataGridView();
            grid->Location = Drawing::Point(10, 40);
            grid->Size = Drawing::Size(560, 380);
            grid->ReadOnly = true;
            grid->AllowUserToAddRows = false;
            grid->AllowUserToDeleteRows = false;
            grid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            grid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;

            grid->Columns->Add("city", "Город");
            grid->Columns->Add("from", "Из");
            grid->Columns->Add("to", "В");
            grid->Columns->Add("total", "Итого");

            FillGrid(grid, departureStats, arrivalStats);

            Button^ closeBtn = gcnew Button();
            closeBtn->Text = "Закрыть";
            closeBtn->Width = 100;
            closeBtn->Height = 30;
            closeBtn->Location = Drawing::Point(470, 430);
            closeBtn->Click += gcnew EventHandler(this, &StatsForm::OnClose);

            this->Controls->Add(titleLabel);
            this->Controls->Add(grid);
            this->Controls->Add(closeBtn);
        }

    private:
        void FillGrid(DataGridView^ grid, Dictionary<String^, int>^ depStats, Dictionary<String^, int>^ arrStats)
        {
            grid->Rows->Clear();

            Dictionary<String^, Tuple<int, int>^>^ combined = gcnew Dictionary<String^, Tuple<int, int>^>();

            // Заполняем из departure
            for each (auto pair in depStats) {
                if (!combined->ContainsKey(pair.Key)) {
                    combined[pair.Key] = gcnew Tuple<int, int>(pair.Value, 0);
                }
                else {
                    int arr = combined[pair.Key]->Item2;
                    combined[pair.Key] = gcnew Tuple<int, int>(pair.Value, arr);
                }
            }

            // Заполняем из arrival
            for each (auto pair in arrStats) {
                if (!combined->ContainsKey(pair.Key)) {
                    combined[pair.Key] = gcnew Tuple<int, int>(0, pair.Value);
                }
                else {
                    int dep = combined[pair.Key]->Item1;
                    combined[pair.Key] = gcnew Tuple<int, int>(dep, pair.Value);
                }
            }

            // Переводим в список и сортируем
            List<Tuple<String^, int, int, int>^>^ list = gcnew List<Tuple<String^, int, int, int>^>();
            for each (auto pair in combined) {
                int from = pair.Value->Item1;
                int to = pair.Value->Item2;
                int total = from + to;
                list->Add(gcnew Tuple<String^, int, int, int>(pair.Key, from, to, total));
            }

            list->Sort(gcnew Comparison<Tuple<String^, int, int, int>^>(this, &StatsForm::CompareStats));

            for each (auto entry in list) {
                grid->Rows->Add(entry->Item1, entry->Item2, entry->Item3, entry->Item4);
            }
        }

        int CompareStats(Tuple<String^, int, int, int>^ a, Tuple<String^, int, int, int>^ b)
        {
            if (a->Item4 != b->Item4)
                return b->Item4.CompareTo(a->Item4); // по убыванию итого
            return a->Item1->CompareTo(b->Item1);    // по алфавиту
        }

        void OnClose(Object^, EventArgs^) {
            this->Close();
        }
    };
}
