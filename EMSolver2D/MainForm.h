#pragma once
#include "Solver.h"
#include <string>
#include "EMConstants.h"
#include "Particle.h"

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
			//Initialize bitmap values
			this->bmp = gcnew Bitmap(this->sol->x_len, this->sol->y_len);
			InitializeComponent();
			this->img = gcnew Bitmap(this->pictureBox1->Width, this->pictureBox1->Height);

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
	private: float um = 10;
	public: Solver* sol = new Solver(um*0.01/2,0.5,um, um);
	public: int t = 0;
	public:System::Drawing::Bitmap^ bmp;
	public:System::Drawing::Bitmap^ img;
	private: double fieldMax = 1e-50;

	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::TrackBar^ trackBar1;
	private: System::Windows::Forms::Button^ button2;
	public: System::Windows::Forms::Timer^ timer1;

	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::TrackBar^ trackBar2;
	private: System::Windows::Forms::ComboBox^ comboBox1;
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
			this->trackBar2 = (gcnew System::Windows::Forms::TrackBar());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Gainsboro;
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox1->Location = System::Drawing::Point(0, 62);
			this->pictureBox1->Margin = System::Windows::Forms::Padding(2);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(536, 462);
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			this->pictureBox1->Click += gcnew System::EventHandler(this, &MainForm::pictureBox1_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label1->Location = System::Drawing::Point(92, 20);
			this->label1->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(70, 26);
			this->label1->TabIndex = 1;
			this->label1->Text = L"label1";
			this->label1->Click += gcnew System::EventHandler(this, &MainForm::label1_Click);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 16.2F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(238)));
			this->label2->Location = System::Drawing::Point(260, 20);
			this->label2->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(70, 26);
			this->label2->TabIndex = 2;
			this->label2->Text = L"label2";
			this->label2->Click += gcnew System::EventHandler(this, &MainForm::label2_Click);
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(505, 20);
			this->button1->Margin = System::Windows::Forms::Padding(2);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(118, 26);
			this->button1->TabIndex = 3;
			this->button1->Text = L"Stop";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// trackBar1
			// 
			this->trackBar1->LargeChange = 10;
			this->trackBar1->Location = System::Drawing::Point(691, 11);
			this->trackBar1->Margin = System::Windows::Forms::Padding(2);
			this->trackBar1->Maximum = 0;
			this->trackBar1->Name = L"trackBar1";
			this->trackBar1->Size = System::Drawing::Size(163, 45);
			this->trackBar1->TabIndex = 5;
			this->trackBar1->Scroll += gcnew System::EventHandler(this, &MainForm::trackBar1_Scroll);
			this->trackBar1->ValueChanged += gcnew System::EventHandler(this, &MainForm::trackBar1_ValueChanged);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(362, 20);
			this->button2->Margin = System::Windows::Forms::Padding(2);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(98, 26);
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
			this->label3->Location = System::Drawing::Point(910, 117);
			this->label3->Margin = System::Windows::Forms::Padding(2, 0, 2, 0);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 8;
			this->label3->Text = L"label3";
			// 
			// trackBar2
			// 
			this->trackBar2->LargeChange = 1000;
			this->trackBar2->Location = System::Drawing::Point(877, 69);
			this->trackBar2->Maximum = 1000000;
			this->trackBar2->Minimum = 1;
			this->trackBar2->Name = L"trackBar2";
			this->trackBar2->Size = System::Drawing::Size(104, 45);
			this->trackBar2->SmallChange = 10;
			this->trackBar2->TabIndex = 9;
			this->trackBar2->Value = this->trackBar2->Maximum;
			this->trackBar2->ValueChanged += gcnew System::EventHandler(this, &MainForm::trackBar2_ValueChanged);
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(6) { L"Ex", L"Ey", L"Ez", L"Hx", L"Hy", L"Hz" });
			this->comboBox1->Location = System::Drawing::Point(754, 61);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(121, 21);
			this->comboBox1->TabIndex = 10;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::comboBox1_SelectedIndexChanged);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1155, 687);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->trackBar2);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->trackBar1);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Margin = System::Windows::Forms::Padding(2);
			this->Name = L"MainForm";
			this->ShowIcon = false;
			this->Text = L"EMSolver2D";
			this->Resize += gcnew System::EventHandler(this, &MainForm::MainForm_Resize);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar2))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void pictureBox1_Click(System::Object^ sender, System::EventArgs^ e) {
	
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
		this->timer1->Enabled = true;
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
		this->timer1->Enabled = false;
	}
	private: System::Void trackBar2_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		this->label3->Text = (this->fieldMax * this->trackBar2->Value).ToString();
	}
	private: System::Void trackBar1_Scroll(System::Object^ sender, System::EventArgs^ e) {
		
	}
	private: System::Void trackBar1_ValueChanged(System::Object^ sender, System::EventArgs^ e) {
		float maxcol = this->fieldMax* abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
		this->label1->Text = this->sol->t_end.ToString();
		if (this->comboBox1->SelectedItem == "Ex")
		{
			for (int x = 0; x < this->sol->x_len; x++)
				for (int y = 0; y < this->sol->y_len; y++) {
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ex) > this->fieldMax)
					{
						this->fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ex);
						maxcol = this->fieldMax * abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
					}
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ex) < maxcol)
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Ex > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(this->sol->SimReg[this->trackBar1->Value][x][y].Ex * 255 / maxcol), 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(-1 * this->sol->SimReg[this->trackBar1->Value][x][y].Ex * 255 / maxcol), 0, 0, 255));
					}
					else
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Ex > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, 255));
					}
				}
		}
		else if (this->comboBox1->SelectedItem == "Ey")
		{
			for (int x = 0; x < this->sol->x_len; x++)
				for (int y = 0; y < this->sol->y_len; y++) {
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ey) > this->fieldMax)
					{
						this->fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ey);
						maxcol = this->fieldMax * abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
					}
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ey) < maxcol)
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Ey > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(this->sol->SimReg[this->trackBar1->Value][x][y].Ey * 255 / maxcol), 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(-1 * this->sol->SimReg[this->trackBar1->Value][x][y].Ey * 255 / maxcol), 0, 0, 255));
					}
					else
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Ey > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, 255));
					}
				}
		}
		else if (this->comboBox1->SelectedItem == "Ez")
		{
			for (int x = 0; x < this->sol->x_len; x++)
				for (int y = 0; y < this->sol->y_len; y++) {
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ez) > this->fieldMax)
					{
						this->fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ez);
						maxcol = this->fieldMax * abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
					}
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Ez) < maxcol)
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Ez > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(this->sol->SimReg[this->trackBar1->Value][x][y].Ez * 255 / maxcol), 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(-1 * this->sol->SimReg[this->trackBar1->Value][x][y].Ez * 255 / maxcol), 0, 0, 255));
					}
					else
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Ez > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, 255));
					}
				}
		}
		else if (this->comboBox1->SelectedItem == "Hx")
		{
			for (int x = 0; x < this->sol->x_len; x++)
				for (int y = 0; y < this->sol->y_len; y++) {
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hx) > this->fieldMax)
					{
						this->fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hx);
						maxcol = this->fieldMax * abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
					}
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hx) < maxcol)
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Hx > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(this->sol->SimReg[this->trackBar1->Value][x][y].Hx * 255 / maxcol), 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(-1 * this->sol->SimReg[this->trackBar1->Value][x][y].Hx * 255 / maxcol), 0, 0, 255));
					}
					else
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Hx > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, 255));
					}
				}
		}
		else if (this->comboBox1->SelectedItem == "Hy")
		{
			for (int x = 0; x < this->sol->x_len; x++)
				for (int y = 0; y < this->sol->y_len; y++) {
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hy) > this->fieldMax)
					{
						this->fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hy);
						maxcol = this->fieldMax * abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
					}
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hy) < maxcol)
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Hy > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(this->sol->SimReg[this->trackBar1->Value][x][y].Hy * 255 / maxcol), 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(-1 * this->sol->SimReg[this->trackBar1->Value][x][y].Hy * 255 / maxcol), 0, 0, 255));
					}
					else
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Hy > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, 255));
					}
				}
		}
		else if (this->comboBox1->SelectedItem == "Hz")
		{
			for (int x = 0; x < this->sol->x_len; x++)
				for (int y = 0; y < this->sol->y_len; y++) {
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hz) > this->fieldMax)
					{
						this->fieldMax = abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hz);
						maxcol = this->fieldMax * abs(float(this->trackBar2->Value) / float(this->trackBar2->Maximum));
					}
					if (abs(this->sol->SimReg[this->trackBar1->Value][x][y].Hz) < maxcol)
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Hz > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(this->sol->SimReg[this->trackBar1->Value][x][y].Hz * 255 / maxcol), 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(int(-1 * this->sol->SimReg[this->trackBar1->Value][x][y].Hz * 255 / maxcol), 0, 0, 255));
					}
					else
					{
						if (this->sol->SimReg[this->trackBar1->Value][x][y].Hz > 0)
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 255, 0, 0));
						else
							this->bmp->SetPixel(x, y, System::Drawing::Color::FromArgb(255, 0, 0, 255));
					}
				}
		}
		for (auto p : this->sol->particles)
		{
			this->bmp->SetPixel(int(p->pos[0]), int(p->pos[1]), System::Drawing::Color::Green);
		}

		this->label3->Text = maxcol.ToString();
		this->pictureBox1->Image = img;
		Graphics^ test = Graphics::FromImage(this->pictureBox1->Image);
		//test->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;
		test->Clear(this->pictureBox1->BackColor);
		test->DrawImage(this->bmp, 0, 0, this->pictureBox1->Width, this->pictureBox1->Height);
	}
	private: System::Void timer1_Tick(System::Object^ sender, System::EventArgs^ e) {
		//this->sol->sourceOnePoint(10,1);
		//this->sol->sourcePlaneWave(10, 1, "x", 2);
		//this->sol->PICpos();
		//this->sol->ExplicitTE();
		//this->sol->PICvel();
		//this->sol->demoCherenkov();
		this->sol->demoPIC();
		//this->sol->PEC();
		//this->sol->PBC();
		//this->sol->primitiveABC();
		//this-> sol->demoYoung();
		//this->sol->PML();
		this->label2->Text = this->sol->t_last.ToString();
		this->trackBar1->Maximum = this->sol->t_last;
		this->trackBar1->Value = this->sol->t_last - 1;
	}

	private: System::Void comboBox1_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e) {
		this->fieldMax = 0;
	}
};
}