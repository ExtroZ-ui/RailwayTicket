#pragma once
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
            this->Text = "Статистика билетов по городам";
            this->Size = Drawing::Size(600, 500);
            this->StartPosition = FormStartPosition::CenterParent;

            Label^ depLabel = gcnew Label();
            depLabel->Text = "Отправления (Откуда)";
            depLabel->Location = Drawing::Point(10, 10);
            depLabel->AutoSize = true;

            DataGridView^ depGrid = gcnew DataGridView();
            depGrid->Location = Drawing::Point(10, 30);
            depGrid->Size = Drawing::Size(560, 180);
            depGrid->ReadOnly = true;
            depGrid->AllowUserToAddRows = false;
            depGrid->AllowUserToDeleteRows = false;
            depGrid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            depGrid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            depGrid->Columns->Add("city", "Город");
            depGrid->Columns->Add("count", "Количество билетов");

            FillGrid(depGrid, departureStats);

            Label^ arrLabel = gcnew Label();
            arrLabel->Text = "Прибытия (Куда)";
            arrLabel->Location = Drawing::Point(10, 230);
            arrLabel->AutoSize = true;

            DataGridView^ arrGrid = gcnew DataGridView();
            arrGrid->Location = Drawing::Point(10, 250);
            arrGrid->Size = Drawing::Size(560, 180);
            arrGrid->ReadOnly = true;
            arrGrid->AllowUserToAddRows = false;
            arrGrid->AllowUserToDeleteRows = false;
            arrGrid->AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode::Fill;
            arrGrid->SelectionMode = DataGridViewSelectionMode::FullRowSelect;
            arrGrid->Columns->Add("city", "Город");
            arrGrid->Columns->Add("count", "Количество билетов");

            FillGrid(arrGrid, arrivalStats);

            Button^ closeBtn = gcnew Button();
            closeBtn->Text = "Закрыть";
            closeBtn->Width = 100;
            closeBtn->Height = 30;
            closeBtn->Location = Drawing::Point(470, 440);
            closeBtn->Click += gcnew EventHandler(this, &StatsForm::OnClose);

            this->Controls->Add(depLabel);
            this->Controls->Add(depGrid);
            this->Controls->Add(arrLabel);
            this->Controls->Add(arrGrid);
            this->Controls->Add(closeBtn);
        }

    private:
        void FillGrid(DataGridView^ grid, Dictionary<String^, int>^ stats)  
        {  
           grid->Rows->Clear();  

           // Сортируем по убыванию количества, затем по алфавиту  
           List<KeyValuePair<String^, int>>^ list = gcnew List<KeyValuePair<String^, int>>(stats);  
           list->Sort(gcnew Comparison<KeyValuePair<String^, int>>(this, &StatsForm::CompareKeyValuePairs));  

           for each (auto pair in list) {  
               grid->Rows->Add(pair.Key, pair.Value);  
           }  
        }  

        int CompareKeyValuePairs(KeyValuePair<String^, int> a, KeyValuePair<String^, int> b)  
        {  
           if (a.Value != b.Value)  
               return b.Value.CompareTo(a.Value);  
           return a.Key->CompareTo(b.Key);  
        }

        void OnClose(Object^, EventArgs^) {
            this->Close();
        }
    };
}
