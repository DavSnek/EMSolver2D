#pragma once
#include "Solver.h"
#include <string>

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
	public: Solver* sol = new Solver(0.1,0.7,30.0,15.0);
	public: int t = 0;
	public:System::Drawing::Bitmap^ bmp;
	private: const int bmpSizeX = 1000;
	private: const int bmpSizeY = 500;

	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Button^ button2;
	public: System::Windows::Forms::Timer^ timer1;

	private: System::Windows::Forms::Label^ label3;
	public:
	private: System::ComponentModel::IContainer^ components;



	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->trackBar1 = (gcnew System::Windows::Forms::TrackBar());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label3 = (gcnew System::Windows::Forms::Label());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::White;
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox1->Location = System::Drawing::Point(-1, 76);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(1000, 500);
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
			this->button1->Text = L"Stop";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 10;
			this->trackBar1->Location = System::Drawing::Point(921, 14);
			this->trackBar1->Maximum = 0;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(217, 56);
			this->trackBar1->TabIndex = 5;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MainForm::trackBar1_Scroll);
			this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &MainForm::trackBar1_ValueChanged);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(482, 24);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(131, 32);
			this->button2->TabIndex = 6;
			this->button2->Text = L"Start";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// timer1
			// 
			this->timer1->Interval = 1;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(1182, 76);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(44, 16);
			this->label3->TabIndex = 8;
			this->label3->Text = L"label3";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1680, 1024);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->Text = L"EMSolver2D";
			this->Resize += gcnew System::EventHandler(this, &MainForm::MainForm_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->pictureBox1->Image = bmp;
	}
	private: System::Void label1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->label1->Text = this->Width.ToString();
	}

	private: System::Void label2_Click(System::Object^ sender, System::EventArgs^ e) {
		this->label2->Text = this->timer1->Enabled.ToString();
	}
	private: System::Void MainForm_Resize(System::Object^ sender, System::EventArgs^ e) {
		this->label1->Text = this->Width.ToString();
		this->label2->Text = this->Height.ToString();
	}
	private: System::Void button2_Click(System::Object^ sender, System::EventArgs^ e) {
		this->label2->Text = L"True";
		this->timer1->Enabled = true;
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->label2->Text = L"False";
		this->timer1->Enabled = false;
	}
	
	private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e) {
		for (int i = 0; i < this->sol->x_len; i++)
			for (int j = 0; j < this->sol->y_len; j++)
				if (this->sol->SimReg[this->trackBar1->Value][i][j].Ex > 0)
					this->bmp->SetPixel(i, j, System::Drawing::Color::FromArgb(255, int(this->sol->SimReg[this->trackBar1->Value][i][j].Ex * 255) % 256, 0, 0));
				else
					this->bmp->SetPixel(i, j, System::Drawing::Color::FromArgb(255, 0, 0, int(-1 * this->sol->SimReg[this->trackBar1->Value][i][j].Ex * 255) % 256));
		this->pictureBox1->Image = this->bmp;
	}
	private: System::Void trackBar1_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		this->label1->Text = this->sol->t_end.ToString();
		double fieldMax = 1;
		int i, j;
		int w = this->pictureBox1->Width;
		int h = this->pictureBox1->Height;

		for (int x = 0; x < w; x++)
			for (int y = 0; y < h; y++) {
				i = int(x * (this->sol->x_len - 1)/ w);
				j = int(y * (this->sol->y_len - 1)/ h);
				if (abs(this->sol->SimReg[this->trackBar1->Value][i][j].Hz) > fieldMax)
					fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][i][j].Hz);
				if (this->sol->SimReg[this->trackBar1->Value][i][j].Hz > 0)
					this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, int(this->sol->SimReg[this->trackBar1->Value][i][j].Hz * 255 / fieldMax), 0, 0));
				else
					this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, int(-1 * this->sol->SimReg[this->trackBar1->Value][i][j].Hz * 255 / fieldMax)));
			}
		this->label3->Text = fieldMax.ToString();
		this->pictureBox1->Image = this->bmp;
	}
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		//this->sol->ExplicitTE();
		//this->sol->PEC();
		//this->sol->primitiveABC();
		this-> sol->demoYoung();
		//this->sol->PML();
		this->label2->Text = this->sol->t_last.ToString();
		this->trackBar1->Maximum = this->sol->t_last;
		this->trackBar1->Value = this->sol->t_last - 1;
	}

};
}