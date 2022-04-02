#pragma once
#include "Solver.h"

namespace EMSolver2D {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	//System::Drawing::Bitmap btm = gcnew System::Drawing::Bitmap(1150,550);

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			this->bmp = gcnew Bitmap(bmpSizeX, bmpSizeY);
			//
			//TODO: Add the constructor code here
			//
			//for (int i = 0; i < bmpSizeX; i++)
				//for (int j = 0; j < bmpSizeY; j++)
					//this->bmp->SetPixel(i, j, System::Drawing::Color::FromArgb(1, int(i%256), 0, int(j %256)));
			InitializeComponent();
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public:System::Drawing::Bitmap^ bmp;
	public: int bmpSizeX = 1000;
	public: int bmpSizeY = 500;
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button1;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox1->Location = System::Drawing::Point(0, 70);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(bmpSizeX, bmpSizeY);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &MainForm::pictureBox1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label1->Location = System::Drawing::Point(123, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(92, 32);
			this->label1->TabIndex = 1;
			this->label1->Text = L"label1";
			this->label1->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label2->Location = System::Drawing::Point(346, 24);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(92, 32);
			this->label2->TabIndex = 2;
			this->label2->Text = L"label2";
			this->label2->Click += gcnew System::EventHandler(this, &MainForm::label2_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(673, 24);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(157, 32);
			this->button1->TabIndex = 3;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1680, 1024);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->Text = L"EMSolver2D";
			this->Resize += gcnew System::EventHandler(this, &MainForm::MainForm_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->pictureBox1->BackColor = System::Drawing::Color::FromArgb(100, 0, 0, 0);
		changeColor(this->bmp, this->Cursor->Position.X, this->Cursor->Position.Y);
		this->pictureBox1->Image = bmp;
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->label1->Text = this->Width.ToString();
	}

	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
		this->label2->Text = this->Height.ToString();
	}
	private: System::Void MainForm_Resize(System::Object^ sender, System::EventArgs^ e) {
		//int pictureBoxSizeX = this->Width - 250;
		//int pictureBoxSizeY = this->Height - 200;
		//this->pictureBox1->Size = System::Drawing::Size(pictureBoxSizeX, pictureBoxSizeY);
		//this->pictureBox1->BackColor = System::Drawing::Color::Black;
		this->label1->Text = this->Width.ToString();
		this->label2->Text = this->Height.ToString();
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		for (int i = 0; i < bmpSizeX; i++)
			for (int j = 0; j < bmpSizeY; j++)
				this->bmp->SetPixel(i, j, System::Drawing::Color::FromArgb(255, int(i * 255 / bmpSizeX), 0, int(j * 255 / bmpSizeY)));
		this->pictureBox1->Image = this->bmp;
	}
	};
}