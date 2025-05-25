#pragma once

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Drawing;

public ref class PriceRangeForm : public Form {
public:
    float MinPrice;
    float MaxPrice;

private:
    TextBox^ tbMin;
    TextBox^ tbMax;
    Button^ btnOk;
    Button^ btnCancel;

public:
    PriceRangeForm() {
        this->Text = "����� ��������� ����";
        this->Size = Drawing::Size(320, 180);
        this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
        this->MaximizeBox = false;
        this->MinimizeBox = false;
        this->StartPosition = FormStartPosition::CenterParent;

        Label^ labelMin = gcnew Label();
        labelMin->Text = "����������� ����:";
        labelMin->Location = Point(10, 20);
        labelMin->AutoSize = true;

        Label^ labelMax = gcnew Label();
        labelMax->Text = "������������ ����:";
        labelMax->Location = Point(10, 60);
        labelMax->AutoSize = true;

        tbMin = gcnew TextBox();
        tbMin->Location = Point(140, 18);
        tbMin->Width = 120;

        tbMax = gcnew TextBox();
        tbMax->Location = Point(140, 58);
        tbMax->Width = 120;

        btnOk = gcnew Button();
        btnOk->Text = "OK";
        btnOk->Location = Point(60, 100);
        btnOk->DialogResult = System::Windows::Forms::DialogResult::OK;
        btnOk->Click += gcnew EventHandler(this, &PriceRangeForm::OnOkClick);

        btnCancel = gcnew Button();
        btnCancel->Text = "������";
        btnCancel->Location = Point(160, 100);
        btnCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;

        this->Controls->Add(labelMin);
        this->Controls->Add(labelMax);
        this->Controls->Add(tbMin);
        this->Controls->Add(tbMax);
        this->Controls->Add(btnOk);
        this->Controls->Add(btnCancel);

        this->AcceptButton = btnOk;
        this->CancelButton = btnCancel;
    }

private:
    void OnOkClick(Object^ sender, EventArgs^ e) {
        float minVal, maxVal;
        if (!Single::TryParse(tbMin->Text, minVal) || !Single::TryParse(tbMax->Text, maxVal)) {
            MessageBox::Show("����������, ������� ���������� �����.", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            this->DialogResult = System::Windows::Forms::DialogResult::None; 
            return;
        }
        if (minVal > maxVal) {
            MessageBox::Show("����������� ���� �� ����� ���� ������ ������������.", "������", MessageBoxButtons::OK, MessageBoxIcon::Error);
            this->DialogResult = System::Windows::Forms::DialogResult::None; 
            return;
        }
        MinPrice = minVal;
        MaxPrice = maxVal;
        // ���� ���������, �.�. DialogResult == OK
    }
};
